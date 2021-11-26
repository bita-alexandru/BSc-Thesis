#pragma once
#include "wx/wx.h"

#include "Ids.h"
#include "Sizes.h"
#include "Grid.h"

class ToolZoom
{
public:
	ToolZoom(wxWindow* parent, wxBoxSizer* sizer);
	~ToolZoom();

	int GetSize();
	void SetSize(char mode, Grid* grid);
private:
	int m_Size = Sizes::CELL_SIZE_DEFAULT;
	int m_MaximumSize = Sizes::CELL_SIZE_MAX;
	int m_MinimumSize = Sizes::CELL_SIZE_MIN;

	wxStaticText* m_TextScale = nullptr;
};

