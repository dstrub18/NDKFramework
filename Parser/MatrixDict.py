from typing import Dict
import json
import numpy as np


class MatrixDict:

    def __init__(self):
        self.matrixDict: Dict[str, np.ndarray] = {}

    def getMatrix(self, matrixName) -> np.ndarray:
        # TODO
        return np.array([])

    def addToDict(self, key, value):
        # TODO: Handle duplicates (return bool or throw except if add was unsuccessful)
        self.matrixDict[key] = value

    def printSelf(self):
        printMatrix = {key: (value.tolist() if isinstance(value, np.ndarray) else value) for (key, value) in self.matrixDict.items()}
        print(json.dumps(printMatrix, indent=4))

    def printMatrixToFile(self, absFilePath):
        printMatrix = {key: (value.tolist() if isinstance(value, np.ndarray) else value) for (key, value) in
                       self.matrixDict.items()}
        with open(absFilePath, 'w') as outfile:
            json.dump(printMatrix, outfile, indent=4)
