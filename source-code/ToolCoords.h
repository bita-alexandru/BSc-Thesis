#pragma once
#include "wx/wx.h"

#include "Ids.h"
#include "Sizes.h"

class ToolCoords : public wxPanel
{
public:
	ToolCoords(wxWindow* parent);
	~ToolCoords();

	void Set(int x, int y, std::string state);
	void Reset();
private:
	wxStaticText* m_Coords = nullptr;
	wxStaticText* m_State = nullptr;

	void BuildInterface();
};

