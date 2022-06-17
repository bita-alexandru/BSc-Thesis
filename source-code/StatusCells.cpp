#include "StatusCells.h"

StatusCells::StatusCells(wxWindow* parent) : wxPanel(parent)
{
	BuildInterface();
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

void StatusCells::UpdateCountGeneration(int n)
{
	m_CountGeneration += n;

	UpdateTextCountGeneration();
}

void StatusCells::UpdateCountPopulation(int n)
{
	m_CountPopulation += n;

	UpdateTextCountPopulation();
}

void StatusCells::SetGenerationMessage(std::string message)
{
	wxString label = wxString::Format("Generation=%i%s", m_CountGeneration, message);

	m_TextCountGeneration->SetLabel(label);
}

void StatusCells::SetPopulationMessage(std::string message)
{
	wxString label = wxString::Format("Population=%i%s", m_CountPopulation, message);

	m_TextCountPopulation->SetLabel(label);
}

void StatusCells::BuildInterface()
{
	m_TextCountGeneration = new wxStaticText(this, wxID_ANY, "Generation:0");
	m_TextCountPopulation = new wxStaticText(this, wxID_ANY, "Population:0");

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(m_TextCountGeneration, 0, wxRIGHT, 256);
	sizer->Add(m_TextCountPopulation, 0, wxRIGHT, 256);

	SetSizer(sizer);
}

void StatusCells::UpdateTextCountGeneration()
{
	wxString label = wxString::Format("Generation=%i", m_CountGeneration);

	m_TextCountGeneration->SetLabel(label);
}

void StatusCells::UpdateTextCountPopulation()
{
	wxString label = wxString::Format("Population=%i", m_CountPopulation);

	m_TextCountPopulation->SetLabel(label);
}
