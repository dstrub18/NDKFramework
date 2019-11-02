from typing import Union
from Component import Component, TwoLegged, NLegged
from ComponentType import ComponentType, NonlinearComponents

import re


class Parser:

    INT = "[0-9]+"
    P_BEHAVIOUR = "(LIN|LOG)?"
    P_LABEL = "(R|L|C)"
    S_LABEL = "VCC"
    I_LABEL = "VIN"
    D_LABEL = "D"
    BJT_LABEL = "Q"
    OPAMP_LABEL = "XUOPA"
    TRIODE_LABEL = "XU"
    POT_TOP = "RT"
    POT_BOTTOM = "RB"
    OPTIONAL_NAME = "[A-Za-z0-9_]*"
    MANDATORY_NAME = "[A-Za-z0-9_]+"
    NODE = "[A-Za-z0-9_]+"
    ANYTHING = "($|\s*$|\s+.*$)"
    MODEL = "[A-Za-z0-9]+"
    SUFFIX1 = "(t|g|meg|k|m|u|\xb5|n)?"     # \xb5 matches iso-8859-1 mu
    SUFFIX2 = "(ohms|ohm|farads|f|henry|h|v|volts)?"
    SPACE = "\s+"
    FLOAT_POSITIVE = "(([0-9]+(\\.[0-9]+)?)|(\\.[0-9]+))"
    FLOAT_ANY = "-?" + FLOAT_POSITIVE
    DC_SPACE = "(DC )?"

    Passive = [P_LABEL, MANDATORY_NAME, SPACE, NODE, SPACE, NODE, SPACE, FLOAT_POSITIVE, SUFFIX1, SUFFIX2, ANYTHING]
    Input = [I_LABEL, INT, OPTIONAL_NAME, SPACE, NODE, SPACE, NODE, ANYTHING]
    Source = [S_LABEL, INT, OPTIONAL_NAME, SPACE, NODE, SPACE, NODE, SPACE, DC_SPACE, FLOAT_ANY, SUFFIX1, SUFFIX2, ANYTHING]
    Diode = [D_LABEL, OPTIONAL_NAME, SPACE, NODE, SPACE, NODE, SPACE, MODEL, ANYTHING]
    PotTop = [POT_TOP, INT, OPTIONAL_NAME, SPACE, NODE, SPACE, NODE, SPACE, FLOAT_POSITIVE, SUFFIX1, SUFFIX2, ANYTHING]
    PotBottom = [POT_BOTTOM, INT, SPACE, NODE, SPACE, NODE, SPACE, FLOAT_POSITIVE, SUFFIX1, SUFFIX2, ANYTHING]
    Bjt = [BJT_LABEL, INT, OPTIONAL_NAME, SPACE, NODE, SPACE, NODE, SPACE, NODE, SPACE, FLOAT_ANY, SPACE, MODEL, ANYTHING]
    Opamp = [OPAMP_LABEL, INT, OPTIONAL_NAME, SPACE, NODE, SPACE, NODE, SPACE, NODE, SPACE, NODE, SPACE, NODE, SPACE, MODEL]
    Triode = [TRIODE_LABEL, INT, OPTIONAL_NAME, SPACE, NODE, SPACE, NODE, SPACE, NODE, SPACE, MODEL, ANYTHING]

    Jfet = ""  # TODO
    Mosfet = ""  # TODO

    def __init__(self, filename: str):
        self.componentNames: [str] = []
        self.components: [Component] = []
        self.outputIndices: [int] = []
        self.outputNames: [str] = []

        self.parseFile(filename)
        self._compileNodes()

    def parseFile(self, fileName):
        # TODO: Handle incorrect file names
        with open(fileName, encoding='iso-8859-1') as netlist:
            for line in netlist:
                component = self._parseLine(line.strip())
                if component:
                    self.components.append(component)

    def getComponents(self):
        return self.components

    def getNodeNames(self):
        return self.nodeNames

    @staticmethod
    def _parseLine(line: str) -> Union[Component, None]:
        if not line:
            # line is empty
            return None
        elif re.match("".join(Parser.PotTop), line, flags=re.IGNORECASE):
            vals = Parser._tokenize(line, Parser.PotTop)
            cType = ComponentType.getType(vals[0])
            index = int(vals[1])
            label = vals[2]
            nodeFrom = vals[3]
            nodeTo = vals[4]
            baseVal = int(vals[5])
            multiplier = Parser._parsePrefix(vals[6])
            return TwoLegged(cType, label, nodeFrom, nodeTo, baseVal * multiplier, componentIndex=index)

        elif re.match("".join(Parser.PotBottom), line, flags=re.IGNORECASE):
            vals = Parser._tokenize(line, Parser.PotBottom)
            cType = ComponentType.getType(vals[0])
            index = int(vals[1])
            nodeFrom = vals[2]
            nodeTo = vals[3]
            baseVal = int(vals[4])
            multiplier = Parser._parsePrefix(vals[5])
            return TwoLegged(cType, Component.COMPONENT_NO_NAME, nodeFrom, nodeTo, baseVal * multiplier, componentIndex=index)

        elif re.match("".join(Parser.Passive), line, flags=re.IGNORECASE):
            vals = Parser._tokenize(line, Parser.Passive)
            cType = ComponentType.getType(vals[0])
            label = vals[1]
            nodeFrom = vals[2]
            nodeTo = vals[3]
            baseVal = float(vals[4])
            multiplier = Parser._parsePrefix(vals[5])
            return TwoLegged(cType, label, nodeFrom, nodeTo, baseVal * multiplier)

        elif re.match("".join(Parser.Input), line, flags=re.IGNORECASE):
            vals = Parser._tokenize(line, Parser.Input)
            cType = ComponentType.getType(vals[0])
            index = int(vals[1])
            label = vals[2]
            nodeFrom = vals[3]
            nodeTo = vals[4]
            return TwoLegged(cType, label, nodeFrom, nodeTo, componentIndex=index)

        elif re.match("".join(Parser.Source), line, flags=re.IGNORECASE):
            vals = Parser._tokenize(line, Parser.Source)
            cType = ComponentType.getType(vals[0])
            index = int(vals[1])
            label = vals[2]
            nodeFrom = vals[3]
            nodeTo = vals[4]
            baseVal = float(vals[5])
            multiplier = Parser._parsePrefix(vals[6])
            return TwoLegged(cType, label, nodeFrom, nodeTo, value=baseVal * multiplier, componentIndex=index)

        elif re.match("".join(Parser.Diode), line, flags=re.IGNORECASE):
            vals = Parser._tokenize(line, Parser.Diode)
            cType = ComponentType.getType(vals[0])
            label = vals[1]
            nodeFrom = vals[2]
            nodeTo = vals[3]
            model = vals[4]
            return TwoLegged(cType, label, nodeFrom, nodeTo, model=model)

        elif re.match("".join(Parser.Bjt), line, flags=re.IGNORECASE):
            vals = Parser._tokenize(line, Parser.Bjt)
            cType = ComponentType.getType(vals[0])
            label = vals[1]
            nodeC = vals[3]
            nodeB = vals[4]
            nodeE = vals[5]
            model = vals[7]
            return NLegged(cType, label, [nodeC, nodeB, nodeE], model)

        elif re.match("".join(Parser.Opamp), line, flags=re.IGNORECASE):
            # TODO: Parser only handles input vcc correctly, not vee
            vals = Parser._tokenize(line, Parser.Opamp)
            cType = ComponentType.getType(vals[0])
            index = int(vals[1])
            label = vals[2]
            nodeNonInv = vals[3]
            nodeInv = vals[4]
            # nodeVcc = vals[5] # Vcc node is not parsed, assumed to be removable
            nodeVee = vals[6]
            nodeOut = vals[7]
            model = vals[8]
            nodes = [nodeNonInv, nodeInv, Component.REMOVABLE_NODE_NAME, nodeVee, nodeOut]
            return NLegged(cType, label, nodes, model, componentIndex=index)

        elif re.match("".join(Parser.Triode), line, flags=re.IGNORECASE):
            vals = Parser._tokenize(line, Parser.Triode)
            cType = ComponentType.getType(vals[0])
            label = vals[1]
            nodeA = vals[3]
            nodeG = vals[4]
            nodeK = vals[5]
            model = vals[6]
            return NLegged(cType, label, [nodeA, nodeG, nodeK], model)


    @staticmethod
    def _tokenize(line, regexList):
        vals = []
        for regex in regexList:
            token = re.match(regex, line, flags=re.IGNORECASE)
            # Entire line should always be matched by _parseLine, hence no error checking here
            line = line[token.end():]
            if not (regex == Parser.SPACE or regex == Parser.ANYTHING or regex == Parser.DC_SPACE):  # TODO: Add ignored to separate list. Also Suffix2
                vals.append(token.group(0))
        return vals

    @staticmethod
    def _parsePrefix(char):
        if re.match("T", char, flags=re.IGNORECASE):
            return pow(10, 12)
        elif re.match("G", char, flags=re.IGNORECASE):
            return pow(10, 9)
        elif re.match("meg", char, flags=re.IGNORECASE):
            return pow(10, 6)
        elif re.match("k", char, flags=re.IGNORECASE):
            return pow(10, 3)
        elif re.match("m", char, flags=re.IGNORECASE):
            return pow(10, -3)
        elif re.match("u", char, flags=re.IGNORECASE):
            return pow(10, -6)
        # ISO-8859-1 mu
        elif re.match("\xb5", char, flags=re.IGNORECASE):
            return pow(10, -6)
        elif re.match("n", char, flags=re.IGNORECASE):
            return pow(10, -9)
        else:
            return 1.0

    def _compileNodes(self):
        nodes = set()
        for component in self.components:
            nodes.update(component.nodes)
        nodes.remove("0")                   # TODO: Handle possible error
        self.nodeNames = list(nodes)

        for component in self.components:
            for legNumber in range(len(component.nodes)):
                try:
                    idxFrom = self.nodeNames.index(component.getNodeName(legNumber))
                    component.setNodeNumber(idxFrom, legNumber)
                except ValueError:
                    # Connected to ground, which is removed. Not an error
                    pass

    def get_nonlinear_models(self):
        return [c.model for c in self.components if c.cType in NonlinearComponents.ALL_NONLINEARS]

    def get_opamp_models(self):
        return [c.model for c in self.components if c.cType == ComponentType.OPAMP]

    # def _parseOutputs(self):
    #     for (i, nodeName) in enumerate(self.nodeNames):
    #         if re.match("".join(Parser.OutputNode), line, flags=re.IGNORECASE):
    #         vals = Parser._tokenize(nodeName, Parser.OutputNode)
    #         index = int(vals[1])
    #         name = vals[2]
    #         self.output
    #         return Component(type, label, nodeFrom, nodeTo, baseVal * multiplier)
    #         nodeName



