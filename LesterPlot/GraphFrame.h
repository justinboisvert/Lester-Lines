#ifndef GRAPH_FRAME
#define GRAPH_FRAME
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <vector>
#include <math.h>
#include "mathplot.h"
#include <muParser.h>
#include <iomanip>
#include <string>
#include <sstream>
#include <iostream>
enum wxIDs {
    FUNC_INP,
    INTERV_INP,
    X_LABEL,
    SLOPE_LABEL,
    SPIN_CTRL
};
class GraphFrame : public wxFrame {
public:
    GraphFrame();
    ~GraphFrame();
    void calcTangentAt(double x);
    double calcLinePointAt(double m, double x, double y, double x_p);
    void fillNewEquation(const wxString &string);
    double evalFuncAt(double x);
    void spinClicked(wxCommandEvent& event);
    void enterPressed(wxCommandEvent& event);
    void intervalSet(wxCommandEvent& event);
    void updatePoint();
private:
    double intval_1,intval_2;
    wxPanel* mpanel;
    wxBoxSizer* vsizer;
    wxBoxSizer* graphtools;
    wxTextCtrl* functionctrl;
    wxStaticText* slope_val;
    wxTextCtrl* interval;
    mpWindow* window;
    mpFXYVector* graph;
    mpFXYVector* tangent;
    std::vector<double>* storagex;
    std::vector<double>* storagey;
    std::vector<double>* tangentx;
    std::vector<double>* tangenty;
    mu::Parser* parser;
    wxSpinCtrlDouble* spin_pos;
    
    
};
#endif