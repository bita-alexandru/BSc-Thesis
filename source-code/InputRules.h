#pragma once
#include "wx/wx.h"
#include "wx/listctrl.h"

#include "IDs.h"

class InputRules
{
public:
	InputRules(wxScrolledWindow* parent, wxStaticBoxSizer* sizer);
	~InputRules();

	wxListView* GetList();
private:
	wxListView* m_List = nullptr;
};

