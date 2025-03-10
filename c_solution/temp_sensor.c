#include <stdio.h>
#include "temp_sensor.h"
#include <time.h>
#include <stdlib.h>

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

int read_ADC(){
    // It is assumed that reading of the internal ADC returns integers between 0 and 2^[ADC register size],
    // so e.g. 0 - 65535 for a 16-bit ADC
    int input_voltage_bits = 120;  // example value of voltage at ADC input
    int noise_range_bits = 5;    // example noise range for this ADC 
    int reading_bits = input_voltage_bits + rand() % (2*noise_range_bits + 1) - noise_range_bits;  // add random noise to the measured voltage
    return reading_bits;
}



void ISR_CPU_timer() {

}


int main() {
    printf("Running temp_sensor.c\n");

    // Initialization
    set_LED_pins();
    set_EEPROM_pins();
    set_ADC_pin();

    int a = read_ADC();
    

    return 0;
}