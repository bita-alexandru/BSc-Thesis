#pragma once
#include "wx/wx.h"

#include "Ids.h"

class StatusControls
{
public:
	StatusControls(wxWindow* parent, wxBoxSizer* sizer);
	~StatusControls();

	char GetStartState();
	void SetStartState(char state);
private:
	char m_StartState = 'S';
	wxButton* m_StartButton = nullptr;
};

