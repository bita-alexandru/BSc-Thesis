#include "GridStatus.h"

GridStatus::GridStatus(wxWindow* parent): wxPanel(parent)
{
	SetBackgroundColour(wxColor(255, 173, 173));

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	m_StatusSpeed = new StatusSpeed(this, sizer);
	sizer->AddSpacer(64);

	m_StatusControls = new StatusControls(this, sizer);
	sizer->AddSpacer(64);

	m_StatusCells = new StatusCells(this, sizer);

	this->SetSizer(sizer);
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
