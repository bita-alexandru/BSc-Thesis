#include "ToolModes.h"
#include "Ids.h"

wxBEGIN_EVENT_TABLE(ToolModes, wxPanel)
	EVT_BUTTON(Ids::ID_BUTTON_PREV, ToolModes::OnPrev)
	EVT_BUTTON(Ids::ID_BUTTON_NEXT, ToolModes::OnNext)
wxEND_EVENT_TABLE()

ToolModes::ToolModes(wxWindow* parent) : wxPanel(parent)
{
	BuildInterface();
}

ToolModes::~ToolModes()
{
}

char ToolModes::GetMode()
{
	return m_Mode;
}

int ToolModes::GetIndex()
{
	return m_Index;
}

void ToolModes::SetMode(char mode)
{
	switch (mode)
	{
	case 'D':
		m_Draw->SetValue(true);
		m_Pick->SetValue(false);
		m_Move->SetValue(false);
		break;
	case 'P':
		m_Draw->SetValue(false);
		m_Pick->SetValue(true);
		m_Move->SetValue(false);
		break;
	case 'M':
		m_Draw->SetValue(false);
		m_Pick->SetValue(false);
		m_Move->SetValue(true);
		break;
	default:
		return;
	}

	m_Mode = mode;
}

void ToolModes::SetStates(std::vector<std::pair<std::string, wxColour>> states)
{
	m_States = states;

	int size = states.size() - 1;
	if (m_Index > size)
	{
		m_Index = size;
	}
	m_MaximumIndex = size;

	UpdateTextIndex();
	UpdateStateColor();
}

void ToolModes::SetListStates(wxListView* list)
{
	m_ListStates = list;
}

void ToolModes::BuildInterface()
{
	m_Draw = new wxToggleButton(this, Ids::ID_MODE_DRAW, "D");
	m_Pick = new wxToggleButton(this, Ids::ID_MODE_PICK, "P");
	m_Move = new wxToggleButton(this, Ids::ID_MODE_MOVE, "M");
	m_Draw->SetValue(true);
	m_Draw->SetToolTip("Draw");
	m_Pick->SetToolTip("Pick");
	m_Move->SetToolTip("Move");

	m_TextIndex = new wxStaticText(this, wxID_ANY, "0 / 0");
	m_State = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(24, 24), wxSIMPLE_BORDER);
	m_State->SetBackgroundColour(wxColour("#FFFFFF"));
	m_State->SetToolTip("FREE");
	m_State->Refresh();

	wxButton* previous = new wxButton(this, Ids::ID_BUTTON_PREV, "<");
	wxButton* next = new wxButton(this, Ids::ID_BUTTON_NEXT, ">");
	previous->SetToolTip("Previous");
	next->SetToolTip("Next");

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(m_Draw, 0, wxALIGN_CENTER_VERTICAL);
	sizer->Add(m_Pick, 0, wxALIGN_CENTER_VERTICAL);
	sizer->Add(m_Move, 0, wxALIGN_CENTER_VERTICAL);
	sizer->AddSpacer(16);
	sizer->Add(m_TextIndex, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER | wxRIGHT);
	sizer->Add(previous, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER);
	sizer->Add(m_State, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER);
	sizer->Add(next, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER);

	this->SetSizer(sizer);

	for (auto it : { m_Draw, m_Pick, m_Move })
	{
		it->Bind(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, &ToolModes::OnToggleButton, this);
	}
}

void ToolModes::UpdateTextIndex()
{
	std::string label = std::to_string(m_Index) + " / " + std::to_string(m_MaximumIndex);
	m_TextIndex->SetLabel(label);

	Layout();
}

void ToolModes::UpdateStateColor() // to do
{
	m_State->SetBackgroundColour(m_States[m_Index].second);
	m_State->SetToolTip(m_States[m_Index].first);
	m_State->Refresh();
}

void ToolModes::OnToggleButton(wxCommandEvent& evt)
{
	char mode = (evt.GetId() - Ids::ID_MODE_DRAW)["DPM"];

	SetMode(mode);
}

void ToolModes::OnPrev(wxCommandEvent& evt)
{
	m_Index = std::max(0, m_Index - 1);

	UpdateTextIndex();
	UpdateStateColor();

	int selection = m_ListStates->GetFirstSelected();
	while (selection != -1)
	{
		m_ListStates->Select(selection, false);
		selection = m_ListStates->GetNextSelected(selection);
	}
	m_ListStates->Select(m_Index);
	m_ListStates->EnsureVisible(m_Index);
}

void ToolModes::OnNext(wxCommandEvent& evt)
{
	m_Index = std::min(m_MaximumIndex, m_Index + 1);

	UpdateTextIndex();
	UpdateStateColor();

	int selection = m_ListStates->GetFirstSelected();
	while (selection != -1)
	{
		m_ListStates->Select(selection, false);
		selection = m_ListStates->GetNextSelected(selection);
	}
	m_ListStates->Select(m_Index);
	m_ListStates->EnsureVisible(m_Index);
}
