#pragma once
#include "wx/wx.h"

class StatusControls
{
public:
	StatusControls(wxWindow* parent, wxBoxSizer* sizer);
	~StatusControls();

	enum _IDs
	{
		ID_BUTTON_START = 11300, ID_BUTTON_RESET,
		ID_BUTTON_STEP, ID_BUTTON_GENERATION
	};

	char GetStartState();
	void SetStartState(char state);
private:
	char m_StartState = 'S';
	wxButton* m_StartButton = nullptr;
};

