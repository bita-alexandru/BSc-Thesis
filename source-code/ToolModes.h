#pragma once
#include "wx/wx.h"

class ToolModes
{
public:
	ToolModes(wxWindow* parent, wxBoxSizer* sizer);
	~ToolModes();

	enum _IDs
	{
		ID_MODE_DRAW = 11100, ID_MODE_PICK, ID_MODE_DRAG,
		ID_BUTTON_PREV, ID_BUTTON_NEXT
	};

	int GetMode();
	int GetIndex();
	void SetIndex(char direction = NULL, int index = NULL);
private:
	int m_Mode = 0;
	int m_Index = 1;
	int m_MaximumIndex = 1;
	wxButton* m_StateColor = nullptr;
	wxStaticText* m_TextIndex = nullptr;

	void UpdateTextIndex();
	void UpdateStateColor();
};

