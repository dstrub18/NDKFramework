from ComponentType import ComponentType


class Component:

    INVALID_NODE_OR_GROUND = -1
    COMPONENT_NO_NAME = None
    NODE_FROM_INDEX = 0
    NODE_TO_INDEX = 1

    REMOVABLE_NODE_NAME = "0"

    def __init__(self, cType, label, nodes, componentIndex=None, value=None):
        self.cType: ComponentType = cType
        self.label: str = label
        self.nodes: [str] = nodes
        self.nodeNumbers: [int] = []
        self.value: float = value
        # Used for potentiometers, voltage source, opamps and opamp sources
        self.componentIndex: int = componentIndex

    def setNodeNumber(self, nodeNumber, legIndex):
        self.nodeNumbers[legIndex] = nodeNumber

    def getNodeNumber(self, legIndex):
        return self.nodeNumbers[legIndex]

    def getNodeName(self, legIndex):
        return self.nodes[legIndex]


class TwoLegged(Component):

    def __init__(self, cType, label, nodeFrom, nodeTo, value=None, model=None, componentIndex=None):
        super().__init__(cType, label, [nodeFrom, nodeTo], componentIndex, value)
        self.model: str = model
        self.nodeNumbers.append(Component.INVALID_NODE_OR_GROUND)   # Node From
        self.nodeNumbers.append(Component.INVALID_NODE_OR_GROUND)   # Node To

    def setNodeFromNumber(self, nodeNumber):
        self.setNodeNumber(nodeNumber, Component.NODE_FROM_INDEX)

    def setNodeToNumber(self, nodeNumber):
        self.setNodeNumber(nodeNumber, Component.NODE_FROM_INDEX)

    def getNodeFromNumber(self):
        return self.nodeNumbers[Component.NODE_FROM_INDEX]

    def getNodeToNumber(self):
        return self.nodeNumbers[Component.NODE_TO_INDEX]

    def getNumNonlinearEquations(self):
        return len(self.nodeNumbers) - 1


class NLegged(Component):

    def __init__(self, cType, label, nodes, model, componentIndex=None, value=None):
        super().__init__(cType, label, nodes, componentIndex, value)
        self.model: str = model
        for _ in range(len(nodes)):
            self.nodeNumbers.append(Component.INVALID_NODE_OR_GROUND)

    def getNumNonlinearEquations(self):
        return len(self.nodeNumbers) - 1

