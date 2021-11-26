#pragma once
#include "wx/wx.h"
#include "wx/listctrl.h"

#include "Constants.h"

class InputRules
{
public:
	InputRules(wxScrolledWindow* parent, wxStaticBoxSizer* sizer);
	~InputRules();

	wxListView* GetList();
private:
	wxListView* m_List = nullptr;
};

