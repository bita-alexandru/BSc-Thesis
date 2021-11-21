#pragma once
#include "wx/wx.h"

#include "GridTools.h"
#include "Grid.h"
#include "GridStatus.h"

class PanelGrid: wxPanel
{
public:
	PanelGrid(wxWindow* parent);
	~PanelGrid();

	GridTools* GetGridTools();
	Grid* GetGrid();
	GridStatus* GetGridStatus();
private:
	GridTools* m_GridTools = nullptr;
	Grid* m_Grid = nullptr;
	GridStatus* m_GridStatus = nullptr;
};

