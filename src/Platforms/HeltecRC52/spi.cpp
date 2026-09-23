#include "Platforms/platforms.h"

#ifdef HELTEC_RC52

namespace Platform {

SPIClass* getSPI() {
    return &SPI1;
}

void beginSPI(SPIClass *spi, uint8_t pin_mosi, uint8_t pin_miso, uint8_t pin_clk) {
    spi->setPins(pin_miso, pin_clk, pin_mosi);
    spi->begin();
}

}

#endif
