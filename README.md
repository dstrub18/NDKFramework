# NDKFramework
Repository of the NDKFramework as showcased at Audio Developer Conference 2019.

## Purpose

Analog circuit emulations can be achieved using State-Space processing. However, prototyping in this domain can be cumbersome, as the derivation of its computational components is non-modular. For each circuit iteration, the necessary matrices have to be computed anew. The NDKFramework reduces this modularity issue by automatically deriving the matrices from the netlist specifications of the circuit in the application LTSpice. A set of Matlab scripts and extensions to the JUCE framework allow for offline-debugging and real-time simulation of analog circuits.

A comprehensive report, including the mathematical background, implementation details as well as the outcomes of our evaluation can be found [here](https://github.com/dstrub18/NDKFramework/blob/master/doc/NDK_Framework_Academic_Report.pdf).

## Dependencies

The following applications are needed to use the NDKFramework: 
* [LTSpice](https://www.analog.com/en/design-center/design-tools-and-calculators/ltspice-simulator.html)
* [MATLAB](https://www.mathworks.com/products/matlab.html). Optional but highly recommended for offline-debugging.
* [JUCE](https://juce.com)

As well as the following libraries: 
* [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page).  Use `brew install eigen` if you're using brew on macOS
* [nlohmann-json]( https://github.com/nlohmann/json) Use `brew install nlohmann-json` if you're using brew on macOS
* [NumPy](https://numpy.org/). Refer to your Python package manager of choice to install it.

## Usage

The NDKFramework facilitates the following workflow: 
1. Design your circuit in LTSpice. Abide to the specified [naming conventions](https://github.com/dstrub18/NDKFramework/blob/master/README.md#naming-conventions) of the components, otherwise, they will not be parsed. Also refer to the list of [currently supported components](https://github.com/dstrub18/NDKFramework/blob/master/README.md#currently-supported-components).
2. Save the netlist of your circuit as follows:
    * Right-click on the editing panel -> View -> SPICE Netlist
    * CMD-S to save the netlist
3. Export the netlist to a .json file as follows:
    * Open the terminal
    * Type `python3 <relative-path-to-Main.py> <relative-path-to-netlist-file> <relative-desired-output-path> `
4. Open the .json file in Matlab using the template files in this repository.
    * Specify the path to the `.json `file. Also load an audio file as input.
    * As of now, you have to **explicitly** specify the equations for the nonlinear components you used in your circuit. For reference,         please look at the           [example circuits](https://github.com/dstrub18/NDKFramework/tree/master/Example_Circuits).
    * By running the simulation, you can see plots of voltages, currents, and states within the circuit
5. Open the NDKCircuitTemplate in the Projucer. in the `prepareToPlay()` method, specify the `.json ` path in the stateSpaceProcessor constructor as follows: 
    * `stateSpaceProcessor = std::make_unique<StateSpaceProcessor>("path-to-file.json", sampleRate); `
6. Build the project.

## Currently supported components
* Voltage sources
* Resistor
* Potentiometers
* Capacitor
* Inductor
* 1N914 Diode (Shockley equation)
* 2N2222 NPN Transistor (Ebers-Moll equation)
* 12AX7 Triode (Equations according [Dempwolf](http://recherche.ircam.fr/pub/dafx11/Papers/76_e.pdf))
* Operational amplifier (Only linear behaviour)


## Naming conventions
* ` # ` is any number (multiple digits are allowed)
* `N_FROM ` and `N_TO ` are the nodes the component is connected from / to
* All component groups are required to be **zero-indexed**
* `VIN`is the entry point for the audio samples later.
```
Resistor       := R#<NAME>    N_FROM N_TO    VALUE
Capacitor      := C#<NAME>    N_FROM N_TO    VALUE
Inductor       := L#<NAME>    N_FROM N_TO    VALUE
Voltage Input  := VIN#<NAME>  N_FROM N_TO
Voltage Supply := VCC#<NAME>  N_FROM N_TO    <"DC">   VALUE
```
* As potentiometers are not natively supported in LTSpice, two resistors with the maximum and minimum value have to be created instead. 

```
Potentiometer Top := RT#<NAME> N_FROM N_TO VALUE
Potentiometer Btm := RB#<NAME> N_FROM N_TO 

Voltage Out := VOUT#<NAME> 
Ground      := 0

Diode    := D#<NAME>    N_FROM N_TO    MODEL
NPN      := Q#<NAME>    NC NB NE NS    MODEL
Triode   := XU#<NAME>   NA NG NC       MODEL
Opamp    := XUOPA<NAME> NI I  VCC   VEE   OUT   MODEL
```
* For a comprehensive understanding of LTSpice abbreviations, please look [here](http://bwrcs.eecs.berkeley.edu/Classes/IcBook/SPICE/UserGuide/elements_fr.html?fbclid=IwAR1Vp1dBDk9oxgnC1lqnxfJvEzfL6ccTTzjXdqwHUVvBoiOIojku5Wr46lI).
## Potentiometer handling in C++
* As of now, the only way to let the user change the circuit behavior is via potentiometers. These can be accessed by creating in a slider in the `PluginEditor` and call the function `StateSpaceProcessor::updatePotValues` with the corresponding index for each potentiometer.
