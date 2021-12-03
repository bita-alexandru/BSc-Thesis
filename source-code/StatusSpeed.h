#pragma once
#include "wx/wx.h"

#include "Ids.h"

class StatusSpeed : public wxPanel
{
public:
	StatusSpeed(wxWindow* parent);
	~StatusSpeed();

	int GetDelay();
	void SetDelay(char mode);
private:
	float m_Delay = 0.125;
	float m_MaximumDelay = 2.0;
	float m_MinimumDelay = 0.03125;

	wxStaticText* m_TextDelay = nullptr;

	void BuildInterface();
	void UpdateDelay();
};

