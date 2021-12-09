#include "ToolStates.h"
#include "Ids.h"

#define cout(x) wxMessageBox(std::to_string(x), "debug")

wxBEGIN_EVENT_TABLE(ToolStates, wxPanel)
    EVT_BUTTON(Ids::ID_BUTTON_PREV, ToolStates::OnPrev)
    EVT_BUTTON(Ids::ID_BUTTON_NEXT, ToolStates::OnNext)
wxEND_EVENT_TABLE()

ToolStates::ToolStates(wxWindow* parent) : wxPanel(parent)
{
    SetBackgroundColour(wxColour(242, 204, 143));

    BuildInterface();
}

ToolStates::~ToolStates()
{
}

int ToolStates::GetIndex()
{
    return m_Index;
}

std::pair<std::string, wxColour> ToolStates::GetState()
{
    return m_States[m_Index];
}

void ToolStates::SetStates(std::vector<std::pair<std::string, wxColour>> states)
{
	m_States = states;

	int size = states.size() - 1;
	if (m_Index > size)
	{
		m_Index = size;
	}
	m_MaximumIndex = size;

	UpdateTextIndex();
	UpdateState();
}

void ToolStates::SetListStates(ListStates* list)
{
	m_ListStates = list;
}

void ToolStates::SetState(std::string state)
{
	if (m_States[m_Index].first == state) return;

	for (int i = 0; i < m_States.size(); i++)
	{
		if (m_States[i].first == state)
		{
			m_Index = i;

			UpdateTextIndex();
			UpdateState();
			return;
		}
	}
}

void ToolStates::SelectPrevState()
{
	m_Index = std::max(0, m_Index - 1);

	UpdateTextIndex();
	UpdateState();

	int selection = m_ListStates->GetFirstSelected();
	while (selection != -1)
	{
		m_ListStates->Select(selection, false);
		selection = m_ListStates->GetNextSelected(selection);
	}
	m_ListStates->Select(m_Index);
	m_ListStates->EnsureVisible(m_Index);
}

void ToolStates::SelectNextState()
{
	m_Index = std::min(m_MaximumIndex, m_Index + 1);

	UpdateTextIndex();
	UpdateState();

	int selection = m_ListStates->GetFirstSelected();
	while (selection != -1)
	{
		m_ListStates->Select(selection, false);
		selection = m_ListStates->GetNextSelected(selection);
	}
	m_ListStates->Select(m_Index);
	m_ListStates->EnsureVisible(m_Index);
}

void ToolStates::BuildInterface()
{
	m_TextIndex = new wxStaticText(this, wxID_ANY, "0 / 0");

	m_State = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(32, 32), wxSIMPLE_BORDER);
	m_State->SetBackgroundColour(wxColour("white"));
	m_State->SetToolTip("FREE");
	m_State->Refresh();

	wxButton* previous = new wxButton(this, Ids::ID_BUTTON_PREV, "<", wxDefaultPosition, wxSize(32, 32));
	wxButton* next = new wxButton(this, Ids::ID_BUTTON_NEXT, ">", wxDefaultPosition, wxSize(32, 32));
	previous->SetToolTip("Previous");
	next->SetToolTip("Next");

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(previous, 0, wxALIGN_CENTER_VERTICAL);
	sizer->Add(next, 0, wxALIGN_CENTER_VERTICAL);
	sizer->Add(m_State, 0, wxALIGN_CENTER_VERTICAL);
	sizer->Add(m_TextIndex, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 8);
	sizer->AddSpacer(48);

	this->SetSizerAndFit(sizer);
}

void ToolStates::UpdateTextIndex()
{
	std::string label = std::to_string(m_Index) + " / " + std::to_string(m_MaximumIndex);
	m_TextIndex->SetLabel(label);
	
	//Layout();
}

void ToolStates::UpdateState()
{
	m_State->SetBackgroundColour(m_States[m_Index].second);
	m_State->SetToolTip(m_States[m_Index].first);
	m_State->Refresh();
}

void ToolStates::OnPrev(wxCommandEvent& evt)
{
	SelectPrevState();
}

void ToolStates::OnNext(wxCommandEvent& evt)
{
	SelectNextState();
}
