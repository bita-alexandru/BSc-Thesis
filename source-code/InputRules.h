#pragma once
#include "wx/wx.h"
#include "wx/listctrl.h"

#include "Ids.h"

class InputRules : public wxPanel
{
public:
	InputRules(wxWindow* parent);
	~InputRules();

	wxListView* GetList();
private:
	wxListView* m_List = nullptr;
};

