#include <iostream>
#include "temp_sensor.hpp"
using namespace std;

class µC {
public:
    struct µC_pins µC_pins_setup;
    struct set_GPIO setGPIO_t;

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

    
    // set pins 4 and 5 to I2C
    µC_temp_sensor.µC_pins_setup.pin6 = ADC;

}

int main() {

    cout << "Running temp_sensor.cpp" << endl;

    µC µC_temp_sensor1;
    setup_µC_peripherals(µC_temp_sensor1);

    return 0;
}

