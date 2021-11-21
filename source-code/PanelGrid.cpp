#include "PanelGrid.h"

PanelGrid::PanelGrid(wxWindow* parent): wxPanel(parent)
{
	SetBackgroundColour(wxColor(190, 225, 230));
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
