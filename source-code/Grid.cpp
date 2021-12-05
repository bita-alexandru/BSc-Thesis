#include "Grid.h"

#define cout(x) wxMessageBox(x, "debug")

wxBEGIN_EVENT_TABLE(Grid, wxHVScrolledWindow)
	EVT_PAINT(Grid::OnPaint)
	EVT_MOUSE_EVENTS(Grid::OnMouseLeftRight)
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

void Grid::SetToolModes(ToolModes* toolModes)
{
	m_ToolModes = toolModes;
}

void Grid::SetToolStates(ToolStates* toolStates)
{
	m_ToolStates = toolStates;
}

void Grid::SetToolCoords(ToolCoords* toolCoords)
{
	m_ToolCoords = toolCoords;
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

	brush.SetColour(wxColour("#FFFFFF"));

	dc.SetPen(pen);

	wxPosition start = GetVisibleBegin();
	wxPosition end = GetVisibleEnd();

	for (int y = start.GetRow(); y <= end.GetRow(); y++)
	{
		for (int x = start.GetCol(); x <= end.GetCol(); x++)
		{
			if (m_Cells.find({ x, y }) != m_Cells.end())
			{
				wxColour color = m_Cells[{x, y}].second;
				brush.SetColour(color);
				dc.SetBrush(brush);

				dc.DrawRectangle(x * m_Size, y * m_Size, m_Size, m_Size);

				brush.SetColour(wxColour("#FFFFFF"));
				dc.SetBrush(brush);
				continue;
			}

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

void Grid::OnMouseLeftRight(wxMouseEvent& evt)
{
	if (evt.LeftIsDown() || evt.RightIsDown())
	{

		wxPosition visible = GetVisibleBegin();

		int x = evt.GetX() / m_Size + visible.GetCol();
		int y = evt.GetY() / m_Size + visible.GetRow();

		std::pair<std::string, wxColour> state = m_ToolStates->GetState();

		if (evt.LeftIsDown())
		{
			if (m_Cells.find({ x, y }) != m_Cells.end() && state.second == wxColour("#FFFFFF"))
			{
				m_Cells.erase({ x, y });
			}
			else
			{
				m_Cells[{x, y}] = state;
			}
			
			this->Refresh(false);
			
			evt.Skip();
			return;
		}

		if (m_Cells.find({ x, y }) != m_Cells.end())
		{
			m_Cells.erase({ x, y });
		}

		this->Refresh(false);
		evt.Skip();
		return;
	}

	evt.Skip();
}
