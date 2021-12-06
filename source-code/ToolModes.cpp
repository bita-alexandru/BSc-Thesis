#include "ToolModes.h"
#include "Ids.h"


ToolModes::ToolModes(wxWindow* parent) : wxPanel(parent)
{
    SetBackgroundColour(wxColour(242, 204, 143));

	BuildInterface();
}

ToolModes::~ToolModes()
{
}

char ToolModes::GetMode()
{
	return m_Mode;
}

void ToolModes::SetMode(char mode)
{
	switch (mode)
	{
	case 'D':
		m_Draw->Disable();
		m_Pick->Enable();
		m_Move->Enable();
		break;
	case 'P':
		m_Draw->Enable();
		m_Pick->Disable();
		m_Move->Enable();
		break;
	case 'M':
		m_Draw->Enable();
		m_Pick->Enable();
		m_Move->Disable();
		break;
	default:
		return;
	}

	m_Mode = mode;
	UpdateTextMode();
}

void ToolModes::BuildInterface()
{
	m_Draw = new wxButton(this, Ids::ID_MODE_DRAW, "D", wxDefaultPosition, wxSize(32, 32));
	m_Pick = new wxButton(this, Ids::ID_MODE_PICK, "P", wxDefaultPosition, wxSize(32, 32));
	m_Move = new wxButton(this, Ids::ID_MODE_MOVE, "M", wxDefaultPosition, wxSize(32, 32));
	m_Draw->Disable();
	m_Draw->SetToolTip("Draw");
	m_Pick->SetToolTip("Pick");
	m_Move->SetToolTip("Move");

	m_TextMode = new wxStaticText(this, wxID_ANY, "Mode=Draw");

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(m_Draw, 0, wxALIGN_CENTER_VERTICAL);
	sizer->Add(m_Pick, 0, wxALIGN_CENTER_VERTICAL);
	sizer->Add(m_Move, 0, wxALIGN_CENTER_VERTICAL);
	sizer->Add(m_TextMode, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 48);

	this->SetSizerAndFit(sizer);

	for (auto it : { m_Draw, m_Pick, m_Move })
	{
		it->Bind(wxEVT_BUTTON, &ToolModes::OnMode, this);
	}
}

void ToolModes::UpdateTextMode()
{
	std::string mode;
	switch (m_Mode)
	{
		case 'D':
		{
			mode = "Draw";
			break;
		}
		case 'P':
		{
			mode = "Pick";
			break;
		}
		case 'M':
		{
			mode = "Move";
			break;
		}
		default :
		{
			mode = "";
			break;
		}
	}

	std::string label ="Mode=" + mode;
	m_TextMode->SetLabel(label);

	//Layout();
}

void ToolModes::OnMode(wxCommandEvent& evt)
{
	char mode = (evt.GetId() - Ids::ID_MODE_DRAW)["DPM"];

	SetMode(mode);
}
