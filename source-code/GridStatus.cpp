#include "GridStatus.h"

GridStatus::GridStatus(wxWindow* parent): wxPanel(parent)
{
    SetBackgroundColour(wxColor(255, 173, 173));

	m_StatusSpeed = new StatusSpeed(this);
	m_StatusControls = new StatusControls(this);
	m_StatusCells = new StatusCells(this);

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(m_StatusSpeed, 0, wxEXPAND);
	sizer->Add(m_StatusControls, 0, wxEXPAND | wxLEFT, 48);
	sizer->Add(m_StatusCells, 0, wxEXPAND | wxLEFT, 48);

	this->SetSizerAndFit(sizer);
}

GridStatus::~GridStatus()
{
	wxDELETE(m_StatusSpeed);
	wxDELETE(m_StatusControls);
	wxDELETE(m_StatusCells);
}

StatusSpeed* GridStatus::GetStatusSpeed()
{
	return m_StatusSpeed;
}

StatusControls* GridStatus::GetStatusControls()
{
	return m_StatusControls;
}

StatusCells* GridStatus::GetStatusCells()
{
	return m_StatusCells;
}
