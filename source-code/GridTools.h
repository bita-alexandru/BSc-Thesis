#pragma once
#include "wx/wx.h"

#include "ToolZoom.h"
#include "ToolModes.h"
#include "ToolCoords.h"

class GridTools: wxPanel
{
public:
	GridTools(wxWindow* parent);
	~GridTools();
	
	ToolZoom* GetToolZoom();
	ToolModes* GetToolModes();
	ToolCoords* GetToolCoords();
private:
	ToolZoom* m_ToolZoom = nullptr;
	ToolModes* m_ToolModes = nullptr;
	ToolCoords* m_ToolCoords = nullptr;
};

