// local:
#include "app.hpp"
#include "frame.hpp"

// STL:
#include <cstdlib>
#include <ctime>
using namespace std;

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;

    srand(time(0));

    MyFrame *frame = new MyFrame("Linear Enzymes");
    frame->Show(true);
    return true;
}

