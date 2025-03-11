#include <iostream>
#include <Windows.h>
#include "temp_sensor.hpp"
using namespace std;

#define EEPROM_ADDRESS_HW_REV 0 // address in EEPROM where HW revision is stored
#define TEMPERATURE_THRESHOLD_NORMAL 85
#define TEMPERATURE_THRESHOLD_CRITICAL_LOW 5
#define TEMPERATURE_THRESHOLD_CRITICAL_HIGH 105

int degrees_per_digit = 0;   // Resolution of the temp sensor. Depends of the HW revision. 
int temperature = 0;         // Temperature read out from the sensor. Will be set by CPU timer 1 ISR.

class µC {
public:
    struct µC_pins µC_pins_setup;
    struct set_GPIO setGPIO_t;
    struct cpu_timer1 cpu_timer1_t;

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

        cpu_timer1_t.cpu_timer1_period = 0;
    }

    // µC methods
    unsigned int read_write_I2C(unsigned int slave_address, unsigned int RW) {
        // reads from / writes to the device with the given slave_address
        // the RW flag indicates write or read access
        return 0;
    };

    unsigned int read_ADC(){
        // It is assumed that reading of the internal ADC returns integers between 0 and 2^[ADC register size],
        // so e.g. 0 - 65535 for a 16-bit ADC
        unsigned int input_voltage_bits = 120;  // example value of voltage at ADC input
        unsigned int noise_range_bits = 5;    // example noise range for this ADC 
        unsigned int reading_bits = input_voltage_bits + rand() % (2*noise_range_bits + 1) - noise_range_bits;  // add random noise to the measured voltage
        return reading_bits;
    };
    

};

µC µC_temp_sensor1; // instance created globally so that the ISR knows it (cannot be passed as parameter into the ISR)


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
    µC_temp_sensor.cpu_timer1_t.cpu_timer1_period = 100;

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

unsigned int ADC_reading_to_temperature(int ADC_reading) {
    // it is assumed that the ADC measurement range covers only positive voltages (e.g. 0 - 3.3V)
    // and that the output voltage of the temperature sensor is positive
    unsigned int temp_out;

    temp_out = ADC_reading * degrees_per_digit;
    return temp_out;
}

void set_LEDs(µC µC_temp_sensor1) {
    int last_temperature = temperature; // create a copy so that it won't be changed by the ISR

    if ((last_temperature < TEMPERATURE_THRESHOLD_CRITICAL_LOW) || (last_temperature >= TEMPERATURE_THRESHOLD_CRITICAL_HIGH)) {
        set_red_LED(µC_temp_sensor1);
    }
    else {
        if (last_temperature < TEMPERATURE_THRESHOLD_NORMAL) {
            set_green_LED(µC_temp_sensor1);
        }
        else {
            set_yellow_LED(µC_temp_sensor1);
        }
    }
    
}

void set_degrees_per_digit(µC& µC_temp_sensor) {
    // sets degrees_per_digit based on the HW revision of the temp sensor
    int hw_revision_temp_sensor;
    hw_revision_temp_sensor = read_EEPROM_one_byte(µC_temp_sensor, EEPROM_ADDRESS_HW_REV); // sets the global variable 
    if (hw_revision_temp_sensor == 0) {
        degrees_per_digit = 1;
    }
    else if (hw_revision_temp_sensor == 1) {
        degrees_per_digit = 0.1;
    } 
    else {
        // error handling for unexpected EEPROM value, e.g. abort execution
    };
}

void ISR_CPU_timer1() {
    // ISR for CPU timer 1
    int ADC_reading;
    ADC_reading = µC_temp_sensor1.read_ADC();  // the instance of the µC class is hardcoded here, because it cannot be passed as a parameter to the ISR
    temperature = ADC_reading_to_temperature(ADC_reading); // sets the global variable 
}

int main() {

    cout << "Running temp_sensor.cpp" << endl;
    setup_µC_peripherals(µC_temp_sensor1);
    
    set_degrees_per_digit(µC_temp_sensor1);

    int i = 0;  // break condition for the while loop - just for debugging
    while(1) {
        set_LEDs(µC_temp_sensor1); 
        Sleep(50);  // Using Windows sleep function here. In real life application it must be replaced by the corresponding sleep function of the µC.

        i++;
        if (i > 10) {break;}  // don't run forever for the demo use case on PC
    }

    return 0;
}

