#include "PanelGrid.h"

PanelGrid::PanelGrid(wxWindow* parent) : wxPanel(parent)
{
	SetBackgroundColour(wxColor(190, 225, 230));

	m_GridTools = new GridTools(this);
	m_Grid = new Grid(this);
	m_GridStatus = new GridStatus(this);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(m_GridTools, 1);
	sizer->Add(m_Grid, 14, wxEXPAND);
	sizer->Add(m_GridStatus, 1);

	this->SetSizer(sizer);
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
