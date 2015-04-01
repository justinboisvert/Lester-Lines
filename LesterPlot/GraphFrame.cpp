#include "GraphFrame.h"

GraphFrame::GraphFrame() : wxFrame(NULL,wxID_ANY,wxT("Lester Lines"),wxDefaultPosition,wxSize(700,650)) {
    intval_1 = -10.00;
    intval_2 = 10.00;
    mpanel = new wxPanel(this);
    vsizer = new wxBoxSizer(wxVERTICAL);
    graphtools = new wxBoxSizer(wxHORIZONTAL);
    interval = new wxTextCtrl(mpanel, INTERV_INP, wxT("-10,10"),wxDefaultPosition,wxDefaultSize,wxTE_PROCESS_ENTER);
    functionctrl = new wxTextCtrl(mpanel,FUNC_INP,wxT("X^3"),wxDefaultPosition,wxDefaultSize,wxTE_PROCESS_ENTER);
    functionctrl->SetSizeHints(150,20);
    spin_pos = new wxSpinCtrlDouble(mpanel,SPIN_CTRL);
    spin_pos->SetSizeHints(50, 20);
    spin_pos->SetValue(0.00);
    spin_pos->SetIncrement(0.1);
    spin_pos->SetRange(-10.00, 10.00);
    slope_val = new wxStaticText(mpanel,SLOPE_LABEL,wxT("m: "));
    graphtools->Add(new wxStaticText(mpanel,wxID_ANY,wxT("f(x)")),wxTOP,5);
    graphtools->Add(functionctrl,0,wxTOP,0);
    graphtools->AddSpacer(4);
    graphtools->Add(spin_pos,0);
    graphtools->AddSpacer(4);
    graphtools->Add(interval,0);
    graphtools->AddSpacer(4);
    graphtools->Add(slope_val,0,wxTOP,0);
    mpanel->SetSizer(vsizer);
    storagex = new std::vector<double>();
    storagey = new std::vector<double>();
    tangentx = new std::vector<double>();
    tangenty = new std::vector<double>();
    window = new mpWindow(mpanel,wxID_ANY);
    window->EnableDoubleBuffer(true);
    mpScaleX* xax = new mpScaleX(wxT("x"),mpALIGN_CENTER,true);
    mpScaleY* yax = new mpScaleY(wxT("y"),mpALIGN_CENTER,true);
    window->AddLayer(xax);
    window->AddLayer(yax);
    graph = new mpFXYVector();
    tangent = new mpFXYVector();
    tangent->SetContinuity(true);
    tangent->SetPen(wxPen(*wxRED,1,wxSOLID));
    graph->SetContinuity(true);
    graph->SetPen(wxPen(*wxBLUE,1,wxSOLID));
    window->AddLayer(graph);
    window->AddLayer(tangent);
    
    vsizer->Add(window,4,wxEXPAND);
    vsizer->Add(graphtools,0);
    parser = new mu::Parser();
    //test out the use of inputted equation
    fillNewEquation(wxT("X^3"));
    //events
    Connect(SPIN_CTRL,wxEVT_COMMAND_SPINCTRLDOUBLE_UPDATED,wxCommandEventHandler(GraphFrame::spinClicked));
    Connect(FUNC_INP,wxEVT_COMMAND_TEXT_ENTER,wxCommandEventHandler(GraphFrame::enterPressed));
    Connect(INTERV_INP,wxEVT_COMMAND_TEXT_ENTER,wxCommandEventHandler(GraphFrame::intervalSet));
}

void GraphFrame::intervalSet(wxCommandEvent& event){
    std::istringstream f(interval->GetValue().ToStdString());
    std::string tok;
    int i = 0;
    while(std::getline(f, tok, ',')){
        if(i == 0){
            std::stringstream(tok) >> intval_1;
        }
        else {
            std::stringstream(tok) >> intval_2;
        }
        i++;
    }
    spin_pos->SetRange(intval_1,intval_2);
}
void GraphFrame::spinClicked(wxCommandEvent &event){
    window->DelLayer(tangent);
    calcTangentAt(spin_pos->GetValue());
    window->AddLayer(tangent);
    double m = (evalFuncAt(spin_pos->GetValue() + 0.0001) - evalFuncAt(spin_pos->GetValue())) / (0.0001);
    wxString s = wxString::Format("m: %.2lf",m);
    slope_val->SetLabel(s);
}

void GraphFrame::enterPressed(wxCommandEvent& event){
    fillNewEquation(functionctrl->GetValue());
}
/*
 m = f(x + h) - f(x) / h
 h = 0.001
 */
void GraphFrame::calcTangentAt(double x){
    tangentx->clear();
    tangenty->clear();
    
    //caclulate slope
    double m = ((evalFuncAt(x + 0.0001) - evalFuncAt(x))) / (0.0001);
    tangentx->push_back(intval_1 - 10.00);
    tangenty->push_back(calcLinePointAt(m, x, evalFuncAt(x), intval_1 - 10.00));
    tangentx->push_back(intval_2 + 10.00);
    tangenty->push_back(calcLinePointAt(m, x, evalFuncAt(x), intval_2 + 10.00));
    tangent->SetData(*tangentx,*tangenty);
    
}

double GraphFrame::calcLinePointAt(double m, double x, double y, double x_p){
    return m*(x_p - x) + y;
}


double GraphFrame::evalFuncAt(double x){
    parser->DefineVar("X",&x);
    try {
        return parser->Eval();
    }
    catch (mu::Parser::exception_type &e){
        slope_val->SetLabel(wxT("Error: syntax"));
    }
}
void GraphFrame::fillNewEquation(const wxString& string){
    storagex->clear();
    storagey->clear();
    window->DelLayer(tangent);
    window->DelLayer(graph);
    parser->SetExpr(string.ToStdString());
    for(double i = intval_1; i <= intval_2; i += 0.01){
        storagex->push_back(i);
        storagey->push_back(evalFuncAt(i));
    }
    graph->SetData(*storagex, *storagey);
    window->AddLayer(graph);
    window->Fit();
    calcTangentAt(spin_pos->GetValue());
    window->AddLayer(tangent);
}

GraphFrame::~GraphFrame(){
    delete storagex;
    delete storagey;
    delete tangentx;
    delete tangenty;
    delete parser;
}