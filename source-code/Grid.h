#pragma once
#include "wx/wx.h"
#include "wx/vscroll.h"
#include "wx/dcclient.h"
#include "wx/dcmemory.h"
#include "wx/dcbuffer.h"

#include "Ids.h"
#include "Sizes.h"

class Grid: public wxHVScrolledWindow
{
public:
	Grid(wxWindow* parent);
	~Grid();

	int GetSize();
	void SetSize(int size);
	void ScrollToCenter();
private:
	int m_Size = Sizes::CELL_SIZE_DEFAULT;

	virtual wxCoord OnGetRowHeight(size_t row) const;
	virtual wxCoord OnGetColumnWidth(size_t row) const;

	void OnDraw(wxDC& dc);
	void OnPaint(wxPaintEvent& evt);

	wxDECLARE_EVENT_TABLE();
};

