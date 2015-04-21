// local:
#include "frame.hpp"

// wxWidgets:
#include <wx/dcbuffer.h>

// STL:
using namespace std;

namespace ID
{
    enum
    {
        // we can use IDs higher than this for our own purposes
        Dummy = wxID_HIGHEST+1,

        SpeedStop,
        SpeedSlowest,
        SpeedMedium,
        SpeedFast,
        Step,
    };
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(wxID_EXIT,  MyFrame::OnQuit)
    EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
    EVT_MENU(ID::SpeedStop, MyFrame::OnSpeedStop)
    EVT_MENU(ID::SpeedSlowest, MyFrame::OnSpeedSlowest)
    EVT_MENU(ID::SpeedMedium, MyFrame::OnSpeedMedium)
    EVT_MENU(ID::SpeedFast, MyFrame::OnSpeedFast)
    EVT_MENU(ID::Step, MyFrame::OnStep)
    EVT_PAINT(MyFrame::OnPaint)
    EVT_SIZE(MyFrame::OnSize)
    EVT_IDLE(MyFrame::OnIdle)
wxEND_EVENT_TABLE()

//-------------------------------------------------------------------------------------

MyFrame::MyFrame(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(900,700) )
       , arena( 80, 60 )
       , iterations( 0 )
       , render_every( 1 )
{
    SetIcon(wxICON(sample));

#if wxUSE_MENUS
    wxMenu *fileMenu = new wxMenu;
    fileMenu->Append(wxID_EXIT, "E&xit\tAlt-X", "Quit this program");

    wxMenu *actionMenu = new wxMenu;
    actionMenu->Append(ID::SpeedStop, "Stop\t0", "Stop running");
    actionMenu->Append(ID::SpeedSlowest, "Run at slow speed\t1", "Run at the slowest speed");
    actionMenu->Append(ID::SpeedMedium, "Run at medium speed\t2", "Run at a medium speed");
    actionMenu->Append(ID::SpeedFast, "Run at fast speed\t3", "Run at a fast speed");
    actionMenu->Append(ID::Step, "Step\tSPACE", "Advance forwards by a single step");

    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(wxID_ABOUT, "&About\tF1", "Show about dialog");

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(actionMenu, "&Action");
    menuBar->Append(helpMenu, "&Help");

    SetMenuBar(menuBar);
#endif // wxUSE_MENUS

    SetBackgroundStyle( wxBackgroundStyle::wxBG_STYLE_PAINT );

    seed();
    initBrushes();

    this->Maximize();
}

//-------------------------------------------------------------------------------------

void MyFrame::seed() {

    Neighborhood bond_range = Neighborhood::Moore;
    try {
        // an 8-cell loop with some rigid sections
        if( 0 ) {
            size_t a = arena.addAtom( 1, 1, 0 );
            size_t b = arena.addAtom( 2, 1, 0 );
            size_t c = arena.addAtom( 2, 2, 0 );
            size_t d = arena.addAtom( 1, 2, 0 );
            size_t e = arena.addAtom( 1, 3, 0 );
            size_t f = arena.addAtom( 0, 3, 0 );
            size_t g = arena.addAtom( 0, 2, 0 );
            size_t h = arena.addAtom( 0, 1, 0 );
            arena.makeBond( a, b, Neighborhood::vonNeumann );
            arena.makeBond( b, c, Neighborhood::vonNeumann );
            arena.makeBond( c, d, Neighborhood::Moore );
            arena.makeBond( d, e, Neighborhood::Moore );
            arena.makeBond( e, f, Neighborhood::vonNeumann );
            arena.makeBond( f, g, Neighborhood::Moore );
            arena.makeBond( g, h, Neighborhood::vonNeumann );
            arena.makeBond( h, a, Neighborhood::Moore );
        }
    
        // a box with flailing arms
        if( 0 ) {
            size_t a = arena.addAtom( 10, 10, 1 );
            size_t b = arena.addAtom( 11, 10, 1 );
            size_t c = arena.addAtom( 12, 10, 1 );
            size_t d = arena.addAtom( 12, 11, 1 );
            size_t e = arena.addAtom( 11, 11, 1 );
            size_t f = arena.addAtom( 10, 11, 1 );
            size_t g = arena.addAtom( 10, 12, 1 );
            size_t h = arena.addAtom( 11, 12, 1 );
            size_t i = arena.addAtom( 12, 12, 1 );
            size_t j = arena.addAtom( 9, 9, 1 );
            size_t k = arena.addAtom( 8, 8, 1 );
            size_t l = arena.addAtom( 7, 7, 1 );
            size_t m = arena.addAtom( 11, 9, 1 );
            size_t n = arena.addAtom( 12, 8, 1 );
            size_t o = arena.addAtom( 13, 7, 1 );
            arena.makeBond( a, b, Neighborhood::vonNeumann );
            arena.makeBond( b, c, Neighborhood::vonNeumann );
            arena.makeBond( c, d, Neighborhood::vonNeumann );
            arena.makeBond( d, e, Neighborhood::vonNeumann );
            arena.makeBond( e, f, Neighborhood::vonNeumann );
            arena.makeBond( f, g, Neighborhood::vonNeumann );
            arena.makeBond( g, h, Neighborhood::vonNeumann );
            arena.makeBond( h, i, Neighborhood::vonNeumann );
            arena.makeBond( a, j, Neighborhood::Moore );
            arena.makeBond( j, k, Neighborhood::Moore );
            arena.makeBond( k, l, Neighborhood::Moore );
            arena.makeBond( c, m, Neighborhood::Moore );
            arena.makeBond( m, n, Neighborhood::Moore );
            arena.makeBond( n, o, Neighborhood::Moore );
        }

        // a double-stranded molecule
        if( 0 ) {
            size_t a = arena.addAtom( 21, 21, 5 );
            size_t b = arena.addAtom( 22, 21, 3 );
            size_t c = arena.addAtom( 21, 22, 5 );
            size_t d = arena.addAtom( 22, 22, 3 );
            size_t e = arena.addAtom( 21, 23, 5 );
            size_t f = arena.addAtom( 22, 23, 3 );
            size_t g = arena.addAtom( 21, 24, 5 );
            size_t h = arena.addAtom( 22, 24, 3 );
            size_t i = arena.addAtom( 21, 25, 5 );
            size_t j = arena.addAtom( 22, 25, 3 );
            size_t k = arena.addAtom( 21, 26, 5 );
            size_t l = arena.addAtom( 22, 26, 3 );
            arena.makeBond( a, b, bond_range );
            arena.makeBond( c, d, bond_range );
            arena.makeBond( e, f, bond_range );
            arena.makeBond( g, h, bond_range );
            arena.makeBond( i, j, bond_range );
            arena.makeBond( k, l, bond_range );
            arena.makeBond( a, c, bond_range );
            arena.makeBond( b, d, bond_range );
            arena.makeBond( c, e, bond_range );
            arena.makeBond( d, f, bond_range );
            arena.makeBond( e, g, bond_range );
            arena.makeBond( f, h, bond_range );
            arena.makeBond( g, i, bond_range );
            arena.makeBond( h, j, bond_range );
            arena.makeBond( i, k, bond_range );
            arena.makeBond( j, l, bond_range );
        }

        if( 1 ) { 
            // some enzymes
            vector<string> vs;
            vs.push_back( Reaction( 3, false, 4, 8, true, 5 ).getString() );
            vs.push_back( Reaction( 5, false, 4, 11, true, 10 ).getString() );
            vs.push_back( Reaction( 11, true, 6, 6, true, 11 ).getString() );
            vs.push_back( Reaction( 11, true, 7, 7, true, 11 ).getString() );
            vs.push_back( Reaction( 11, true, 8, 8, true, 11 ).getString() );
            vs.push_back( Reaction( 0, true, 11, 6, true, 12 ).getString() );
            vs.push_back( Reaction( 1, true, 11, 7, true, 13 ).getString() );
            vs.push_back( Reaction( 12, true, 6, 6, true, 12 ).getString() );
            vs.push_back( Reaction( 12, true, 7, 7, true, 12 ).getString() );
            vs.push_back( Reaction( 12, true, 8, 8, true, 12 ).getString() );
            vs.push_back( Reaction( 13, true, 6, 6, true, 13 ).getString() );
            vs.push_back( Reaction( 13, true, 7, 7, true, 13 ).getString() );
            vs.push_back( Reaction( 13, true, 8, 8, true, 13 ).getString() );
            vs.push_back( "210103" );
            vs.push_back( Reaction( 12, true, 10, 6, true, 5 ).getString() );
            vs.push_back( Reaction( 13, true, 10, 7, true, 5 ).getString() );
            vs.push_back( Reaction( 2, true, 11, 2, true, 14 ).getString() );
            vs.push_back( Reaction( 14, true, 6, 0, true, 14 ).getString() );
            vs.push_back( Reaction( 14, true, 7, 1, true, 14 ).getString() );
            vs.push_back( Reaction( 14, true, 8, 3, false, 15 ).getString() );
            vs.push_back( Reaction( 15, false, 4, 14, true, 3 ).getString() );
            vs.push_back( Reaction( 14, true, 10, 2, false, 9 ).getString() );
            int x = 11;
            for( const string& s : vs ) {
                const int N = s.length();
                int y = 10;
                size_t a = arena.addAtom( x, y, s[0] - '0' );
                for( int i = 1; i < N; ++i ) {
                    y++;
                    size_t a2 = arena.addAtom( x, y, s[i] - '0' );
                    arena.makeBond( a, a2, bond_range );
                    a = a2;
                }
                x += 2;
            }
        }
    
        if( 1 ) {
            // add some surrounding atoms
            for( int i = 0; i < 500; ++i ) {
                int x = rand() % this->arena.getArenaWidth();
                int y = rand() % this->arena.getArenaHeight();
                if( !this->arena.hasAtom( x, y ) )
                    this->arena.addAtom( x, y, 4 );
            }
        }
    }
    catch( exception& e ) {
        wxMessageBox( e.what() );
    }
}

//-------------------------------------------------------------------------------------

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

//-------------------------------------------------------------------------------------

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox("Linear Enzymes",
                 "About Linear Enzymes",
                 wxOK | wxICON_INFORMATION,
                 this);
}

