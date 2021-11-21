#include "StatusControls.h"

StatusControls::StatusControls(wxWindow* parent, wxBoxSizer* sizer)
{
    wxButton* start = new wxButton(parent, ID_BUTTON_START, "Start");
    wxButton* reset = new wxButton(parent, ID_BUTTON_RESET, "Reset");
    wxButton* step = new wxButton(parent, ID_BUTTON_STEP, "Next Step");
    wxButton* generation = new wxButton(parent, ID_BUTTON_GENERATION, "Next Generation");

    start->SetToolTip("Start");
    reset->SetToolTip("Reset");
    step->SetToolTip("Next step");
    generation->SetToolTip("Next generation");

    sizer->Add(start, 0, wxALIGN_CENTER_VERTICAL);
    sizer->Add(reset, 0, wxALIGN_CENTER_VERTICAL);

    sizer->AddSpacer(16);

    sizer->Add(step, 0, wxALIGN_CENTER_VERTICAL);
    sizer->Add(generation, 0, wxALIGN_CENTER_VERTICAL);
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
