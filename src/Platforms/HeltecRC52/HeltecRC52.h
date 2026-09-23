// Specific options for the Heltec RadioCore RC52.

#ifndef __HELTEC_RC52_PLATFORM_H__
#define __HELTEC_RC52_PLATFORM_H__

#if defined(HELTEC_RC52) && !defined(PLATFORM_SUPPORTED)

#include <Arduino.h>
#include <SPI.h>
#include <avr/dtostrf.h> // nRF52 core declaration used by BaseDisplay::printCenter()

#define PLATFORM_SUPPORTED

#define CAN_MOVE_SPI_PINS       true
#define ALL_IN_ONE              false
#define DEFAULT_SDI             PIN_SPI1_MOSI
#define DEFAULT_CLK             PIN_SPI1_SCK
#define DEFAULT_MISO            PIN_SPI1_MISO

#define DEFAULT_PAGE_HEIGHT     32
#define MAX_PAGE_HEIGHT         panel_height
#define PRESERVE_IMAGE          true

namespace Platform {
    extern SPIClass* getSPI();
    extern void beginSPI(SPIClass *spi, uint8_t pin_mosi, uint8_t pin_miso, uint8_t pin_clk);
}

#endif

#endif
