#pragma once
#include "wx/wx.h"
#include "wx/listctrl.h"

class InputRules
{
public:
	InputRules(wxScrolledWindow* parent, wxStaticBoxSizer* sizer);
	~InputRules();

	enum _IDs
	{
		ID_EDIT_RULES = 10300,
		ID_DELETE_RULE,
		ID_GOTO_RULE,
		ID_LIST_RULES
	};

	wxListView* GetList();
private:
	wxListView* m_List = nullptr;
};

