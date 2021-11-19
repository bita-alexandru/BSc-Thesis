#pragma once
#include "wx/wx.h"

class MenuBar: wxMenuBar
{
public:
	MenuBar();
	~MenuBar();

	enum _IDs
	{
		ID_OPEN_C = 10001, ID_OPEN_G,
		ID_SAVE_C, ID_SAVE_G,
		ID_EXIT,
		ID_RESET_C, ID_RESET_G,
		ID_DOCUMENTATION,
	};
};

