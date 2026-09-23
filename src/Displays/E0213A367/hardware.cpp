#include "E0213A367.h"

void E0213A367::reset() {
    if (!external_driver) {
        BaseDisplay::reset();
        return;
    }

    operation_timed_out = false;

    pinMode(pin_enable, OUTPUT);
    digitalWrite(pin_enable, enable_active);
    delay(100);

    pinMode(pin_reset, OUTPUT);
    digitalWrite(pin_reset, LOW);
    delay(10);
    digitalWrite(pin_reset, HIGH);
    delay(10);

    wait();
    if (operation_timed_out)
        return;

    sendCommand(0x12); // Software reset
    wait();
}

void E0213A367::wait() {
    if (!external_driver) {
        BaseDisplay::wait();
        return;
    }

    if (operation_timed_out)
        return;

    const uint32_t started_ms = millis();
    while (digitalRead(pin_busy) == HIGH) {
        if (millis() - started_ms >= busy_timeout_ms) {
            operation_timed_out = true;
            powerOff();
            return;
        }
        delay(1);
        yield();
    }
}

void E0213A367::powerOff() {
    if (external_driver)
        digitalWrite(pin_enable, !enable_active);
}

void E0213A367::sendImageData() {
    if (!operation_timed_out)
        BaseDisplay::sendImageData();
}

void E0213A367::sendBlankImageData() {
    if (!operation_timed_out)
        BaseDisplay::sendBlankImageData();
}

void E0213A367::endImageTxQuiet() {
    if (!operation_timed_out)
        BaseDisplay::endImageTxQuiet();
}

// Inform the display of selected memory area
void E0213A367::setMemoryArea(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey) {

    if (operation_timed_out)
        return;

    // Data entry mode - Left to Right, Top to Bottom
    sendCommand(0x11);
    sendData(0x03);

    // Select controller IC memory region to display a fullscreen image
    sendCommand(0x44); // Memory X start - end
    sendData(sx);
    sendData(ex);
    sendCommand(0x45); // Memory Y start - end
    sendData(sy);
    sendData(ey);

    // Place the cursor at the start of this memory region, ready to send image data x=0 y=0
    sendCommand(0x4E); // Memory cursor X
    sendData(sx);
    sendCommand(0x4F); // Memory cursor y
    sendData(sy);
}

void E0213A367::calculateMemoryArea( int16_t &sx, int16_t &sy, int16_t &ex, int16_t &ey,                        
                                            int16_t region_left, int16_t region_top, int16_t region_right, int16_t region_bottom ) {
    sx = region_left / 8;
    sy = region_top;
    ex = region_right / 8;
    ey = region_bottom;
}

void E0213A367::activate() {
    if (operation_timed_out)
        return;

    // Specify the update operation to run
    sendCommand(0x22);
    
    if ( fastmode_state == OFF )
        sendData(0xF7);
    else
        sendData(0xFF);

    // Execute the update
    sendCommand(0x20);

    // Block while the command runs
    wait();
}
