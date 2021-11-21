#include "StatusCells.h"

StatusCells::StatusCells(wxWindow* parent, wxBoxSizer* sizer)
{
	m_TextCountGeneration = new wxStaticText(parent, wxID_ANY, "Generation: 0");
	m_TextCountPopulation= new wxStaticText(parent, wxID_ANY, "Population: 0");

	sizer->Add(m_TextCountGeneration, 0, wxALIGN_CENTER_VERTICAL);

	sizer->AddSpacer(16);

	sizer->Add(m_TextCountPopulation, 0, wxALIGN_CENTER_VERTICAL);
}

StatusCells::~StatusCells()
{
}

int StatusCells::GetCountGeneration()
{
	return m_CountGeneration;
}

int StatusCells::GetCountPopulation()
{
	return m_CountPopulation;
}

void StatusCells::SetCountGeneration(int n)
{
	m_CountGeneration = n;
	UpdateTextCountGeneration();
}

void StatusCells::SetCountPopulation(int n)
{
	m_CountPopulation = n;
	UpdateTextCountPopulation();
}

void StatusCells::UpdateTextCountGeneration()
{
	std::string label = "Generation=" + std::to_string(m_CountGeneration);
	m_TextCountGeneration->SetLabel(label);
}

void StatusCells::UpdateTextCountPopulation()
{
	std::string label = "Population=" + std::to_string(m_CountPopulation);
	m_TextCountPopulation->SetLabel(label);
}
