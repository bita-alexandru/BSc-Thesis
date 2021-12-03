#pragma once
#include "wx/wx.h"

#include "Ids.h"
#include "Sizes.h"

class ToolCoords : public wxPanel
{
public:
	ToolCoords(wxWindow* parent);
	~ToolCoords();

	void SetCoords(int x, int y);
private:
	wxStaticText* m_Coords = nullptr;

	void BuildInterface();
};

