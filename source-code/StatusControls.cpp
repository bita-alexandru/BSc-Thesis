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

void StatusControls::BuildInterface()
{
    wxButton* start = new wxButton(this, Ids::ID_BUTTON_START, "S", wxDefaultPosition, wxSize(32, 32));
    wxButton* reset = new wxButton(this, Ids::ID_BUTTON_RESET, "R", wxDefaultPosition, wxSize(32, 32));
    wxButton* step = new wxButton(this, Ids::ID_BUTTON_STEP, "NS", wxDefaultPosition, wxSize(32, 32));
    wxButton* generation = new wxButton(this, Ids::ID_BUTTON_GENERATION, "NG", wxDefaultPosition, wxSize(32, 32));

    start->SetToolTip("Start");
    reset->SetToolTip("Reset");
    step->SetToolTip("Next step");
    generation->SetToolTip("Next generation");

    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(start, 0, wxALIGN_CENTER_VERTICAL);
    sizer->Add(reset, 0, wxALIGN_CENTER_VERTICAL);
    sizer->Add(step, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 16);
    sizer->Add(generation, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 48);

    this->SetSizer(sizer);
}
