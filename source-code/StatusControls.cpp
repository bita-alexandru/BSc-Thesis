#include "StatusControls.h"

StatusControls::StatusControls(wxWindow* parent) : wxPanel(parent)
{
    SetBackgroundColour(wxColour(242, 204, 143));

    BuildInterface();
}

StatusControls::~StatusControls()
{
}

char StatusControls::GetStartState()
{
    return m_StartState;
}

void StatusControls::SetStartState(char state)
{
    // to do, change bitmaps, not labels
    switch (state)
    {
    case 'S':
    {
        m_StartButton->SetLabel("Start");
        m_StartButton->SetToolTip("Start");
        break;
    }
    case 'P':
    {
        m_StartButton->SetLabel("Pause");
        m_StartButton->SetToolTip("Pause");
        break;
    }
    default:
        break;
    }
}

void StatusControls::SetGrid(Grid* grid)
{
    m_Grid = grid;
}

void StatusControls::BuildInterface()
{
    wxBitmapButton* start = new wxBitmapButton(this, Ids::ID_BUTTON_START, wxBitmap("BTN_START", wxBITMAP_TYPE_PNG_RESOURCE), wxDefaultPosition, wxSize(32, 32));
    wxBitmapButton* reset = new wxBitmapButton(this, Ids::ID_BUTTON_RESET, wxBitmap("BTN_RESET", wxBITMAP_TYPE_PNG_RESOURCE), wxDefaultPosition, wxSize(32, 32));
    wxBitmapButton* center = new wxBitmapButton(this, Ids::ID_BUTTON_CENTER, wxBitmap("BTN_CENTER", wxBITMAP_TYPE_PNG_RESOURCE), wxDefaultPosition, wxSize(32, 32));
    wxBitmapButton* step = new wxBitmapButton(this, Ids::ID_BUTTON_STEP, wxBitmap("BTN_STEP", wxBITMAP_TYPE_PNG_RESOURCE), wxDefaultPosition, wxSize(32, 32));
    wxBitmapButton* generation = new wxBitmapButton(this, Ids::ID_BUTTON_GENERATION, wxBitmap("BTN_GENERATION", wxBITMAP_TYPE_PNG_RESOURCE), wxDefaultPosition, wxSize(32, 32));

    center->SetToolTip("Go To Center");
    center->Bind(wxEVT_BUTTON, &StatusControls::GoToCenter, this);

    start->SetToolTip("Start");

    reset->SetToolTip("Reset");
    reset->Bind(wxEVT_BUTTON, &StatusControls::Reset, this);

    step->SetToolTip("Next Step");

    generation->SetToolTip("Next Generation");
    generation->Bind(wxEVT_BUTTON, &StatusControls::NextGeneration, this);

    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(start, 0, wxALIGN_CENTER_VERTICAL);
    sizer->Add(reset, 0, wxALIGN_CENTER_VERTICAL);
    sizer->Add(center, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 24);
    sizer->Add(step, 0, wxALIGN_CENTER_VERTICAL);
    sizer->Add(generation, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 48);

    this->SetSizer(sizer);
}

void StatusControls::GoToCenter(wxCommandEvent& evt)
{
    m_Grid->ScrollToCenter();
    m_Grid->SetFocus();
}

void StatusControls::Reset(wxCommandEvent& evt)
{
    m_Grid->Reset();
    m_Grid->SetFocus();
}

void StatusControls::NextGeneration(wxCommandEvent& evt)
{
    bool result = m_Grid->NextGeneration();

    if (result) wxLogDebug("SUCCES");
    else wxLogDebug("FAIL");
}