//-------------------------------------------------------------------------------------

void MyFrame::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    int X = this->GetClientSize().x; 
    int Y = this->GetClientSize().y; 
    if( X < 0 || Y < 0 )
        return;

    wxAutoBufferedPaintDC dc( this );        
    wxGraphicsContext* pGC = wxGraphicsContext::Create( dc );
    if( pGC ) {
        draw( pGC, X, Y );
    }
}

//-------------------------------------------------------------------------------------

void MyFrame::draw( wxGraphicsContext *pGC, int X, int Y ) {
    int scale = min( (X-1) / this->arena.getArenaWidth(), (Y-1) / this->arena.getArenaHeight() );

    // blank the area
    pGC->SetPen(*wxLIGHT_GREY_PEN);
    pGC->SetBrush(*wxLIGHT_GREY_BRUSH);
    pGC->DrawRectangle( 0, 0, X, Y );

    // draw the arena
    pGC->SetPen(*wxBLACK_PEN);
    pGC->SetBrush(*wxWHITE_BRUSH);
    pGC->DrawRectangle( 0, 0, this->arena.getArenaWidth()*scale, this->arena.getArenaHeight()*scale );
    drawArena( pGC, scale );

    delete pGC;
}

//-------------------------------------------------------------------------------------

void MyFrame::drawArena( wxGraphicsContext* pGC, int scale ) {

    // draw atoms
    pGC->SetPen(*wxMEDIUM_GREY_PEN);
    pGC->SetBrush(*wxLIGHT_GREY_BRUSH);
    pGC->SetFont(*wxNORMAL_FONT,*wxBLACK);
    for( size_t iAtom = 0; iAtom < this->arena.getNumberOfAtoms(); ++iAtom ) {
        Atom a = this->arena.getAtom( iAtom );
        pGC->SetBrush( brushes[ a.state ] ) ;
        pGC->DrawRectangle( a.x * scale, a.y * scale, scale, scale );
        pGC->DrawText( wxString::Format("%d",a.state), a.x * scale, a.y * scale );
    }

    // draw bonds
    wxPen thinBondPen(*wxBLACK,1);
    wxPen thickBondPen(*wxBLACK,2);
    for( size_t iAtom = 0; iAtom < this->arena.getNumberOfAtoms(); ++iAtom ) {
        Atom a = this->arena.getAtom( iAtom );
        for( const Bond& bond : a.bonds ) {
            const size_t iAtom2 = bond.iAtom;
            if( iAtom2 < iAtom ) continue; 
            Atom b = this->arena.getAtom( iAtom2 );
            switch( bond.range ) {
                default:
                case Neighborhood::Moore:       pGC->SetPen(thinBondPen);  break;
                case Neighborhood::vonNeumann:  pGC->SetPen(thickBondPen); break;
                case Neighborhood::vonNeumann2: 
                case Neighborhood::Moore2:      pGC->SetPen(*wxGREY_PEN); break;
            }
            pGC->StrokeLine( ( a.x + 0.5 ) * scale, ( a.y + 0.5 ) * scale, ( b.x + 0.5 ) * scale, ( b.y + 0.5 ) * scale );
        }
    }

    // draw stats
    pGC->SetFont(*wxNORMAL_FONT,*wxBLACK);
    pGC->DrawText( wxString::Format("Its: %d",this->iterations), 10, 10 );
    pGC->DrawText( wxString::Format("Groups: %d",this->arena.getNumberOfGroups()), 10, 30 );
}

