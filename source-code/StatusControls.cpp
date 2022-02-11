#include "StatusControls.h"

StatusControls::StatusControls(wxWindow* parent) : wxPanel(parent)
{
    //SetBackgroundColour(wxColour(242, 204, 143));

    BuildInterface();
}

StatusControls::~StatusControls()
{
}

void StatusControls::SetGrid(Grid* grid)
{
    m_Grid = grid;
}

void StatusControls::SetPlayButton(bool play)
{
    if (play)
    {
        m_PlayButton->SetBitmap(wxBitmap("BTN_PLAY", wxBITMAP_TYPE_PNG_RESOURCE));
        m_PlayButton->SetToolTip("Play\t(Ctrl+Space)");
    }
    else
    {
        m_PlayButton->SetBitmap(wxBitmap("BTN_PAUSE", wxBITMAP_TYPE_PNG_RESOURCE));
        m_PlayButton->SetToolTip("Pause\t(Ctrl+Space)");
    }
}

void StatusControls::BuildInterface()
{
    m_PlayButton = new wxBitmapButton(this, Ids::ID_BUTTON_PLAY, wxBitmap("BTN_PLAY", wxBITMAP_TYPE_PNG_RESOURCE), wxDefaultPosition, wxSize(32, 32));
    wxBitmapButton* reset = new wxBitmapButton(this, Ids::ID_BUTTON_RESET, wxBitmap("BTN_RESET", wxBITMAP_TYPE_PNG_RESOURCE), wxDefaultPosition, wxSize(32, 32));
    wxBitmapButton* center = new wxBitmapButton(this, Ids::ID_BUTTON_CENTER, wxBitmap("BTN_CENTER", wxBITMAP_TYPE_PNG_RESOURCE), wxDefaultPosition, wxSize(32, 32));
    wxBitmapButton* generation = new wxBitmapButton(this, Ids::ID_BUTTON_GENERATION, wxBitmap("BTN_GENERATION", wxBITMAP_TYPE_PNG_RESOURCE), wxDefaultPosition, wxSize(32, 32));

    center->SetToolTip("Go To Center\t(Ctrl+M)");
    center->Bind(wxEVT_BUTTON, &StatusControls::GoToCenter, this);

    m_PlayButton->SetToolTip("Play\t(Ctrl+Space)");
    m_PlayButton->Bind(wxEVT_BUTTON, &StatusControls::Play, this);

    reset->SetToolTip("Reset\t(Ctrl+R)");
    reset->Bind(wxEVT_BUTTON, &StatusControls::Reset, this);

    generation->SetToolTip("Next Generation\t(Ctrl+G)");
    generation->Bind(wxEVT_BUTTON, &StatusControls::NextGeneration, this);

    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(center, 0, wxALIGN_CENTER_VERTICAL);
    sizer->Add(reset, 0, wxALIGN_CENTER_VERTICAL);
    sizer->Add(m_PlayButton, 0, wxALIGN_CENTER_VERTICAL);
    sizer->Add(generation, 0, wxALIGN_CENTER_VERTICAL);

    SetSizerAndFit(sizer);
}

void StatusControls::GoToCenter(wxCommandEvent& evt)
{
    m_Grid->SetFocus();
    m_Grid->ScrollToCenter();
}

void StatusControls::Reset(wxCommandEvent& evt)
{
    m_Grid->SetFocus();
    m_Grid->Reset();
}

void StatusControls::Play(wxCommandEvent& evt)
{
    m_Grid->SetFocus();

    // pause
    if (!m_Grid->GetPaused())
    {
        SetPlayButton(1);
        m_Grid->PauseUniverse();

        return;
    }

    // play
    if (m_Grid->GetPaused())
    {
        SetPlayButton(0);
        m_Grid->OnPlayUniverse();
    }
}

void StatusControls::NextGeneration(wxCommandEvent& evt)
{
    m_Grid->SetFocus();
    m_Grid->OnNextGeneration();
}
