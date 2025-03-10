#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "temp_sensor.h"

#define EEPROM_ADDRESS_HW_REV 0 // address in EEPROM where HW revision is stored
#define TEMPERATURE_THRESHOLD_NORMAL 85
#define TEMPERATURE_THRESHOLD_CRITICAL_LOW 5
#define TEMPERATURE_THRESHOLD_CRITICAL_HIGH 105

int hw_revision_temp_sensor; // Hardware revision of the temp sensor. Will be read out from EEPROM.
int degrees_per_digit = 0;   // Resolution of the temp sensor. Depends of the HW revision. 
int temperature = 0;         // Temperature read out from the sensor. Will be set by CPU timer 1 ISR.

struct µC_pins pinsUseCaseTemp;
struct set_GPIO setGPIO_t;

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

void set_CPU_timer1_µs(int µsecs){
    // sets the CPU timer 1 to interrupt every µsecs microseconds
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

unsigned int read_write_I2C(unsigned int slave_address, unsigned int RW) {
    // reads from / writes to the device with the given address slave_address
    // the RW flag indicates write or read access
    return 0;
}

unsigned int read_EEPROM_one_byte(unsigned int address) {
    // reads one byte from EEPROM at the given address
    unsigned int eeprom_reading;
    eeprom_reading = read_write_I2C(address, 1);
    return eeprom_reading; // just an example value for Rev-A of the temp sensor
}

unsigned int read_ADC(){
    // It is assumed that reading of the internal ADC returns integers between 0 and 2^[ADC register size],
    // so e.g. 0 - 65535 for a 16-bit ADC
    unsigned int input_voltage_bits = 120;  // example value of voltage at ADC input
    unsigned int noise_range_bits = 5;    // example noise range for this ADC 
    unsigned int reading_bits = input_voltage_bits + rand() % (2*noise_range_bits + 1) - noise_range_bits;  // add random noise to the measured voltage
    return reading_bits;
}

unsigned int ADC_reading_to_temperature(int ADC_reading) {
    // it is assumed that the ADC measurement range covers only positive voltages (e.g. 0 - 3.3V)
    // and that the output voltage of the temperature sensor is positive
    unsigned int temp_out;

    if (hw_revision_temp_sensor == 0) {
        degrees_per_digit = 1;
    }
    else if (hw_revision_temp_sensor == 1) {
        degrees_per_digit = 0.1;
    } 
    else {
        // error handling for unexpected EEPROM value, e.g. abort execution
    };

    temp_out = ADC_reading * degrees_per_digit;
    return temp_out;
}

void set_LEDs() {
    int last_temperature = temperature; // create a copy so that it won't be changed by the ISR

    if last_temperature < 
}


void ISR_CPU_timer1() {
    // ISR for CPU timer 1
    int ADC_reading;
    ADC_reading = read_ADC();
    temperature = ADC_reading_to_temperature(ADC_reading); // sets the global variable 
}


int main() {
    printf("Running temp_sensor.c\n");

    // Peripheral intializations
    set_LED_pins();
    set_EEPROM_pins();
    set_ADC_pin();
    set_CPU_timer1_µs(100);

    hw_revision_temp_sensor = read_EEPROM_one_byte(EEPROM_ADDRESS_HW_REV); // sets the global variable 

    int i = 0;  // break condition for the while loop - just for debugging
    while(1) {
        set_LEDs(); 
        Sleep(50);  // Using Windows sleep function here. In real life application it must be replaced by the corresponding sleep function of the µC.

        i++;
        if (i > 10) {break;}  // for debugging on PC 
    }

    return 0;
}