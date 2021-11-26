#pragma once
#include "wx/wx.h"
#include "wx/listctrl.h"

#include "Constants.h"

class InputStates
{
public:
	InputStates(wxScrolledWindow* parent, wxStaticBoxSizer* sizer);
	~InputStates();

	wxListView* GetList();
private:
	wxListView* m_List = nullptr;
};

