#pragma once
#include "wx/wx.h"
#include "wx/listctrl.h"

#include "Ids.h"

class InputStates : public wxPanel
{
public:
	InputStates(wxWindow* parent);
	~InputStates();

	wxListView* GetList();
private:
	wxListView* m_List = nullptr;
};

