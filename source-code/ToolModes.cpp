#include "ToolModes.h"
#include "Ids.h"

ToolModes::ToolModes(wxWindow* parent) : wxPanel(parent)
{
	BuildInterface();
}

ToolModes::~ToolModes()
{
}

int ToolModes::GetMode()
{
	return m_Mode;
}

int ToolModes::GetIndex()
{
	return m_Index;
}

void ToolModes::SetIndex(char direction, int index)
{
	switch (direction)
	{
	case 'L':
	{
		if (m_Index == 1)
		{
			// error

			return;
		}

		m_Index--;
		break;
	}
	case 'R':
	{
		if (m_Index == m_MaximumIndex)
		{
			// error

			return;
		}

		m_Index++;
		break;
	}
	case NULL:
	{
		if (!(m_Index > 1 && m_Index < m_MaximumIndex))
		{
			// error

			return;
		}

		m_Index = index;
		break;
	}
	default:
		return;
	}

	UpdateTextIndex();
	UpdateStateColor();
}

void ToolModes::BuildInterface()
{
	wxButton* draw = new wxButton(this, Ids::ID_MODE_DRAW, "D");
	wxButton* pick = new wxButton(this, Ids::ID_MODE_PICK, "P");
	wxButton* drag = new wxButton(this, Ids::ID_MODE_DRAG, "M");

	draw->SetToolTip("Draw");
	pick->SetToolTip("Pick");
	drag->SetToolTip("Drag");

	m_TextIndex = new wxStaticText(this, wxID_ANY, "1 / 1");
	m_StateColor = new wxButton(this, wxID_ANY);
	m_StateColor->Enable(false);

	wxButton* previous = new wxButton(this, Ids::ID_BUTTON_PREV, "<");
	wxButton* next = new wxButton(this, Ids::ID_BUTTON_NEXT, ">");
	previous->SetToolTip("Previous");
	next->SetToolTip("Next");

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(draw, 0, wxALIGN_CENTER_VERTICAL);
	sizer->Add(pick, 0, wxALIGN_CENTER_VERTICAL);
	sizer->Add(drag, 0, wxALIGN_CENTER_VERTICAL);

	sizer->AddSpacer(16);
	sizer->Add(m_TextIndex, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER);
	sizer->Add(previous, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER);
	sizer->Add(m_StateColor, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER);
	sizer->Add(next, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER);

	this->SetSizer(sizer);
}

void ToolModes::UpdateTextIndex()
{
	std::string label = std::to_string(m_Index) + " / " + std::to_string(m_MaximumIndex);
	m_TextIndex->SetLabel(label);
}

void ToolModes::UpdateStateColor() // to do
{
	std::srand(std::time(NULL));

	int r = std::rand() % 256;
	int g = std::rand() % 256;
	int b = std::rand() % 256;
	m_StateColor->SetBackgroundColour(wxColour(r, g, b));
}
