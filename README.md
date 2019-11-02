# NDKFramework
Repository of the NDKFramework as showcased at Audio Developer Conference 2019

## Dependencies

The following applications are needed to use the NDKFramework: 
* [LtSpice](https://www.analog.com/en/design-center/design-tools-and-calculators/ltspice-simulator.html)
* [MATLAB](https://www.mathworks.com/products/matlab.html). Optional but highly recommended for offline-debugging.
* [JUCE](https://juce.com)

As well as the following libraries: 
* [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page).  Use `brew install eigen` if you're using brew on macOS
* [nlohmann-json]( https://github.com/nlohmann/json) Use `brew install nlohmann-json` if you're using brew on macOS
* [NumPy](https://numpy.org/). Refer to your Python package manager of choice to install it.

## Usage

The NDKFramework facilitates the following workflow: 
1. Design your circuit in LTSpice. Abide to the specified naming of the components, otherwise, they will not be parsed. Also refer to the list of currently supported components.
2. Save the netlist of your circuit as follows:
    * Right-click on the editing panel -> View -> SPICE Netlist
    * CMD-S to save the netlist
3. Export the netlist to a .json file as follows:
    * Open the terminal
    * Type `python3 <relative-path-to-Main.py> <relative-path-to-netlist-file> <relative-desired-output-path>
4. Open the .json file in Matlab using the template files in this repository to 
    * As of now, you have to **explicitly** specify the equations for the nonlinear components you used in your circuit. Refer to the           example circuits.
5. Open the NDKCircuitTemplate in the Projucer. in the `prepareToPlay()` method, specify the .json path in the stateSpaceProcessor constructor.
    * `stateSpaceProcessor = std::make_unique<StateSpaceProcessor>("path-to-file.json", sampleRate); `
6. Build the project.

## Purpose

## Currently supported components
