#include "StatusSpeed.h"

StatusSpeed::StatusSpeed(wxWindow* parent) : wxPanel(parent)
{
    BuildInterface();
}

StatusSpeed::~StatusSpeed()
{
}

int StatusSpeed::GetDelay()
{
    return m_Delay;
}

void StatusSpeed::SetDelay(char mode)
{
}

void StatusSpeed::BuildInterface()
{
    wxButton* slower = new wxButton(this, Ids::ID_BUTTON_SLOWER, "<<", wxDefaultPosition, wxSize(32, 32));
    wxButton* faster = new wxButton(this, Ids::ID_BUTTON_FASTER, ">>", wxDefaultPosition, wxSize(32, 32));
    slower->SetToolTip("Slower");
    faster->SetToolTip("Faster");

    std::string delay = std::to_string(m_Delay);
    delay.erase(delay.find_last_not_of('0') + 1, std::string::npos);
    delay += "s";

    std::string label = "Delay: " + delay;
    m_TextDelay = new wxStaticText(this, wxID_ANY, label);

    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(slower, 0, wxALIGN_CENTER_VERTICAL);
    sizer->Add(faster, 0, wxALIGN_CENTER_VERTICAL);
    sizer->Add(m_TextDelay, 0, wxALIGN_CENTER_VERTICAL);

    this->SetSizerAndFit(sizer);
}

void StatusSpeed::UpdateDelay()
{
}
