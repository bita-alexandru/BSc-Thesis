#include "PanelGrid.h"

PanelGrid::PanelGrid(wxWindow* parent) : wxPanel(parent)
{
    SetBackgroundColour(wxColor(220, 220, 220));

	m_GridTools = new GridTools(this);
	m_Grid = new Grid(this);
	m_GridStatus = new GridStatus(this);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(m_GridTools, 0, wxEXPAND);
	sizer->Add(m_Grid, 1, wxEXPAND);
	sizer->Add(m_GridStatus, 0, wxEXPAND);

	SetSizer(sizer);
}

PanelGrid::~PanelGrid()
{
}

GridTools* PanelGrid::GetGridTools()
{
	return m_GridTools;
}

Grid* PanelGrid::GetGrid()
{
	return m_Grid;
}

GridStatus* PanelGrid::GetGridStatus()
{
	return m_GridStatus;
}
