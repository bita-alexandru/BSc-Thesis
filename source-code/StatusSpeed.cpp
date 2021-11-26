#include "StatusSpeed.h"

StatusSpeed::StatusSpeed(wxWindow* parent, wxBoxSizer* sizer)
{
    wxButton* slower = new wxButton(parent, Constants::ID_BUTTON_SLOWER, "<<");
    wxButton* faster = new wxButton(parent, Constants::ID_BUTTON_FASTER, ">>");

    slower->SetToolTip("Slower");
    faster->SetToolTip("Faster");

    std::string delay = std::to_string(m_Delay);
    delay.erase(delay.find_last_not_of('0') + 1, std::string::npos);
    delay += "s";

    std::string label = "Delay: " + delay;
    m_TextDelay = new wxStaticText(parent, wxID_ANY, label);

    sizer->Add(slower, 0, wxALIGN_CENTER_VERTICAL);
    sizer->Add(faster, 0, wxALIGN_CENTER_VERTICAL);
    sizer->Add(m_TextDelay, 0, wxALIGN_CENTER_VERTICAL);
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

void StatusSpeed::UpdateDelay()
{
}
