#include "AppRegion.h"

IMPLEMENT_APP(AppRegion)

bool AppRegion::OnInit(){
    GraphFrame* frame = new GraphFrame();
    return frame->Show();
}