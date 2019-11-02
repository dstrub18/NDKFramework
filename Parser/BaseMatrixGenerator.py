from MatrixDict import MatrixDict
from ComponentType import ComponentType, NonlinearComponents
from Component import Component
import numpy as np
import re


class BaseMatrixGenerator:

    Z_INDUCTOR_VAL = -1
    Z_CAPACTITOR_VAL = 1

    N_NODE_FROM = 1
    N_NODE_TO = -1

    def __init__(self, components: [Component], nodeNames: [str]):
        self.matrixDict = MatrixDict()

        self.Nr = None
        self.Nx = None
        self.Nv = None
        self.No = None
        self.Nn = None

        self.Gr = None
        self.Xlc = None
        self.Rv0 = None
        self.Z = None

        # vstacked together to create Nu, where the first numInputs rows of inputs will be the first
        self.NuInputs = None
        self.NuSources = None
        self.Nu = None

        # Values for sources (vector u in state space model, assumed to be constant (DC))
        self.uInputs = None
        self.uSources = None
        self.sourceVoltages = None

        self.Nopao = None
        self.Nopai = None

        self.numNodes = 0
        self.numResistors = 0
        self.numReactives = 0
        self.numVcc = 0
        self.numInputs = 0
        self.numPotentiometers = 0
        self.numOutputs = 0
        self.numNonlinearFunctions = 0
        self.numSources = 0
        self.numOpamps = 0

        self._countComponents(components, nodeNames)
        self._createMatrices()
        self._populateMatrices(components)

        self._parseOutputs(nodeNames)

        self._concatU()
        self._addMatricesToDict()
        self._addRestToDict()

    def _countComponents(self, components: [Component], nodes: [str]) -> None:
        self.numNodes = len(nodes)
        self.numResistors = len([c for c in components if c.cType == ComponentType.RESISTOR])
        self.numReactives = len([c for c in components
                                 if c.cType == ComponentType.CAPACITOR
                                 or c.cType == ComponentType.INDUCTOR])
        self.numVcc = len([c for c in components if c.cType == ComponentType.VOLTAGE_SOURCE])
        self.numInputs = len([c for c in components if c.cType == ComponentType.VOLTAGE_INPUT])
        numPotTops = len([c for c in components if c.cType == ComponentType.POT_TOP])
        numPotBottoms = len([c for c in components if c.cType == ComponentType.POT_BOTTOM])
        self.numPotentiometers = numPotTops + numPotBottoms
        self.numNonlinearFunctions = sum([c.getNumNonlinearEquations() for c in components
                                          if c.cType in NonlinearComponents.ALL_NONLINEARS])
        self.numOpamps = len([c for c in components if c.cType == ComponentType.OPAMP])
        if not (numPotTops == numPotBottoms):
            print("Number of potentiometer tops (", numPotTops, ") does not match bottoms (", numPotBottoms, ").")

    def _createMatrices(self) -> None:
        self.Nr = np.zeros((self.numResistors, self.numNodes))
        self.Nx = np.zeros((self.numReactives, self.numNodes))
        self.Nu = np.zeros((self.numInputs + self.numVcc, self.numNodes))
        self.Nv = np.zeros((self.numPotentiometers, self.numNodes))
        self.No = np.zeros((self.numOutputs, self.numNodes))
        self.Nn = np.zeros((self.numNonlinearFunctions, self.numNodes))

        self.Gr = np.zeros((self.numResistors, self.numResistors))
        self.Xlc = np.zeros((self.numReactives, self.numReactives))
        self.Rv0 = np.zeros((self.numPotentiometers, self.numPotentiometers))
        self.Z = np.zeros((self.numReactives, self.numReactives))

        self.NuInputs = np.zeros((self.numInputs, self.numNodes))
        self.NuSources = np.zeros((self.numVcc, self.numNodes))

        self.uInputs = np.zeros((self.numInputs, 1))
        self.uSources = np.zeros((self.numVcc, 1))
        self.uOpamps = np.zeros((self.numOpamps, 1))

        self.Nopao = np.zeros((self.numOpamps, self.numNodes))
        self.Nopai = np.zeros((self.numOpamps, self.numNodes))

    def _populateMatrices(self, components: [Component]) -> None:
        # Components with external counter:
        resistorCount = 0
        reactiveCount = 0
        nonlinearityCount = 0
        opampCount = 0
        # All other components have internal counting in their netlist naming

        # TODO: Fix all this mess with _addNumber

        for component in components:
            if component.cType == ComponentType.RESISTOR:
                if not component.getNodeFromNumber() == Component.INVALID_NODE_OR_GROUND:
                    self.Nr[resistorCount, component.getNodeFromNumber()] = BaseMatrixGenerator.N_NODE_FROM
                if not component.getNodeToNumber() == Component.INVALID_NODE_OR_GROUND:
                    self.Nr[resistorCount, component.getNodeToNumber()] = BaseMatrixGenerator.N_NODE_TO
                self.Gr[resistorCount, resistorCount] = 1 / component.value
                resistorCount += 1

            elif component.cType == ComponentType.INDUCTOR or component.cType == ComponentType.CAPACITOR:
                if component.getNodeFromNumber() != Component.INVALID_NODE_OR_GROUND:
                    self.Nx[reactiveCount, component.getNodeFromNumber()] = BaseMatrixGenerator.N_NODE_FROM
                if component.getNodeToNumber() != Component.INVALID_NODE_OR_GROUND:
                    self.Nx[reactiveCount, component.getNodeToNumber()] = BaseMatrixGenerator.N_NODE_TO
                if component.cType == ComponentType.INDUCTOR:
                    self.Xlc[reactiveCount, reactiveCount] = component.value
                    self.Z[reactiveCount, reactiveCount] = BaseMatrixGenerator.Z_INDUCTOR_VAL
                else:
                    self.Xlc[reactiveCount, reactiveCount] = component.value
                    self.Z[reactiveCount, reactiveCount] = BaseMatrixGenerator.Z_CAPACTITOR_VAL
                reactiveCount += 1

            elif component.cType == ComponentType.POT_TOP or component.cType == ComponentType.POT_BOTTOM:
                if component.cType == ComponentType.POT_TOP:
                    potOffset = 0
                else:
                    potOffset = 1
                potIndex = 2 * component.componentIndex + potOffset
                if component.getNodeFromNumber() != Component.INVALID_NODE_OR_GROUND:
                    self.Nv[potIndex, component.getNodeFromNumber()] = BaseMatrixGenerator.N_NODE_FROM
                if component.getNodeToNumber() != Component.INVALID_NODE_OR_GROUND:
                    self.Nv[potIndex, component.getNodeToNumber()] = BaseMatrixGenerator.N_NODE_TO
                self.Rv0[potIndex, potIndex] = component.value

            elif component.cType == ComponentType.VOLTAGE_INPUT:
                if component.getNodeFromNumber() != Component.INVALID_NODE_OR_GROUND:
                    self.NuInputs[component.componentIndex, component.getNodeFromNumber()] = BaseMatrixGenerator.N_NODE_FROM
                if component.getNodeToNumber() != Component.INVALID_NODE_OR_GROUND:
                    self.NuInputs[component.componentIndex, component.getNodeToNumber()] = BaseMatrixGenerator.N_NODE_TO
                # self.uInputs defaulted to 0

            elif component.cType == ComponentType.VOLTAGE_SOURCE:
                if component.getNodeFromNumber() != Component.INVALID_NODE_OR_GROUND:
                    self.NuSources[component.componentIndex, component.getNodeFromNumber()] = BaseMatrixGenerator.N_NODE_FROM
                if component.getNodeToNumber() != Component.INVALID_NODE_OR_GROUND:
                    self.NuSources[component.componentIndex, component.getNodeToNumber()] = BaseMatrixGenerator.N_NODE_TO
                self.uSources = component.value

            elif component.cType == ComponentType.NONLINEAR_TWO_LEG:
                if component.getNodeFromNumber() != Component.INVALID_NODE_OR_GROUND:
                    self.Nn[nonlinearityCount, component.getNodeFromNumber()] = BaseMatrixGenerator.N_NODE_FROM
                if component.getNodeToNumber() != Component.INVALID_NODE_OR_GROUND:
                    self.Nn[nonlinearityCount, component.getNodeToNumber()] = BaseMatrixGenerator.N_NODE_TO
                nonlinearityCount += 1

            elif component.cType == ComponentType.BJT_THREE_LEG:
                collectorIdx, baseIdx, emitterIdx = component.nodeNumbers
                self._addNumber(self.Nn, nonlinearityCount, collectorIdx, self.N_NODE_FROM)
                self._addNumber(self.Nn, nonlinearityCount, baseIdx, self.N_NODE_TO)
                nonlinearityCount += 1
                self._addNumber(self.Nn, nonlinearityCount, collectorIdx, self.N_NODE_FROM)
                self._addNumber(self.Nn, nonlinearityCount, emitterIdx, self.N_NODE_TO)
                nonlinearityCount += 1

            elif component.cType == ComponentType.TRIODE:
                anodeIdx, gridIdx, cathodeIdx = component.nodeNumbers
                self._addNumber(self.Nn, nonlinearityCount, anodeIdx, self.N_NODE_FROM)
                self._addNumber(self.Nn, nonlinearityCount, gridIdx, self.N_NODE_TO)
                nonlinearityCount += 1
                self._addNumber(self.Nn, nonlinearityCount, anodeIdx, self.N_NODE_FROM)
                self._addNumber(self.Nn, nonlinearityCount, cathodeIdx, self.N_NODE_TO)
                nonlinearityCount += 1

            elif component.cType == ComponentType.OPAMP:
                noninverting, inverting, _, vee, out = component.nodeNumbers
                self._addNumber(self.Nopai, opampCount, noninverting, self.N_NODE_FROM)
                self._addNumber(self.Nopai, opampCount, inverting, self.N_NODE_TO)
                self._addNumber(self.Nopao, opampCount, out, self.N_NODE_FROM)
                self._addNumber(self.Nopao, opampCount, vee, self.N_NODE_TO)

    def _addNumber(self, matrix, componentCount, nodeIdx, value):
        if nodeIdx != Component.INVALID_NODE_OR_GROUND:
            matrix[componentCount, nodeIdx] = value

    def _parseOutputs(self, nodeNames: [str]):
        # TODO: Ugly workaround. Change this to same behaviour as resistors
        outputCount = 0     # TODO: Redundant
        outputs: [(int, int)] = []
        for (nodeIndex, node) in enumerate(nodeNames):
            out = re.match("VOUT([0-9]+).*", node, flags=re.IGNORECASE)
            if out:     # Matches regex
                outputIndex = int(out.group(1))
                outputs.append((outputIndex, nodeIndex))
                outputCount += 1
            else:
                # TODO: Invalid input: handle (throw error)
                pass

        self.numOutputs = outputCount
        self.No = np.zeros((self.numOutputs, self.numNodes))
        for (outIdx, nodeIdx) in outputs:
            self.No[outIdx, nodeIdx] = 1

    def _concatU(self):
        self.Nu = np.vstack((self.NuInputs, self.NuSources))
        # self.uInputs will always be zeros. Only contains placeholders for inputs
        self.sourceVoltages = np.vstack((self.uInputs, self.uSources, self.uOpamps))
        self.numSources = self.numVcc + self.numInputs + self.numOpamps

    def _addMatricesToDict(self):
        self.matrixDict.addToDict("Nr", self.Nr)
        self.matrixDict.addToDict("Nx", self.Nx)
        self.matrixDict.addToDict("Nv", self.Nv)
        self.matrixDict.addToDict("No", self.No)
        self.matrixDict.addToDict("Nn", self.Nn)
        self.matrixDict.addToDict("Nu", self.Nu)
        self.matrixDict.addToDict("Gr", self.Gr)
        self.matrixDict.addToDict("Xlc", self.Xlc)
        self.matrixDict.addToDict("Rv0", self.Rv0)
        self.matrixDict.addToDict("Z", self.Z)
        self.matrixDict.addToDict("Nopao", self.Nopao)
        self.matrixDict.addToDict("Nopai", self.Nopai)

    def _addRestToDict(self):
        self.matrixDict.addToDict("numNodes", self.numNodes)
        self.matrixDict.addToDict("numResistors", self.numResistors)
        self.matrixDict.addToDict("numReactives", self.numReactives)
        self.matrixDict.addToDict("numInputs", self.numInputs)
        self.matrixDict.addToDict("numSources", self.numSources)
        self.matrixDict.addToDict("numPotentiometers", self.numPotentiometers)
        self.matrixDict.addToDict("numOutputs", self.numOutputs)
        self.matrixDict.addToDict("numNonlinears", self.numNonlinearFunctions)

        self.matrixDict.addToDict("sourceVoltages", self.sourceVoltages)
        self.matrixDict.addToDict("numOpamps", self.numOpamps)

        # TODO: Add Node names
        # TODO: Add Pot names

    def getMatrixDict(self) -> MatrixDict:
        return self.matrixDict

