################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CONTROL_ECU_functions.c \
../Final_Project_CONTROL_ECU.c \
../buzzer.c \
../dc-motor.c \
../eeprom.c \
../gpio.c \
../lcd.c \
../timer0.c \
../timer1.c \
../twi.c \
../uart.c 

OBJS += \
./CONTROL_ECU_functions.o \
./Final_Project_CONTROL_ECU.o \
./buzzer.o \
./dc-motor.o \
./eeprom.o \
./gpio.o \
./lcd.o \
./timer0.o \
./timer1.o \
./twi.o \
./uart.o 

C_DEPS += \
./CONTROL_ECU_functions.d \
./Final_Project_CONTROL_ECU.d \
./buzzer.d \
./dc-motor.d \
./eeprom.d \
./gpio.d \
./lcd.d \
./timer0.d \
./timer1.d \
./twi.d \
./uart.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


