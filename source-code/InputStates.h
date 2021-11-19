#pragma once
#include "wx/wx.h"
#include "wx/listctrl.h"

class InputStates
{
public:
	InputStates(wxScrolledWindow* parent, wxStaticBoxSizer* sizer);
	~InputStates();

	enum _IDs
	{
		ID_EDIT_STATES = 10100,
		ID_DELETE_STATE,
		ID_GOTO_STATE,
		ID_LIST_STATES
	};

	wxListView* GetList();
private:
	wxListView* m_List = nullptr;
};

