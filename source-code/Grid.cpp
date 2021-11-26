#include "Grid.h"

wxBEGIN_EVENT_TABLE(Grid, wxHVScrolledWindow)
	EVT_PAINT(Grid::OnPaint)
wxEND_EVENT_TABLE()


Grid::Grid(wxWindow* parent): wxHVScrolledWindow(parent)
{
	SetBackgroundColour(wxColor(255, 214, 165));
	
	SetRowColumnCount(Sizes::TOTAL_CELLS, Sizes::TOTAL_CELLS);

	SetBackgroundStyle(wxBG_STYLE_PAINT);
	
	ScrollToCenter();
}

Grid::~Grid()
{
}

int Grid::GetSize()
{
	return m_Size;
}

void Grid::SetSize(int size)
{
	m_Size = size;

	wxVarHScrollHelper::RefreshAll();
	wxVarVScrollHelper::RefreshAll();
}

void Grid::ScrollToCenter()
{
	ScrollToRowColumn(wxPosition(Sizes::TOTAL_CELLS / 2, Sizes::TOTAL_CELLS / 2));
}

wxCoord Grid::OnGetRowHeight(size_t row) const
{
	return wxCoord(m_Size);
}

wxCoord Grid::OnGetColumnWidth(size_t row) const
{
	return wxCoord(m_Size);
}

void Grid::OnDraw(wxDC& dc)
{
	dc.Clear();

	wxBrush brush = dc.GetBrush();
	wxPen pen = dc.GetPen();

	pen.SetStyle(wxPENSTYLE_SOLID);
	pen.SetColour(wxColour(200, 200, 200));

	dc.SetPen(pen);
	dc.SetBrush(brush);

	wxPosition start = GetVisibleBegin();
	wxPosition end = GetVisibleEnd();

	for (int y = start.GetRow(); y <= end.GetRow(); y++)
	{
		for (int x = start.GetCol(); x <= end.GetCol(); x++)
		{
			dc.DrawRectangle(x * m_Size, y * m_Size, m_Size, m_Size);
		}
	}
}

void Grid::OnPaint(wxPaintEvent& evt)
{
	wxBufferedPaintDC dc(this);
	this->PrepareDC(dc);
	this->OnDraw(dc);
}
