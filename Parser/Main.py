from Parser import Parser
from BaseMatrixGenerator import BaseMatrixGenerator
import sys
import os


class Main:

    def __init__(self):
        if len(sys.argv) != 3:
            print("\033[91m[INPUT ERROR]: Invalid number of input arguments.\033[0m")
            self.printHelp()

        inFile = sys.argv[1]
        outFileRel = sys.argv[2]
        currentWorkingDir = os.getcwd()
        outFileAbs = currentWorkingDir + "/" + outFileRel

        print(inFile)
        print(outFileAbs)

        parser = Parser(inFile)

        components = parser.getComponents()
        nodesNames = parser.getNodeNames()
        nonlinearModels = parser.get_nonlinear_models()
        opampModels = parser.get_opamp_models()

        baseMatrices = BaseMatrixGenerator(components, nodesNames)
        matrixDict = baseMatrices.getMatrixDict()
        matrixDict.addToDict("nodeNames", nodesNames)
        matrixDict.addToDict("potentiometerNames", [])  # TODO: Implement pot names
        matrixDict.addToDict("nonlinearModels", nonlinearModels)
        matrixDict.addToDict("opampModels", opampModels)
        matrixDict.printMatrixToFile(outFileAbs)

        print("Json data written to " + outFileAbs + ".")

    @staticmethod
    def printHelp():
        print("Usage: python3 Main.py relative_file_in relative_file_out")
        sys.exit(1)

m = Main()
