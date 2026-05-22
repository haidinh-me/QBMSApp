# BMSApp Documentation 

## Project Overview

**BMSApp** is a Qt6 QML application for real-time Battery Management System (BMS) monitoring and control on Raspberry Pi with temperature-based GPIO fan control.

## Architecture

```
┌─────────────────────────────────────────────────────────┐
│                     QML UI Layer                        │
│  (Main.qml, Views, Components - DashboardView, etc.)    │
└────────────────────┬────────────────────────────────────┘
                     │
┌─────────────────────▼───────────────────────────────────┐
│            BmsViewModel (MVVM Pattern)                  │
│   - Q_PROPERTY bindings for QML data binding            │
│   - Q_INVOKABLE slots for user actions                  │
│   - Converts BmsManager data to QVariantMap             │
└────────────────────┬────────────────────────────────────┘
                     │
┌─────────────────────▼───────────────────────────────────┐
│                  BmsManager (Core Engine)               │
│   - Serial communication orchestration                  │
│   - Auto-polling with configurable interval             │
│   - GPIO fan speed control integration                  │
│   - Comprehensive error handling                        │
└────────────────────┬────────────────────────────────────┘
         ┌───────────┴───────────┬──────────────┐
         │                       │              │
    ┌────▼──────┐    ┌──────────▼──┐   ┌──────▼──────────┐
    │BmsSerial  │    │JbdProtocol  │   │GpioFanController│
    │Reader     │    │             │   │                 │
    │(UART)     │    │(Parsing)    │   │(PWM/Digital)    │
    └────┬──────┘    └─────────────┘   └────────┬────────┘
         │                                      │
    ┌────▼──────────────────┐        ┌──────────▼──────┐
    │ Serial Port (UART)    │        │ Python Process  │
    │ /dev/ttyUSB0 (9600)   │        │gpio_fan_ctrller │
    └───────────────────────┘        └────────┬────────┘
                                               │
                                    ┌──────────▼──────┐
                                    │ pigpiod Daemon  │
                                    │ (Persistent)    │
                                    └────────┬────────┘
                                             │
                                    ┌────────▼──────┐
                                    │ GPIO Hardware │
                                    │ PWM Pin       │
                                    └───────────────┘
```

## Module Documentation

### 1. **Main Application (main.cpp)**
```cpp
/// @brief Main application entry point for BMSApp
/// Sets up Qt6 QML engine, registers BmsViewModel as context property
```

**Key Functions:**
- `main(int argc, char *argv[])` - Initialize app and load QML module

