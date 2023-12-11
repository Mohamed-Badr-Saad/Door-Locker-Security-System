# Door Locker Security System

## Project Overview

The Door Locker Security System is designed to unlock a door using a password. The system is implemented using two ATmega32 Microcontrollers operating at a frequency of 8MHz. The project follows a layered architecture model with two main components:

1. **HMI_ECU (Human Machine Interface):** Responsible for user interaction with a 2x16 LCD and a 4x4 keypad.

2. **Control_ECU:** Manages all processing and decisions, including password checking, door unlocking, and system alarm activation. It includes EEPROM, a Buzzer, and a DC-Motor.

## Project Context

This project was the graduation project from the MT diploma, an embedded systems diploma, under the supervision of Eng. Mohamed Tarek.

## System Requirements

### 1. Password Creation
- Display "Please Enter Password" on the LCD.
- Enter a password consisting of 5 numbers, displaying '*' for each digit.
- Press enter to confirm.
- Ask the user to re-enter the same password for confirmation.
- HMI_ECU sends the two passwords to Control_ECU through UART.
- If passwords match, save the password in EEPROM and proceed to the next step. Otherwise, repeat the process.

### 2. Main Options
- LCD displays the main system options.

### 3. Open Door
- Display "Please Enter Password" on the LCD.
- Enter the password and press enter.
- HMI_ECU sends the password to Control_ECU for verification.
- If passwords match:
  - Rotate the motor for 15 seconds clockwise and display "Door is Unlocking."
  - Hold the motor for 3 seconds.
  - Rotate the motor for 15 seconds anti-clockwise and display "Door is Locking."

### 4. Change Password
- Display "Please Enter Password" on the LCD.
- Enter the password and press enter.
- HMI_ECU sends the password to Control_ECU for verification.
- If passwords match, repeat the password creation process.

### 5. Failed Attempts Handling
- If passwords are unmatched in steps 3 or 4, ask the user to enter the password again.
- If passwords match after the third attempt, open the door or change the password.
- If passwords are not matched for the third consecutive time:
  - Activate the buzzer for 1 minute.
  - Display an error message on the LCD for 1 minute.
  - Lock the system; no inputs from the keypad will be accepted.
  - Return to Step 2 (Main Options).

## Driver Requirements

### GPIO Driver
- Use the same GPIO driver implemented in the course.
- The same driver should be used in both ECUs.

### LCD Driver
- Use a 2x16 LCD.
- Use the same LCD driver implemented in the course with 8-bits or 4-bits data mode.
- Connect the LCD control and data bus pins to any MCU pins.
- LCD should be connected to HMI_ECU.

### Keypad Driver
- Use a 4x4 Keypad.
- Connect the keypad pins to any MCU pins.
- Keypad should be connected to HMI_ECU.

### DC_Motor Driver
- Use the same DC_Motor driver implemented in the fan controller project.
- Motor should always run with the maximum speed using Timer0 PWM.
- Motor should be connected to Control_ECU.
- Connect the motor pins to any MCU pins.

### EEPROM Driver
- Use the same external EEPROM driver controlled by I2C.
- EEPROM should be connected to Control_ECU.

### I2C Driver
- Use the same I2C driver implemented in the course.
- I2C driver will be used in Control_ECU to communicate with the external EEPROM.
- Modify the `TWI_init` function to take a pointer to the configuration structure.

### UART Driver
- Use the same UART driver implemented in the course.
- The same driver should be used in both ECUs.
- Modify the `UART_init` function to take a pointer to the configuration structure.

### Timer Driver
- Use the same driver in both ECUs.
- Implement a full Timer driver for TIMER1 with the configuration technique.
- The Timer1 Driver should be designed using interrupts with the callback’s technique.
- The Timer1 Driver should support both normal and compare modes.

### Buzzer Driver
- Implement a full Buzzer driver.
- Buzzer should be connected to Control_ECU.
- Connect the Buzzer pin to any MCU pins.

