#!/usr/bin/env python3
"""
Raspberry Pi GPIO Fan Controller (pigpio version)

Usage:
    python3 gpio_fan_controller.py <GPIO_PIN> <COMMAND>

Commands:
    fan_on
    fan_off
    pwm:<0-100>

Example:
    python3 gpio_fan_controller.py 17 fan_on
    python3 gpio_fan_controller.py 17 pwm:60
"""

import sys
import time
import pigpio

PWM_FREQ = 25000  # 25kHz (chuẩn cho quạt DC / PC fan)

pi = pigpio.pi()

if not pi.connected:
    print("[ERROR] Cannot connect to pigpio daemon. Run: sudo systemctl start pigpiod")
    sys.exit(1)


def set_pwm(pin, duty_percent):
    """Set PWM duty cycle (0-100%)"""
    duty_percent = max(0, min(100, duty_percent))

    duty = int(duty_percent * 255 / 100)

    pi.set_mode(pin, pigpio.OUTPUT)
    pi.set_PWM_frequency(pin, PWM_FREQ)
    pi.set_PWM_dutycycle(pin, duty)

    print(f"[OK] GPIO {pin} PWM set to {duty_percent}% (freq={PWM_FREQ}Hz)")


def fan_on(pin):
    set_pwm(pin, 100)


def fan_off(pin):
    set_pwm(pin, 0)


def main():
    if len(sys.argv) < 3:
        print("Usage: python3 gpio_fan_controller.py <GPIO_PIN> <COMMAND>")
        print("Commands: fan_on | fan_off | pwm:<0-100>")
        sys.exit(1)

    pin = int(sys.argv[1])
    command = sys.argv[2]

    try:
        if command == "fan_on":
            fan_on(pin)

        elif command == "fan_off":
            fan_off(pin)

        elif command.startswith("pwm:"):
            duty = int(command.split(":")[1])
            if not 0 <= duty <= 100:
                raise ValueError("PWM must be 0-100")
            set_pwm(pin, duty)

        else:
            print(f"[ERROR] Unknown command: {command}")
            sys.exit(1)

        # Giữ process sống để PWM ổn định khi test
        time.sleep(0.2)

    except Exception as e:
        print(f"[ERROR] {e}")
        sys.exit(1)


if __name__ == "__main__":
    main()