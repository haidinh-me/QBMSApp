#!/usr/bin/env python3
"""@brief GPIO Fan Controller for Raspberry Pi - Controls DC fan via GPIO PWM/Digital mode"""

import sys
import pigpio

def init_gpio(pin, mode, pwm_freq):
    """@brief Initialize GPIO pin for PWM or Digital mode"""
    try:
        pi = pigpio.pi()
        if not pi.connected:
            print("[GPIO Error] Cannot connect to pigpio daemon", file=sys.stderr)
            print("[GPIO Error] Run: sudo systemctl start pigpiod", file=sys.stderr)
            return False
        
        if mode == "pwm":
            pi.set_mode(pin, pigpio.OUTPUT)
            pi.set_PWM_frequency(pin, pwm_freq)
            pi.set_PWM_dutycycle(pin, 0)
            print(f"[GPIO] Initialized: pin={pin}, mode=pwm, freq={pwm_freq}Hz")
        else:
            pi.set_mode(pin, pigpio.OUTPUT)
            pi.write(pin, 0)
            print(f"[GPIO] Initialized: pin={pin}, mode=digital")
        
        return True
    except Exception as e:
        print(f"[GPIO Error] {e}", file=sys.stderr)
        return False


def set_digital(pin, value):
    """@brief Set GPIO digital output (ON/OFF)"""
    try:
        pi = pigpio.pi()
        if not pi.connected:
            print("[GPIO Error] Cannot connect to pigpio daemon", file=sys.stderr)
            return False
        
        pi.write(pin, 1 if value else 0)
        print(f"[GPIO] set_digital: pin={pin}, value={value}")
        return True
    except Exception as e:
        print(f"[GPIO Error] {e}", file=sys.stderr)
        return False


def set_pwm(pin, percent):
    """@brief Set PWM duty cycle (0-100%)"""
    try:
        pi = pigpio.pi()
        if not pi.connected:
            print("[GPIO Error] Cannot connect to pigpio daemon", file=sys.stderr)
            return False
        
        percent = max(0, min(100, percent))
        duty = int(percent * 255 / 100)
        pi.set_PWM_dutycycle(pin, duty)
        print(f"[GPIO] set_pwm: pin={pin}, duty={percent}%")
        return True
    except Exception as e:
        print(f"[GPIO Error] {e}", file=sys.stderr)
        return False


def main():
    """@brief Main entry point - process command line arguments"""
    if len(sys.argv) < 2:
        print("Usage:")
        print("  gpio_fan_controller.py init <pin> <mode> <pwm_freq>")
        print("    mode: 'pwm' or 'digital'")
        print("  gpio_fan_controller.py set_digital <pin> <value>")
        print("    value: 0 or 1")
        print("  gpio_fan_controller.py set_pwm <pin> <percent>")
        print("    percent: 0-100")
        sys.exit(1)

    command = sys.argv[1]

    try:
        if command == "init":
            if len(sys.argv) < 5:
                print("Error: init requires pin, mode, pwm_freq", file=sys.stderr)
                sys.exit(1)

            pin = int(sys.argv[2])
            mode = sys.argv[3]
            pwm_freq = int(sys.argv[4])

            if not init_gpio(pin, mode, pwm_freq):
                sys.exit(1)

        elif command == "set_digital":
            if len(sys.argv) < 4:
                print("Error: set_digital requires pin and value", file=sys.stderr)
                sys.exit(1)

            pin = int(sys.argv[2])
            value = int(sys.argv[3])
            
            if not set_digital(pin, value):
                sys.exit(1)

        elif command == "set_pwm":
            if len(sys.argv) < 4:
                print("Error: set_pwm requires pin and percent", file=sys.stderr)
                sys.exit(1)

            pin = int(sys.argv[2])
            percent = int(sys.argv[3])
            
            if not set_pwm(pin, percent):
                sys.exit(1)

        else:
            print(f"Error: unknown command '{command}'", file=sys.stderr)
            sys.exit(1)

    except Exception as e:
        print(f"[GPIO Error] {e}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()