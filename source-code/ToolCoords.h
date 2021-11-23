#pragma once
#include "wx/wx.h"

#include "IDs.h"

class ToolCoords
{
public:
	ToolCoords(wxWindow* parent, wxBoxSizer* sizer);
	~ToolCoords();

	void SetCoords(int x, int y);
private:
	wxStaticText* m_Coords = nullptr;
};

