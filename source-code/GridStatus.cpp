#include "GridStatus.h"

GridStatus::GridStatus(wxWindow* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE)
{
    SetBackgroundColour(wxColor(211, 211, 211));

	m_StatusDelay = new StatusDelay(this);
	m_StatusControls = new StatusControls(this);
	m_StatusCells = new StatusCells(this);

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(m_StatusDelay, 0, wxEXPAND);
	sizer->Add(m_StatusControls, 0, wxEXPAND);
	sizer->Add(m_StatusCells, 0, wxEXPAND | wxTOP | wxBOTTOM, 4);

	SetSizerAndFit(sizer);
}

GridStatus::~GridStatus()
{
	wxDELETE(m_StatusDelay);
	wxDELETE(m_StatusControls);
	wxDELETE(m_StatusCells);
}

StatusDelay* GridStatus::GetStatusDelay()
{
	return m_StatusDelay;
}

StatusControls* GridStatus::GetStatusControls()
{
	return m_StatusControls;
}

StatusCells* GridStatus::GetStatusCells()
{
	return m_StatusCells;
}
