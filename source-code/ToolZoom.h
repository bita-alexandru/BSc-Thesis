#pragma once
#include "wx/wx.h"

#include "IDs.h"

class ToolZoom
{
public:
	ToolZoom(wxWindow* parent, wxBoxSizer* sizer);
	~ToolZoom();

	int GetSize();
	void SetSize(char mode);
private:
	int m_Size = 16;
	int m_MaximumSize = 32;
	int m_MinimumSize = 1;

	wxStaticText* m_TextScale = nullptr;

	void UpdateTextScale();
};

