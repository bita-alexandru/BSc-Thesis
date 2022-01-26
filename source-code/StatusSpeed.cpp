#include "StatusSpeed.h"

StatusSpeed::StatusSpeed(wxWindow* parent) : wxPanel(parent)
{
    //SetBackgroundColour(wxColour(242, 204, 143));

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
    wxBitmapButton* slower = new wxBitmapButton(this, Ids::ID_BUTTON_SLOWER, wxBitmap("BTN_SLOWER", wxBITMAP_TYPE_PNG_RESOURCE), wxDefaultPosition, wxSize(32, 32));
    wxBitmapButton* faster = new wxBitmapButton(this, Ids::ID_BUTTON_FASTER, wxBitmap("BTN_FASTER", wxBITMAP_TYPE_PNG_RESOURCE), wxDefaultPosition, wxSize(32, 32));
    slower->SetToolTip("Lower\t(Ctrl+,)");
    faster->SetToolTip("Higher\t(Ctrl+.)");

    std::string delay = std::to_string(m_Delay);
    delay.erase(delay.find_last_not_of('0') + 1, std::string::npos);
    delay += "s";

    std::string label = "Delay: " + delay;
    m_TextDelay = new wxStaticText(this, wxID_ANY, label);

    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(slower, 0, wxALIGN_CENTER_VERTICAL);
    sizer->Add(faster, 0, wxALIGN_CENTER_VERTICAL);
    sizer->Add(m_TextDelay, 0, wxALIGN_CENTER_VERTICAL);
    //sizer->Add(m_TextDelay, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 48);

    this->SetSizerAndFit(sizer);
}

void StatusSpeed::UpdateDelay()
{
}
