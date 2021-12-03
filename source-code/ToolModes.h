#pragma once
#include "wx/wx.h"

class ToolModes : public wxPanel
{
public:
	ToolModes(wxWindow* parent);
	~ToolModes();

	int GetMode();
	int GetIndex();
	void SetIndex(char direction = NULL, int index = NULL);
private:
	int m_Mode = 0;
	int m_Index = 1;
	int m_MaximumIndex = 1;
	wxButton* m_StateColor = nullptr;
	wxStaticText* m_TextIndex = nullptr;

	void BuildInterface();
	void UpdateTextIndex();
	void UpdateStateColor();
};

