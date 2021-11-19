#pragma once
#include "wx/wx.h"
#include "wx/listctrl.h"

class InputNeighbors
{
public:
	InputNeighbors(wxScrolledWindow* parent, wxStaticBoxSizer* sizer);
	~InputNeighbors();

	enum _IDs
	{
		ID_EDIT_NEIGHBORS = 10200,
		ID_DELETE_NEIGHBOR,
		ID_GOTO_NEIGHBOR,
		ID_LIST_NEIGHBORS,
	};

	wxListView* GetList();
private:
	wxListView* m_List = nullptr;
};

