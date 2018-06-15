/**
* Jordan Electronics
* May, 2018
* Based on Weatherbit code from Sparkfun:
* https://github.com/sparkfun/pxt-weather-bit

* Development environment specifics:
* Written in Microsoft PXT
*
* This code is released under the [MIT License](http://opensource.org/licenses/MIT).
* Distributed as-is; no warranty is given.
*/

#include "pxt.h"
#include <cstdint>
#include <math.h>

using namespace pxt;

namespace DS1820 {

class microbitp : public MicroBitComponent
{
  public:
    void *pin;
    int id;
    int status;
    PinCapability capability;
    uint8_t pullMode;
    PinName name;

    microbitp(int id, PinName name, PinCapability capability){
        this->id = id;
        this->name = name;
        this->capability = capability;
        this->pullMode = 1;
        this->status = 0x00;
        this->pin = NULL;
    }

    void disconnect(){ 
        if (status & 0x01)
            delete ((DigitalIn *)pin);
        if (status & 0x02)
            delete ((DigitalOut *)pin);
        this->pin = NULL;
        this->status = 0;
    }

    int setDigitalValue(int value){
        // Move into a Digital input state if necessary.
        if (!(status & 0x02)){
            disconnect();
            pin = new DigitalOut(name);
            status |= 0x02;
        }
        // Write the value.
        ((DigitalOut *)pin)->write(value);
        return 0;
    }

    int getDigitalValue(){
        if (!(status & (0x01 | 0x20 | 0x40)))
        {
            ((DigitalIn *)pin)->mode(PullNone);
            status |= 0x01;
        }
        return ((DigitalIn *)pin)->read();
//          return 0;
    }
};
    
//    MicroBitPin pin1 = uBit.io.P1;
//    MicroBit uBit;
    MicroBitPin pin1 = uBit.io.P2;
//    microbitp  pin0(7, 3, 15);
//    microbitp  pin1(8, MICROBIT_PIN_P0, PIN_CAPABILITY_ALL);
//    microbitp  pin2(9, 1, 15);

    uint8_t init() {
        pin1.setDigitalValue(0);
        for (volatile uint16_t i = 0; i < 600; i++);
        pin1.setDigitalValue(1);
        for (volatile uint8_t i = 0; i < 30; i++);
        int b = pin1.getDigitalValue();
        for (volatile uint16_t i = 0; i < 600; i++);
        return b;
    }

    void sendZero() {
        pin1.setDigitalValue(0);
        for (volatile uint8_t i = 1; i < 75; i++);
        pin1.setDigitalValue(1);
        for (volatile uint8_t i = 1; i < 6; i++);
    }

    void sendOne() {
        pin1.setDigitalValue(0);
        for (volatile uint8_t i = 1; i < 1; i++);
        pin1.setDigitalValue(1);
        for (volatile uint8_t i = 1; i < 80; i++);
    }

    void writeBit(int b) {
        int delay1, delay2;
        if (b == 1) {
            delay1 = 1;
            delay2 = 80;
        } else {
            delay1 = 75;
            delay2 = 6;
        }
        pin1.setDigitalValue(0);
        for (uint8_t i = 1; i < delay1; i++);
        pin1.setDigitalValue(1);
        for (uint8_t i = 1; i < delay2; i++);
    }

    void sendskip() {
        writeBit(0);
        writeBit(0);
        writeBit(1);
        writeBit(1);
        writeBit(0);
        writeBit(0);
        writeBit(1);
        writeBit(1);
    }

    void writeByte(int byte) {
        int i;
        for (i = 0; i < 8; i++) {
            if (byte & 1) {
                writeBit(1);
            } else {
                writeBit(0);
            }
            byte = byte >> 1;
        }
    }

    int readBit() {
        volatile int i;
        pin1.setDigitalValue(0);
        pin1.setDigitalValue(1);
        for (i = 1; i < 20; i++);
        int b = pin1.getDigitalValue();
        for (i = 1; i < 60; i++);
        return b;
    }

    int convert() {
        volatile int i;
        int j;
        writeByte(0x44);
        for (j = 1; j < 1000; j++) {
            for (i = 1; i < 900; i++) {
        };
        if (readBit() == 1)
            break;
        };
        return (j);
    }

    int readByte() {
        int byte = 0;
        int i;
        for (i = 0; i < 8; i++) {
            byte = byte | readBit() << i;
        };
        return byte;
    }

    //%
    int16_t Temperature() {
      
        init();
        writeByte(0xCC);
        convert();
        init();
        writeByte(0xCC);
        writeByte(0xBE);
        int b1 = readByte();
        int b2 = readByte();

        int16_t temp = (b2 << 8 | b1);
        return temp * 100 / 16;
        
        return 1;
    }
 
}
