#ifndef __DEPG1020BNS770F1_H__
#define __DEPG1020BNS770F1_H__

#include <Arduino.h>
#include "Displays/BaseDisplay/base.h"

// 10.2-inch, 960x640 monochrome panel driven by SSD1677.
class DEPG1020BNS770F1 : public BaseDisplay {
    private:
        static const uint16_t panel_width = 960;
        static const uint16_t panel_height = 640;
        static const Color supported_colors = (Color) (BLACK | WHITE);
        static const uint32_t busy_timeout_ms = 60000UL;

    public:
        DEPG1020BNS770F1(
            uint8_t pin_dc,
            uint8_t pin_cs,
            uint8_t pin_busy,
            uint8_t pin_reset,
            uint8_t pin_enable,
            bool enable_active,
            uint16_t page_height = DEFAULT_PAGE_HEIGHT)
            : BaseDisplay(pin_dc, pin_cs, pin_busy, DEFAULT_SDI, DEFAULT_CLK, page_height),
              pin_reset(pin_reset),
              pin_enable(pin_enable),
              enable_active(enable_active) {
                init();
            }

        #if CAN_MOVE_SPI_PINS
            DEPG1020BNS770F1(
                uint8_t pin_dc,
                uint8_t pin_cs,
                uint8_t pin_busy,
                uint8_t pin_reset,
                uint8_t pin_enable,
                bool enable_active,
                uint8_t pin_sdi,
                uint8_t pin_clk,
                uint16_t page_height = DEFAULT_PAGE_HEIGHT)
                : BaseDisplay(pin_dc, pin_cs, pin_busy, pin_sdi, pin_clk, page_height),
                  pin_reset(pin_reset),
                  pin_enable(pin_enable),
                  enable_active(enable_active) {
                    init();
                }
        #endif

        bool timedOut() const { return operation_timed_out; }

        // This panel is intentionally full-refresh only.
        void fastmodeOn(bool clear_if_reset = true) override {
            (void) clear_if_reset;
            fastmodeOff();
        }
        void fastmodeTurbo(bool clear_if_reset = true) override {
            (void) clear_if_reset;
            fastmodeOff();
        }

    private:
        PROGMEM static constexpr uint8_t lut_full[105] = {
            #include "lut_full.inc"
        };

        uint8_t pin_reset;
        uint8_t pin_enable;
        bool enable_active;
        bool operation_timed_out = false;

        void init();
        void powerOff();
        void reset() override;
        void wait() override;
        void configFull() override;
        void activate() override;
        void setMemoryArea(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey) override;
        void sendImageData() override;
        void sendBlankImageData() override;
        void endImageTxQuiet() override;
        void calculateMemoryArea(
            int16_t &sx,
            int16_t &sy,
            int16_t &ex,
            int16_t &ey,
            int16_t region_left,
            int16_t region_top,
            int16_t region_right,
            int16_t region_bottom) override;
};

#endif
