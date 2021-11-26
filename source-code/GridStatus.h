#pragma once
#include "wx/wx.h"

#include "StatusSpeed.h"
#include "StatusControls.h"
#include "StatusCells.h"

class GridStatus: public wxPanel
{
public:
	GridStatus(wxWindow* parent);
	~GridStatus();

	StatusSpeed* GetStatusSpeed();
	StatusControls* GetStatusControls();
	StatusCells* GetStatusCells();
private:
	StatusSpeed* m_StatusSpeed = nullptr;
	StatusControls* m_StatusControls = nullptr;
	StatusCells* m_StatusCells = nullptr;
};

