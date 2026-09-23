#include "DEPG1020BNS770F1.h"

void DEPG1020BNS770F1::init() {
    BaseDisplay::panel_width = this->panel_width;
    BaseDisplay::panel_height = this->panel_height;
    BaseDisplay::drawing_width = this->panel_width;
    BaseDisplay::drawing_height = this->panel_height;
    BaseDisplay::supported_colors = this->supported_colors;

    BaseDisplay::instantiateBounds();
    BaseDisplay::initDrawingParams();
}
