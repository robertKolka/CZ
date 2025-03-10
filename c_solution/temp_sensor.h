/* 
   This header file contains example structs and definitions
   for demonstrating how µC pins might be set for a given microcontroller.
   It will be usually provided by µC vendor libraries
*/

#define GPIO_IN  0
#define GPIO_OUT 1
#define I2C_SDA 2
#define I2C_SCL 2
#define ADC 2

#define GPIO_LOW 0
#define GPIO_HIGH 1

struct µC_pins {
    int pin1;
    int pin2;
    int pin3;
    int pin4;
    int pin5;
    int pin6;
};

struct set_GPIO {
    int pin1;
    int pin2;
    int pin3;
    int pin4;
    int pin5;
    int pin6;
};
