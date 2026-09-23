#include "DEPG1020BNS770F1.h"

void DEPG1020BNS770F1::reset() {
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

void DEPG1020BNS770F1::wait() {
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

void DEPG1020BNS770F1::powerOff() {
    digitalWrite(pin_enable, !enable_active);
}

void DEPG1020BNS770F1::calculateMemoryArea(
    int16_t &sx,
    int16_t &sy,
    int16_t &ex,
    int16_t &ey,
    int16_t region_left,
    int16_t region_top,
    int16_t region_right,
    int16_t region_bottom) {
    sx = region_left;
    ex = region_right;
    sy = panel_height - 1 - region_top;
    ey = panel_height - 1 - region_bottom;
}

void DEPG1020BNS770F1::setMemoryArea(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey) {
    if (operation_timed_out)
        return;

    sendCommand(0x11); // X increases, Y decreases
    sendData(0x01);

    sendCommand(0x44);
    sendData(sx & 0xFF);
    sendData((sx >> 8) & 0xFF);
    sendData(ex & 0xFF);
    sendData((ex >> 8) & 0xFF);

    sendCommand(0x45);
    sendData(sy & 0xFF);
    sendData((sy >> 8) & 0xFF);
    sendData(ey & 0xFF);
    sendData((ey >> 8) & 0xFF);

    sendCommand(0x4E);
    sendData(sx & 0xFF);
    sendData((sx >> 8) & 0xFF);
    sendCommand(0x4F);
    sendData(sy & 0xFF);
    sendData((sy >> 8) & 0xFF);

    wait();
}

void DEPG1020BNS770F1::sendImageData() {
    if (operation_timed_out)
        return;

    sendCommand(0x24);
    for (uint32_t index = 0; index < pagefile_length; ++index)
        sendData(page_black[index]);
}

void DEPG1020BNS770F1::sendBlankImageData() {
    if (operation_timed_out)
        return;

    const uint8_t blank = (default_color & WHITE) ? 0xFF : 0x00;
    const uint32_t frame_bytes = (uint32_t) panel_width * panel_height / 8;
    sendCommand(0x24);
    for (uint32_t index = 0; index < frame_bytes; ++index)
        sendData(blank);
}

void DEPG1020BNS770F1::endImageTxQuiet() {
    // SSD1677 needs no end-of-transmission command before activation.
}

void DEPG1020BNS770F1::activate() {
    if (!operation_timed_out) {
        sendCommand(0x22);
        sendData(0xC7);
        sendCommand(0x20);
        wait();
    }

    if (!operation_timed_out) {
        sendCommand(0x10);
        sendData(0x01);
        delay(100);
    }

    powerOff();
}