### 2. **User Interface Layer (QML)**
- **Main.qml** - Root QML application window
- **qml/components/** - Reusable UI components
  - `AppShell.qml` - Main application shell
  - `BottomTabBar.qml` - Tab navigation
  - `ValueTile.qml` - Data display tile
  - `BatteryCellBar.qml` - Cell voltage visualization
  - `Gauge.qml` - Analog gauge display
  - `InfoPair.qml` - Label-value pair
  - `AppCard.qml` - Card container
- **qml/views/** - Application views
  - `DashboardView.qml` - Main dashboard
  - `PacksView.qml` - Battery pack info
  - `SettingsView.qml` - Settings and configuration

### 3. **BmsViewModel (ViewModel Layer)**

```cpp
/// @brief BmsViewModel - Qt MVVM bridge for BMS data
/// Provides QML-friendly interface to BMS operations
```

**@brief for Key Methods:**
- `snapshot()` - Get current BMS data as variant map
- `connectToUart()` - Connect to BMS via serial port
- `disconnectFromUart()` - Disconnect from BMS
- `refreshNow()` - Request BMS data immediately
- `reloadAvailablePorts()` - Reload serial port list
- `setPortName()` - Configure serial port
- `setBaudRate()` - Configure baud rate
- `setPollingIntervalMs()` - Configure polling frequency

**Q_PROPERTY Bindings:**
- `snapshot` - Current BMS data
- `connected` - Connection status
- `connectionState` - Connection state code
- `connectionStateText` - Localized state text
- `errorCode` - Last error code
- `errorMessage` - Last error message
- `statusMessage` - Current status
- `availablePorts` - List of serial ports
- `portName` - Selected port
- `baudRate` - Baud rate setting
- `pollingIntervalMs` - Poll interval

### 4. **BmsManager (Core Engine)**

```cpp
/// @brief BmsManager - Core BMS data manager
/// Manages serial communication, data parsing, and fan control
```

**Features:**
- Auto-reconnection with configurable polling
- Temperature-based PWM fan control
- Cell voltage monitoring
- Comprehensive error handling

**@brief for Key Methods:**
- `connectSerial()` - Connect to BMS via UART
- `disconnectSerial()` - Disconnect from BMS
- `requestBasicInfo()` - Request BMS data immediately
- `initializeFanControl()` - Initialize GPIO fan control
- `setFanDigitalThresholds()` - Set ON/OFF temperatures
- `setFanPwmRange()` - Set PWM speed temperature mapping
- `updateTemperature()` - Update fan speed based on temperature

**Enumerations:**

```cpp
enum class ConnectionState {
    Idle,           ///< Not connected
    Connecting,     ///< Attempting connection
    Online,         ///< Connected and receiving
    Degraded,       ///< Connected with issues
    Error           ///< Connection error
};

enum class ErrorCode {
    None,                   ///< No error
    InvalidConfig,          ///< Invalid settings
    OpenFailed,             ///< Serial port open failed
    PortClosed,             ///< Port closed unexpectedly
    WriteFailed,            ///< Write operation failed
    Timeout,                ///< Communication timeout
    InvalidFrame,           ///< Invalid frame format
    ChecksumMismatch,       ///< Checksum verification failed
    DeviceStatusError,      ///< BMS device error
    TransportError,         ///< Serial transport error
    Unknown                 ///< Unknown error
};
```

### 5. **BmsSerialReader (Serial Transport)**

```cpp
/// @brief BmsSerialReader - Serial port handler for UART communication
/// Manages low-level serial operations with async frame reception
```

**@brief for Key Methods:**
- `connectPort()` - Connect to serial port
- `disconnectPort()` - Disconnect
- `isConnected()` - Check connection status
- `writeFrame()` - Send frame to BMS
- `availablePorts()` - Get list of available ports

**Signals:**
- `frameReceived()` - New frame received
- `connectionChanged()` - Connection status changed
- `transportErrorOccurred()` - Transport error

### 6. **JbdProtocol (Protocol Handling)**

```cpp
/// @brief JbdProtocol - JBD BMS protocol encoder/decoder
/// Implements JBD (杰比特) serial protocol for BMS communication
```

**@brief for Key Methods:**
- `buildBasicInfoRequest()` - Build basic info command
- `buildCellVoltagesRequest()` - Build cell voltages command
- `parseFrame()` - Parse received frame and extract data

**Enumerations:**

```cpp
enum class FrameType {
    Unknown,        ///< Unknown frame
    BasicInfo,      ///< Battery info frame
    CellVoltages    ///< Cell voltage frame
};

enum class ParseErrorCode {
    None,                   ///< No error
    InvalidFormat,          ///< Invalid frame format
    BmsStatusError,         ///< BMS status error
    LengthMismatch,         ///< Length mismatch
    ChecksumMismatch,       ///< Checksum failed
    UnsupportedCommand,     ///< Unsupported command
    PayloadTooShort         ///< Insufficient data
};
```

### 7. **BmsSnapshot (Data Structure)**

```cpp
/// @brief BmsSnapshot - Data structure containing BMS status snapshot
/// Contains all current BMS parameters at a specific point in time
```

**Members:**
- `timestamp` - When snapshot was captured
- `packVoltage` - Total pack voltage (V)
- `current` - Battery current (A)
- `soc` - State of charge (0-100%)
- `temperatureC` - Temperature (°C)
- `alarms` - List of active alarms
- `cells` - Individual cell voltages (V)
- `remainingCapacityAh` - Remaining capacity (Ah)
- `designCapacityAh` - Design capacity (Ah)
- `cycleLife` - Charge/discharge cycles

### 8. **GpioFanController (Fan Control)**

```cpp
/// @brief GpioFanController - GPIO fan control for Raspberry Pi
/// Controls DC fan speed via PWM or Digital mode based on temperature
```

**Modes:**
```cpp
enum class FanMode {
    Digital,    ///< ON/OFF mode
    PWM         ///< Speed control 0-100%
};
```

**@brief for Key Methods:**
- `initialize()` - Initialize GPIO fan control
- `updateTemperature()` - Update fan speed based on temperature
- `setDigitalThresholds()` - Set ON/OFF temperature thresholds
- `setPwmRange()` - Set PWM temperature-to-speed mapping
- `setFanState()` - Set fan ON/OFF directly
- `setFanSpeed()` - Set PWM speed directly

**Signals:**
- `fanStateChanged()` - Fan state changed
- `fanSpeedChanged()` - PWM speed changed
- `temperatureUpdated()` - Temperature received
- `errorOccurred()` - Error occurred
- `initialized()` - Initialization complete

**Architecture:**
- Uses pigpio-based Python subprocess for GPIO control
- Spawns new process for each command (safe for Qt slots)
- Connects to persistent `pigpiod` daemon for GPIO state persistence

### 9. **GPIO Fan Controller Python Script**

```python
"""
@brief GPIO Fan Controller Helper Script for Raspberry Pi
@details Controls DC fan speed via GPIO + LM298 module
@details Uses pigpio daemon for persistent GPIO state
"""
```

**Functions:**
- `init_gpio(pin, mode, pwm_freq)` - @brief Initialize GPIO
- `set_digital(pin, value)` - @brief Set digital output
- `set_pwm(pin, percent)` - @brief Set PWM duty cycle
- `main()` - @brief Process command line arguments

**Commands:**
```bash
# Initialize GPIO17 as PWM at 25kHz
python3 gpio_fan_controller.py init 17 pwm 25000

# Set PWM to 50%
python3 gpio_fan_controller.py set_pwm 17 50

# Set digital ON
python3 gpio_fan_controller.py set_digital 17 1
```

## Data Flow

### Temperature-Based Fan Control

```
BMS → UART Frame
   ↓
BmsSerialReader (receive)
   ↓
BmsManager (parse via JbdProtocol)
   ↓
BmsSnapshot (update temperature)
   ↓
GpioFanController::updateTemperature()
   ↓
calculatePwmFromTemperature() [PWM mode]
   or
m_fanOn = temp > onTemp [Digital mode]
   ↓
executePythonCommand()
   ↓
gpio_fan_controller.py set_pwm/set_digital
   ↓
pigpiod daemon (GPIO hardware control)
   ↓
Raspberry Pi GPIO (physical PWM output)
```

## Compilation

```bash
# Build with CMake
mkdir build
cd build
cmake ..
cmake --build .

# Output: appBMSApp executable
```

## Usage

### On Windows/Linux (Host)

```bash
./appBMSApp
```

### On Raspberry Pi (Hardware)

**Prerequisites:**
```bash
sudo apt-get install pigpio python3-pigpio
sudo systemctl enable pigpiod
sudo systemctl start pigpiod
```

**Run Application:**
```bash
./appBMSApp
```

## Configuration

**Serial Connection:**
- Default: COM3 (Windows) or /dev/ttyUSB0 (Linux/RPi)
- Baud Rate: 9600 bps
- Data Bits: 8
- Stop Bits: 1
- Parity: None

**Fan Control:**
- GPIO Pin: 17 (can be changed)
- PWM Frequency: 25000 Hz (DC fan standard)
- PWM Mode: 30-50°C → 20-100% speed
- Digital Mode: >40°C ON, <35°C OFF (hysteresis)

## Error Handling

All errors are propagated through `errorCode` and `errorMessage` Q_PROPERTIES with corresponding signals.

### Error Codes:
- `InvalidConfig` - Invalid configuration
- `OpenFailed` - Serial port open failed
- `PortClosed` - Port closed unexpectedly
- `WriteFailed` - Write operation failed
- `Timeout` - No response from BMS
- `InvalidFrame` - Malformed frame
- `ChecksumMismatch` - Checksum verification failed
- `DeviceStatusError` - BMS device reported error
- `TransportError` - Low-level serial error

## Key Design Patterns

### 1. MVVM (Model-View-ViewModel)
- BmsManager = Model
- QML UI = View
- BmsViewModel = ViewModel (adapts model for QML)

### 2. Subprocess with Daemon
- GpioFanController spawns new Python process for each GPIO command
- Process connects to persistent pigpiod daemon
- Daemon maintains GPIO state across subprocess calls
- Avoids global variable issues with Qt's async nature

### 3. Qt Signal/Slot for Async Operations
- Serial communication is non-blocking
- Signals propagate state changes to QML
- Q_PROPERTY bindings keep UI synchronized

### 4. Factory Pattern (Protocol)
- JbdProtocol provides static methods for frame building and parsing
- No instantiation needed - pure utility class

## File Structure

```
BMSApp/
├── CMakeLists.txt              // Build configuration
├── main.cpp                    // Application entry point
├── Main.qml                    // Root QML window
├── src/
│   ├── backend/
│   │   ├── bmsmanager.h/cpp           // Core BMS engine
│   │   ├── bmsserialreader.h/cpp      // Serial port handler
│   │   ├── bmssnapshot.h              // Data structure
│   │   ├── jbdprotocol.h/cpp          // Protocol codec
│   │   ├── gpioFanController.h/cpp    // GPIO fan control
│   │   └── gpio_fan_controller.py     // Python GPIO helper
│   └── viewmodels/
│       └── bmsviewmodel.h/cpp         // MVVM adapter
├── qml/
│   ├── components/              // Reusable UI components
│   └── views/                   // Application views
└── assets/                      // Images, fonts, etc.
```

## Contact & Support

@author: haidinh-me
