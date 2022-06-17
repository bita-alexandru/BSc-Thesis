#include "PanelGrid.h"
#include "Colors.h"

PanelGrid::PanelGrid(wxWindow* parent) : wxPanel(parent)
{
    SetBackgroundColour(wxColor(Colors::COLOR_MAIN_R, Colors::COLOR_MAIN_G, Colors::COLOR_MAIN_B));

	BuildInterface();
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

void PanelGrid::BuildInterface()
{
	m_GridTools = new GridTools(this);
	m_Grid = new Grid(this);
	m_GridStatus = new GridStatus(this);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(m_GridTools, 0, wxEXPAND | wxLEFT, 6);
	sizer->Add(m_Grid, 1, wxEXPAND | wxLEFT | wxRIGHT, 6);
	sizer->Add(m_GridStatus, 0, wxEXPAND | wxLEFT, 6);

	SetSizer(sizer);
}
