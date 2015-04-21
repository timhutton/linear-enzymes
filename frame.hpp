// local:
#include "Arena.hpp"

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include <wx/graphics.h>

class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString& title);

    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnIdle(wxIdleEvent& event);
    void OnSpeedStop(wxCommandEvent& WXUNUSED(event)) { this->render_every = 0; }
    void OnSpeedSlowest(wxCommandEvent& WXUNUSED(event)) { this->render_every = 1; }
    void OnSpeedMedium(wxCommandEvent& WXUNUSED(event)) { this->render_every = 100; }
    void OnSpeedFast(wxCommandEvent& WXUNUSED(event)) { this->render_every = 10000; }
    void OnStep(wxCommandEvent& WXUNUSED(event));

private:
    wxDECLARE_EVENT_TABLE();

    Arena arena;
    int iterations;
    int render_every;
    std::vector< wxBrush > brushes;

    void seed();
    void initBrushes();
    void draw( wxGraphicsContext *pGC, int X, int Y );
    void drawArena( wxGraphicsContext* pGC, int scale );
};

