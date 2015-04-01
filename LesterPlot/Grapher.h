#ifndef GRAPHER_H
#define GRAPHER_H
#include "mathplot.h"
#include <wx/wx.h>
class Grapher : public mpWindow {
public:
    Grapher(wxPanel* parent);
private:
    mpScaleX* xax;
    mpScaleY* yax;
    
};
#endif