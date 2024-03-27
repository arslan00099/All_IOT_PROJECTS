## FreedomTools_Generic firmware

The FreedomTools_Generic firmware is developed using PlatformIO and VS Code, providing several advantages over the Arduino environment. PlatformIO offers flexibility with support for multiple platforms and frameworks, while VS Code provides a feature-rich code editor. PlatformIO's efficient build system reduces compilation times and firmware size. The unified project structure and CLI facilitate automation and collaboration. Debugging support is robust, and the library manager simplifies library management. Overall, this combination enhances development experience, productivity, and enables advanced firmware development for ESP32 devices.

## Project Structure

The firmware project has the following structure:

- `src/main.cpp`: The main file containing the `setup()` and `loop()` functions. This is where the firmware's execution starts.

- `lib`: A directory containing the header files used in the firmware. These files define functions, classes, and structures used throughout the project.

- `data`: A directory containing HTML files and other binaries used in the firmware. These files are stored in a separate LittleFS partition on the device. On the local system, they can be found in the `data` directory.

- `protobuf`: A directory containing the Protobuf declarations for the firmware. These files define the message structures used for communication.

## Setting up environment

Find detailed instructions on to set up your environment [here](https://code.k9network.org/K9network/FreedomTools_Generic/src/branch/master/Firmware/platform_io.md) 

## Todo:

- Implement void createDefaultSettings(); 
- Implement bool loadSettings(Settings& settings);; 
- Implement bool saveSettings(const Settings& settings); 

## Configuration and Settings

The firmware handles the `settings.bin` file, which contains device configuration settings stored in the Protobuf format. It provides functions to load and save the settings, as well as create default settings if the file doesn't exist.

### Loading and Saving Settings

- `bool loadSettings(Settings& settings)`: Loads the device settings from the `settings.bin` file. Returns `true` if the loading is successful, `false` otherwise.

- `bool saveSettings(const Settings& settings)`: Saves the device settings to the `settings.bin` file. Returns `true` if the saving is successful, `false` otherwise.

### Default Settings

- `void createDefaultSettings()`: Creates default settings if the `settings.bin` file doesn't exist.
