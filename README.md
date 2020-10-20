# simconnect-toolbox

SimConnect Toolbox for MATLAB/Simulink

This repository provides a Toolbox for MATLAB/Simulink with the following blocks:

- SimConnectSource
- SimConnectSink
- SimConnectInput

:warning: **The blocks only work properly using the solver *FixedStepDiscrete* (e.g. with step size `0.03`) and
a *Simulation Pacing* set to `1`.**

## Building

### Prerequisites

- Microsoft Visual Studio 2019 (e.g. Community Edition)
- MATLAB / Simulink (tested with R2020b)
- Microsoft Flight Simulator SDK
- Blockfactory
- CMake (tested with 3.18.4)

### Commands

Ensure the environment variable `MSFS_SDK` is properly pointing to the MSFS SDK location.

Use the following commands to get this repository and build it:

```lang-bash
# download and build blockfactory
git clone https://github.com/robotology/blockfactory.git
cd blockfactory
cmake -S . -B build
cmake --build build --config Release
cmake --install build --config Release --prefix install
cd ..

# download and build simconnect-toolbox
git clone https://github.com/aguther/simconnect-toolbox.git
cd simconnect-toolbox
cmake -S . -B build
cmake --build build --config Release
```

## SimConnect Source & Sink

This block allows to read/write data from/to SimConnect.

The blocks have the following parameters:

- Configuration Index
- Connection Name
- Variables

### Configuration Index

The connection configuration index found in the `SimConnect.cfg`.

### Connection Name

The name of the connection to be used when calling the SimConnect API.

### Variable specification

Variables have to be in the following format: `VARIABLE NAME, UNIT;`

The variable names and units can be found in the SimConnect SDK.

:warning: Types with any string data type are not supported.

Example:

```lang-none
G FORCE, GFORCE;
PLANE PITCH DEGREES, RADIANS;
PLANE BANK DEGREES, RADIANS;
AIRSPEED INDICATED, KNOTS;
PLANE ALTITUDE, FEET;
STRUCT WORLD ROTATION VELOCITY, STRUCT;
LIGHT STROBE ON, BOOL;
```

![SimConnectSource-Parameters](https://github.com/aguther/simconnect-toolbox/raw/main/images/SimConnectSource-Parameters.png "SimConnectSource-Parameters")
![SimConnectSink-Parameters](https://github.com/aguther/simconnect-toolbox/raw/main/images/SimConnectSink-Parameters.png "SimConnectSink-Parameters")

#### Structs Types

Struct types are provided / consumed as vector to Simulink.

The following struct types are supported:

- `SIMCONNECT_DATATYPE_LATLONALT`
- `SIMCONNECT_DATATYPE_XYZ`

## SimConnect Input

This block allows to read input event data from SimConnect.

The block has the following parameters:

- Configuration Index
- Connection Name
- Variables

### Configuration Index

The connection configuration index found in the `SimConnect.cfg`.

### Connection Name

The name of the connection to be used when calling the SimConnect API.

### Variable specification

Variables have to be in the following format: `INPUT EVENT ID, SHOULD MASK;`

The event ids can be found in the SimConnect SDK. The second parameter specifies if the event id should be 
forwarded to the sim or not. A value of `TRUE` will mask the event id meaning only the Simulink model will receive it.

Example:

```lang-none
AXIS_ELEVATOR_SET, TRUE;
AXIS_AILERONS_SET, TRUE;
AXIS_RUDDER_SET, FALSE;
```

The following input events are currently supported:

- `AXIS_ELEVATOR_SET`
- `AXIS_AILERONS_SET`
- `AXIS_RUDDER_SET`
- `AXIS_ELEV_TRIM_SET`
- `AXIS_SPOILER_SET`
- `AXIS_FLAPS_SET`
- `AXIS_LEFT_BRAKE_SET`
- `AXIS_RIGHT_BRAKE_SET`
- `AXIS_THROTTLE_SET`
- `AXIS_THROTTLE1_SET`
- `AXIS_THROTTLE2_SET`
- `AXIS_THROTTLE3_SET`
- `AXIS_THROTTLE4_SET`
- `AXIS_MIXTURE_SET`
- `AXIS_MIXTURE1_SET`
- `AXIS_MIXTURE2_SET`
- `AXIS_MIXTURE3_SET`
- `AXIS_MIXTURE4_SET`
- `AXIS_PROPELLER_SET`
- `AXIS_PROPELLER1_SET`
- `AXIS_PROPELLER2_SET`
- `AXIS_PROPELLER3_SET`
- `AXIS_PROPELLER4_SET`

:information_source: The range `[-16384, +16384]` is converted to `[-1.0, 1.0]`.

![SimConnectInput-Parameters](https://github.com/aguther/simconnect-toolbox/raw/main/images/SimConnectInput-Parameters.png "SimConnectInput-Parameters")

## Example Model

This repository includes an example model `matlab/SimConnectToolboxExample.slx` that demonstrates the functionality.

![SimConnectToolboxExample](https://github.com/aguther/simconnect-toolbox/raw/main/images/SimConnectToolboxExample.png "SimConnectToolboxExample.slx")

## Open Points / Wishlist

- Allow configuration index to be set via input
- Improve parameter checking (distinguish between event ids and data variables)
- Documentation of classes and methods

## Thank you notice

A special thanks to:

- Ferigo, Diego
- Traversaro, Silvio
- Romano, Francesco
- Pucci, Daniele

They provide a nice framework to make development of Simulink blocks an toolboxes easier and
the code better understandable than using plain MATLAB/Simulink API.

See also here: <https://github.com/robotology/blockfactory>
