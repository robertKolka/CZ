#include <stdio.h>
#include "temp_sensor.h"


void set_LED_pins() {
    // set pins 1, 2 and 3 to GPIO
    pinsUseCaseTemp.pin1 = GPIO_OUT; // red
    pinsUseCaseTemp.pin2 = GPIO_OUT; // yellow
    pinsUseCaseTemp.pin3 = GPIO_OUT; // green
}

void set_EEPROM_pins() {
    // set pins 4 and 5 to I2C
    pinsUseCaseTemp.pin4 = I2C_SCL;
    pinsUseCaseTemp.pin5 = I2C_SDA;
}

void set_ADC_pin() {
    // set pins 4 and 5 to I2C
    pinsUseCaseTemp.pin6 = ADC;
}

void set_red_LED() {
    // red LED should turn on, others should turn off
    setGPIO_t.pin1 = GPIO_LOW;
    setGPIO_t.pin2 = GPIO_HIGH;
    setGPIO_t.pin3 = GPIO_HIGH;
}
void set_yellow_LED() {
    // yellow LED should turn on, others should turn off
    setGPIO_t.pin1 = GPIO_HIGH;
    setGPIO_t.pin2 = GPIO_LOW;
    setGPIO_t.pin3 = GPIO_HIGH;
}
void set_green_LED() {
    // green LED should turn on, others should turn off
    setGPIO_t.pin1 = GPIO_HIGH;
    setGPIO_t.pin2 = GPIO_HIGH;
    setGPIO_t.pin3 = GPIO_LOW;
}

int main() {
    printf("Running temp_sensor.c");
    set_LED_pins();
    set_EEPROM_pins();
    set_ADC_pin();

    return 0;
}