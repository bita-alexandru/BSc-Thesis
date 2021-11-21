#pragma once
#include "wx/wx.h"

class ToolZoom
{
public:
	ToolZoom(wxWindow* parent, wxBoxSizer* sizer);
	~ToolZoom();

	enum _IDs
	{
		ID_ZOOM_IN = 11000, ID_ZOOM_OUT
	};

	int GetSize();
	void SetSize(char mode);
private:
	int m_Size = 16;
	int m_MaximumSize = 32;
	int m_MinimumSize = 1;

	wxStaticText* m_TextScale = nullptr;

	void UpdateTextScale();
};

