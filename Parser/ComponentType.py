from enum import IntEnum

class ComponentType(IntEnum):
    RESISTOR = 1
    CAPACITOR = 2
    INDUCTOR = 3
    VOLTAGE_SOURCE = 4
    VOLTAGE_INPUT = 5
    POT_TOP = 6
    POT_BOTTOM = 7
    NONLINEAR_TWO_LEG = 8
    INVALID = 9
    BJT_THREE_LEG = 10
    OPAMP = 11
    OPAMP_SOURCE = 12
    TRIODE = 13

    @staticmethod
    def getType(char):
        if char == "RT":
            return ComponentType.POT_TOP
        if char == "RB":
            return ComponentType.POT_BOTTOM
        if char == "R":
            return ComponentType.RESISTOR
        if char == "C":
            return ComponentType.CAPACITOR
        if char == "L":
            return ComponentType.INDUCTOR
        if char == "VCC":
            return ComponentType.VOLTAGE_SOURCE
        if char == "VIN":
            return ComponentType.VOLTAGE_INPUT
        if char == "D":
            return ComponentType.NONLINEAR_TWO_LEG
        if char == "Q":
            return ComponentType.BJT_THREE_LEG
        if char == "XUOPA":
            return ComponentType.OPAMP
        if char == "VOPA":
            return ComponentType.OPAMP_SOURCE
        if char == "XU":
            return ComponentType.TRIODE
        else:
            return ComponentType.INVALID


class NonlinearComponents:
    ALL_NONLINEARS = list([ComponentType.NONLINEAR_TWO_LEG, ComponentType.BJT_THREE_LEG, ComponentType.TRIODE])
