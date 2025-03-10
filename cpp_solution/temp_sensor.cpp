#include <iostream>
#include "temp_sensor.hpp"
using namespace std;

#define EEPROM_ADDRESS_HW_REV 0 // address in EEPROM where HW revision is stored
#define TEMPERATURE_THRESHOLD_NORMAL 85
#define TEMPERATURE_THRESHOLD_CRITICAL_LOW 5
#define TEMPERATURE_THRESHOLD_CRITICAL_HIGH 105

int hw_revision_temp_sensor; // Hardware revision of the temp sensor. Will be read out from EEPROM.
int degrees_per_digit = 0;   // Resolution of the temp sensor. Depends of the HW revision. 
int temperature = 0;         // Temperature read out from the sensor. Will be set by CPU timer 1 ISR.

class µC {
public:
    struct µC_pins µC_pins_setup;
    struct set_GPIO setGPIO_t;
    struct cpu1_timer cpu1_timer_t;

    // default constructor
    µC() {
        µC_pins_setup.pin1 = 0;
        µC_pins_setup.pin2 = 0;
        µC_pins_setup.pin3 = 0;
        µC_pins_setup.pin4 = 0;
        µC_pins_setup.pin5 = 0;
        µC_pins_setup.pin6 = 0;

        setGPIO_t.pin1 = 0;
        setGPIO_t.pin2 = 0;
        setGPIO_t.pin3 = 0;
        setGPIO_t.pin4 = 0;
        setGPIO_t.pin5 = 0;
        setGPIO_t.pin6 = 0;

        cpu1_timer_t.cpu1_timer_period = 0;
    }

    // µC methods
    unsigned int read_write_I2C(unsigned int slave_address, unsigned int RW) {
        // reads from / writes to the device with the given address slave_address
        // the RW flag indicates write or read access
        return 0;
    }

};



void setup_µC_peripherals(µC& µC_temp_sensor)
{
    // set pins 1, 2 and 3 to GPIO
    µC_temp_sensor.µC_pins_setup.pin1 = GPIO_OUT; // red
    µC_temp_sensor.µC_pins_setup.pin2 = GPIO_OUT; // yellow
    µC_temp_sensor.µC_pins_setup.pin3 = GPIO_OUT; // green
    
    // set pins 4 and 5 to I2C
    µC_temp_sensor.µC_pins_setup.pin4 = I2C_SCL;
    µC_temp_sensor.µC_pins_setup.pin5 = I2C_SDA;
    
    // set pin 6 to ADC
    µC_temp_sensor.µC_pins_setup.pin6 = ADC;

    // set cpu1 timer period to 100 µs
    µC_temp_sensor.cpu1_timer_t.cpu1_timer_period = 100;

}

void set_red_LED(µC& µC_temp_sensor) {
    // red LED should turn on, others should turn off
    µC_temp_sensor.setGPIO_t.pin1 = GPIO_LOW;
    µC_temp_sensor.setGPIO_t.pin2 = GPIO_HIGH;
    µC_temp_sensor.setGPIO_t.pin3 = GPIO_HIGH;
}
void set_yellow_LED(µC& µC_temp_sensor) {
    // yellow LED should turn on, others should turn off
    µC_temp_sensor.setGPIO_t.pin1 = GPIO_HIGH;
    µC_temp_sensor.setGPIO_t.pin2 = GPIO_LOW;
    µC_temp_sensor.setGPIO_t.pin3 = GPIO_HIGH;
}
void set_green_LED(µC& µC_temp_sensor) {
    // green LED should turn on, others should turn off
    µC_temp_sensor.setGPIO_t.pin1 = GPIO_HIGH;
    µC_temp_sensor.setGPIO_t.pin2 = GPIO_HIGH;
    µC_temp_sensor.setGPIO_t.pin3 = GPIO_LOW;
}

unsigned int read_EEPROM_one_byte(µC& µC_temp_sensor, unsigned int address) {
    // reads one byte from EEPROM at the given address
    unsigned int eeprom_reading;
    eeprom_reading = µC_temp_sensor.read_write_I2C(address, 1);
    return eeprom_reading; // just an example value for Rev-A of the temp sensor
}


int main() {

    cout << "Running temp_sensor.cpp" << endl;

    µC µC_temp_sensor1;
    setup_µC_peripherals(µC_temp_sensor1);
    
    hw_revision_temp_sensor = read_EEPROM_one_byte(µC_temp_sensor1, EEPROM_ADDRESS_HW_REV); // sets the global variable 

    return 0;
}