//-------------------------------------------------------------------------------------

void MyFrame::OnSize(wxSizeEvent& event) {
    this->Refresh( false );
}

//-------------------------------------------------------------------------------------

void MyFrame::OnIdle(wxIdleEvent& event) {
    if( render_every == 0 ) return;

    try {
        this->arena.update();
    }
    catch( exception& e ) {
        wxMessageBox( e.what() );
    }
    this->iterations++;
    if( iterations % this->render_every == 0 )
        this->Refresh( false );
    event.RequestMore(); // render continuously, not only once on idle
}

//-------------------------------------------------------------------------------------

void MyFrame::OnStep(wxCommandEvent& WXUNUSED(event)) {
    try {
        this->arena.update();
    }
    catch( exception& e ) {
        wxMessageBox( e.what() );
    }
    this->iterations++;
    this->render_every = 0;
    this->Refresh(false);
}

void MyFrame::initBrushes() { 
    // assign some colors
    brushes.push_back( *wxRED_BRUSH );
    brushes.push_back( *wxYELLOW_BRUSH );
    brushes.push_back( *wxCYAN_BRUSH );
    brushes.push_back( *wxLIGHT_GREY_BRUSH );
    brushes.push_back( *wxBLUE_BRUSH );
    brushes.push_back( *wxGREEN_BRUSH );
    for( int i = 0; i < 30; ++i )
        brushes.push_back( wxBrush( wxColour( 128 + rand()%128, 128 + rand()%128, 128 + rand()%128 ) ) );

}