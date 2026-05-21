/* 
 * Example: Using GPIO Fan Control with BmsManager
 * This shows how to integrate PWM fan control in your application
 */

#include "bmsmanager.h"

// Example 1: Auto PWM Control
void setupFanControlPWM(BmsManager* bmsManager)
{
    // Initialize fan control on GPIO17, PWM mode, 1000Hz frequency
    bool success = bmsManager->initializeFanControl(
        17,    // GPIO pin
        true,  // PWM mode
        1000   // Frequency (Hz)
    );

    if (!success) {
        qWarning() << "Failed to initialize fan control";
        return;
    }

    // Configure temperature range
    // 30°C = 20% speed, 50°C = 100% speed (linear interpolation)
    bmsManager->setFanPwmRange(
        30.0,   // minTemp
        50.0,   // maxTemp
        20,     // minPwm%
        100     // maxPwm%
    );

    qDebug() << "✓ Fan control initialized (PWM mode)";
    qDebug() << "  GPIO: 17, Range: 30-50°C, Speed: 20-100%";
    qDebug() << "  Temperature updates automatic from BMS";
}

// Example 2: Digital ON/OFF Control with Hysteresis
void setupFanControlDigital(BmsManager* bmsManager)
{
    // Initialize fan control on GPIO18, Digital mode
    bool success = bmsManager->initializeFanControl(
        18,    // GPIO pin
        false, // Digital mode (true=PWM)
        0      // Frequency not used in digital mode
    );

    if (!success) {
        qWarning() << "Failed to initialize fan control";
        return;
    }

    // Configure temperature thresholds with hysteresis
    // Turn ON at 40°C, OFF at 35°C
    bmsManager->setFanDigitalThresholds(
        40.0,   // onTemp
        35.0    // offTemp
    );

    qDebug() << "✓ Fan control initialized (Digital mode)";
    qDebug() << "  GPIO: 18, ON: 40°C, OFF: 35°C";
    qDebug() << "  Temperature updates automatic from BMS";
}

// Example 3: Manual Control (for testing/debugging)
void testFanControl(BmsManager* bmsManager)
{
    qDebug() << "Testing fan control...";

    // Test PWM mode
    for (int pwm = 0; pwm <= 100; pwm += 25) {
        bmsManager->setFanSpeed(pwm);
        qDebug() << "Set PWM to" << pwm << "%";
        QThread::msleep(1000);  // Wait 1 second
    }

    // Test Digital mode
    bmsManager->setFanState(true);
    qDebug() << "Fan ON";
    QThread::msleep(2000);

    bmsManager->setFanState(false);
    qDebug() << "Fan OFF";
}

// Example 4: Check initialization status
void checkFanStatus(BmsManager* bmsManager)
{
    if (bmsManager->isFanInitialized()) {
        qDebug() << "✓ Fan control is initialized and ready";
    } else {
        qDebug() << "✗ Fan control is not initialized";
        qDebug() << "  Call initializeFanControl() first";
    }
}

// Example 5: Integration in main application
class BatteryApp
{
public:
    BatteryApp()
        : m_bms(nullptr)
    {
    }

    void init()
    {
        // Create BMS manager
        m_bms = new BmsManager(this);

        // Connect to BMS (UART)
        m_bms->connectSerial("/dev/ttyAMA0", 9600);

        // Initialize fan control
        // PWM mode will auto-adjust fan speed based on BMS temperature
        if (m_bms->initializeFanControl(17, true, 1000)) {
            m_bms->setFanPwmRange(30.0, 50.0, 20, 100);
            qDebug() << "Fan control ready - will adjust automatically";
        }
    }

    void shutdown()
    {
        if (m_bms) {
            // Fan will be cleaned up automatically when BmsManager is destroyed
            m_bms->disconnectSerial();
            delete m_bms;
        }
    }

private:
    BmsManager* m_bms;
};

/*
 * USAGE FLOW:
 *
 * 1. Initialize BmsManager and connect to UART:
 *    BmsManager bms;
 *    bms.connectSerial("/dev/ttyAMA0", 9600);
 *
 * 2. Set up fan control (choose PWM or Digital):
 *    bms.initializeFanControl(17, true);  // PWM on GPIO17
 *    bms.setFanPwmRange(30, 50, 20, 100);
 *
 * 3. After this, no more action needed!
 *    - BMS sends temperature readings periodically
 *    - BmsManager::handleFrame() automatically calls:
 *      m_fanController->updateTemperature(snapshot.temperatureC)
 *    - Fan adjusts automatically based on current temperature
 *
 * FLOW DIAGRAM:
 *
 *    BMS Device
 *        ↓
 *    UART Serial Frame
 *        ↓
 *    BmsSerialReader::frameReceived()
 *        ↓
 *    BmsManager::handleFrame()
 *        ↓
 *    Parse JBD Protocol
 *        ↓
 *    Update snapshot.temperatureC
 *        ↓
 *    GpioFanController::updateTemperature(temp)
 *        ↓
 *    Calculate PWM or check Digital threshold
 *        ↓
 *    Execute Python Script
 *        ↓
 *    GPIO → LM298 → Fan
 *
 * NO QML/UI NEEDED - Pure backend logic!
 */
