#pragma once
#include "wx/wx.h"

class StatusSpeed
{
public:
	StatusSpeed(wxWindow* parent, wxBoxSizer* sizer);
	~StatusSpeed();

	enum _IDs
	{
		ID_BUTTON_FASTER = 12000, ID_BUTTON_SLOWER
	};

	int GetDelay();
	void SetDelay(char mode);
private:
	float m_Delay = 0.125;
	float m_MaximumDelay = 2.0;
	float m_MinimumDelay = 0.03125;

	wxStaticText* m_TextDelay = nullptr;

	void UpdateDelay();
};
