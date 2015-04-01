#include "Grapher.h"

Grapher::Grapher(wxPanel* parent) : mpWindow(parent,wxID_ANY){
    xax = new mpScaleX(wxT("x"),mpALIGN_CENTER,true);
    yax = new mpScaleY(wxT("y"),mpALIGN_CENTER, true);
    AddLayer(xax);
    AddLayer(yax);
}

