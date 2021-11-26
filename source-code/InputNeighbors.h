#pragma once
#include "wx/wx.h"
#include "wx/listctrl.h"

#include "Ids.h"

class InputNeighbors
{
public:
	InputNeighbors(wxScrolledWindow* parent, wxStaticBoxSizer* sizer);
	~InputNeighbors();

	wxListView* GetList();
private:
	wxListView* m_List = nullptr;
};

