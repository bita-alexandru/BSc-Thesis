#include "ToolModes.h"

ToolModes::ToolModes(wxWindow* parent, wxBoxSizer* sizer)
{
	wxButton* draw = new wxButton(parent, IDs::ID_MODE_DRAW, "D");
	wxButton* pick = new wxButton(parent, IDs::ID_MODE_PICK, "P");
	wxButton* drag = new wxButton(parent, IDs::ID_MODE_DRAG, "M");

	sizer->Add(draw, 0, wxALIGN_CENTER_VERTICAL);
	sizer->Add(pick, 0, wxALIGN_CENTER_VERTICAL);
	sizer->Add(drag, 0, wxALIGN_CENTER_VERTICAL);

	sizer->AddSpacer(16);

	draw->SetToolTip("Draw");
	pick->SetToolTip("Pick");
	drag->SetToolTip("Drag");

	m_TextIndex = new wxStaticText(parent, wxID_ANY, "1 / 1");
	m_StateColor = new wxButton(parent, wxID_ANY);

	wxButton* previous = new wxButton(parent, IDs::ID_BUTTON_PREV, "<");
	wxButton* next = new wxButton(parent, IDs::ID_BUTTON_NEXT, ">");

	sizer->Add(m_TextIndex, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER);
	sizer->Add(previous, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER);
	sizer->Add(m_StateColor, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER);
	sizer->Add(next, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER);

	previous->SetToolTip("Previous");
	next->SetToolTip("Next");
	
	m_StateColor->Enable(false);
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
