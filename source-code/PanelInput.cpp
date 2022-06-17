#include "PanelInput.h"
#include "Colors.h"

PanelInput::PanelInput(wxWindow* parent) : wxScrolledWindow(parent)
{
	SetBackgroundColour(wxColor(Colors::COLOR_MAIN_R, Colors::COLOR_MAIN_G, Colors::COLOR_MAIN_B));

	BuildInterface();
}

PanelInput::~PanelInput()
{
	wxDELETE(m_InputStates);
	wxDELETE(m_InputNeighbors);
	wxDELETE(m_InputRules);
}

InputStates* PanelInput::GetInputStates()
{
	return m_InputStates;
}

InputNeighbors* PanelInput::GetInputNeighbors()
{
	return m_InputNeighbors;
}

InputRules* PanelInput::GetInputRules()
{
	return m_InputRules;
}

void PanelInput::BuildInterface()
{
	m_InputStates = new InputStates(this);
	m_InputNeighbors = new InputNeighbors(this);
	m_InputRules = new InputRules(this);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(m_InputStates, 1, wxEXPAND | wxALL, 6);
	sizer->Add(m_InputNeighbors, 1, wxEXPAND | wxLEFT | wxRIGHT, 6);
	sizer->Add(m_InputRules, 1, wxEXPAND | wxALL, 6);

	SetSizer(sizer);
	SetScrollRate(1, 1);
}
