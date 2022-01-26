#include "StatusCells.h"

StatusCells::StatusCells(wxWindow* parent) : wxPanel(parent)
{
    //SetBackgroundColour(wxColour(242, 204, 143));

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
	std::string label = "Generation=" + std::to_string(m_CountGeneration) + message;

	m_TextCountGeneration->SetLabel(label);

	//Layout();
	//Update();
}

void StatusCells::SetPopulationMessage(std::string message)
{
	std::string label = "Population=" + std::to_string(m_CountPopulation) + message;

	m_TextCountPopulation->SetLabel(label);

	//Layout();
	//Update();
}

void StatusCells::BuildInterface()
{
	m_TextCountGeneration = new wxStaticText(this, wxID_ANY, "Generation: 0");
	m_TextCountPopulation = new wxStaticText(this, wxID_ANY, "Population: 0");

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(m_TextCountGeneration, 0, wxALIGN_CENTER_VERTICAL);
	sizer->Add(m_TextCountPopulation, 0, wxALIGN_CENTER_VERTICAL);
	//sizer->Add(m_TextCountPopulation, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 16);
	sizer->AddSpacer(48);

	this->SetSizer(sizer);
}

void StatusCells::UpdateTextCountGeneration()
{
	std::string label = "Generation=" + std::to_string(m_CountGeneration);

	m_TextCountGeneration->SetLabel(label);

	//Layout();
	//Update();
}

void StatusCells::UpdateTextCountPopulation()
{
	std::string label = "Population=" + std::to_string(m_CountPopulation);

	m_TextCountPopulation->SetLabel(label);

	//Layout();
	//Update();
}
