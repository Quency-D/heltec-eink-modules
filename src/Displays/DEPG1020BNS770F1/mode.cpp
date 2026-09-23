#include "DEPG1020BNS770F1.h"

PROGMEM constexpr uint8_t DEPG1020BNS770F1::lut_full[];

void DEPG1020BNS770F1::configFull() {
    if (operation_timed_out)
        return;

    sendCommand(0x0C); // Booster soft-start
    sendData(0xAE);
    sendData(0xC7);
    sendData(0xC3);
    sendData(0xC0);
    sendData(0x80);

    sendCommand(0x01); // 640 gate outputs
    sendData(0x7F);
    sendData(0x02);
    sendData(0x01); // Gate scan setting from the supplied panel demo

    sendCommand(0x3C); // Border waveform
    sendData(0x01);

    sendCommand(0x18); // Internal temperature sensor
    sendData(0x80);

    // Voltage values supplied with the panel demonstration package.
    sendCommand(0x2C); // VCOM
    sendData(0x50);
    sendCommand(0x03); // Gate voltage
    sendData(0x17);
    sendCommand(0x04); // Source voltages
    sendData(0x41);
    sendData(0xBA);
    sendData(0x32);

    sendCommand(0x32);
    for (uint16_t index = 0; index < sizeof(lut_full); ++index)
        sendData(pgm_read_byte_near(lut_full + index));

    wait();
}
