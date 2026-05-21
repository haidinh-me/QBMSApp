#!/bin/bash
# Setup script for GPIO Fan Control on Raspberry Pi

echo "=== GPIO Fan Control Setup for Raspberry Pi ==="
echo ""

# Check Python3
if ! command -v python3 &> /dev/null; then
    echo "❌ Error: python3 not found"
    exit 1
fi
echo "✓ python3 found: $(python3 --version)"

# Try gpiozero
echo ""
echo "Checking GPIO backends..."
python3 -c "import gpiozero; print('✓ gpiozero available')" 2>/dev/null || echo "⚠ gpiozero not available"

# Try RPi.GPIO
python3 -c "import RPi.GPIO; print('✓ RPi.GPIO available')" 2>/dev/null || echo "⚠ RPi.GPIO not available"

# Check GPIO access
echo ""
echo "Checking GPIO permissions..."
if [ -w /sys/class/gpio/export ] 2>/dev/null; then
    echo "✓ GPIO access granted (direct)"
elif id -G | grep -q gpio; then
    echo "✓ GPIO access granted (via gpio group)"
else
    echo "⚠ Warning: May need elevated permissions for GPIO"
    echo "  Run with sudo or add user to gpio group:"
    echo "  sudo usermod -a -G gpio \$USER"
fi

echo ""
echo "Installation complete!"
echo ""
echo "To use GPIO Fan Control:"
echo "1. Call bmsManager->initializeFanControl(pin, pwmMode)"
echo "2. Set thresholds with setFanDigitalThresholds() or setFanPwmRange()"
echo "3. Temperature updates automatically from BMS"
echo ""
echo "For more info, see GPIO_FAN_CONTROL_GUIDE.md"
