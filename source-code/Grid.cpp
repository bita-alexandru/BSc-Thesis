#include "Grid.h"

#define cout(x) wxMessageBox(x, "debug")

wxBEGIN_EVENT_TABLE(Grid, wxHVScrolledWindow)
	EVT_PAINT(Grid::OnPaint)
	EVT_MOUSE_EVENTS(Grid::OnMouse)
wxEND_EVENT_TABLE()


Grid::Grid(wxWindow* parent): wxHVScrolledWindow(parent)
{
    SetBackgroundColour(wxColor(255, 214, 165));

	SetRowColumnCount(Sizes::TOTAL_CELLS, Sizes::TOTAL_CELLS);

	SetBackgroundStyle(wxBG_STYLE_PAINT);
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
	wxPosition visibleBegin = GetVisibleBegin();
	wxPosition visibleEnd = GetVisibleEnd();

	int row = Sizes::TOTAL_CELLS / 2;
	int column = Sizes::TOTAL_CELLS / 2;

	int a = visibleEnd.GetCol();
	int b = visibleBegin.GetCol();

	row = row - (visibleEnd.GetRow() - visibleBegin.GetRow()) / 2;
	column = column - (visibleEnd.GetCol() - visibleBegin.GetCol()) / 2;

	ScrollToRowColumn(row, column);
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

	brush.SetColour(wxColour("white"));

	dc.SetPen(pen);

	wxPosition visibleBegin = GetVisibleBegin();
	wxPosition visibleEnd = GetVisibleEnd();

	for (int y = visibleBegin.GetRow(); y < visibleEnd.GetRow(); y++)
	{
		for (int x = visibleBegin.GetCol(); x < visibleEnd.GetCol(); x++)
		{
			if (m_Cells.find({ x, y }) != m_Cells.end())
			{
				wxColour color = m_Cells[{x, y}].second;
				brush.SetColour(color);
				dc.SetBrush(brush);

				dc.DrawRectangle(x * m_Size, y * m_Size, m_Size, m_Size);

				brush.SetColour(wxColour("white"));
				dc.SetBrush(brush);
				continue;
			}

			dc.DrawRectangle(x * m_Size, y * m_Size, m_Size, m_Size);
		}
	}

	if (!m_Centered)
	{
		ScrollToCenter();
		m_Centered = true;
	}
}

void Grid::OnPaint(wxPaintEvent& evt)
{
	wxBufferedPaintDC dc(this);
	this->PrepareDC(dc);
	this->OnDraw(dc);
}

void Grid::OnMouse(wxMouseEvent& evt)
{
	if (evt.Leaving())
	{
		m_ToolCoords->Reset();
		return;
	}

	wxPosition visible = GetVisibleBegin();

	int x = evt.GetX() / m_Size + visible.GetCol();
	int y = evt.GetY() / m_Size + visible.GetRow();

	if (m_Cells.find({ x, y }) != m_Cells.end())
	{
		m_ToolCoords->Set(x - m_Offset, y - m_Offset, m_Cells[{x, y}].first);
	}
	else
	{
		m_ToolCoords->Set(x - m_Offset, y - m_Offset, "FREE");
	}

	if (evt.LeftIsDown() || evt.RightIsDown())
	{
		/*std::string msg = std::to_string(x-200) + "," + std::to_string(y-200) + "\n";
		cout(msg);*/

		std::pair<std::string, wxColour> state = m_ToolStates->GetState();

		if (evt.LeftIsDown())
		{
			if (m_Cells.find({ x, y }) != m_Cells.end() && state.second == wxColour("white"))
			{
				m_Cells.erase({ x, y });
			}
			else
			{
				m_Cells[{x, y}] = state;
			}
		}
		else
		{
			if (m_Cells.find({ x, y }) != m_Cells.end())
			{
				m_Cells.erase({ x, y });
			}
		}

		this->Refresh(false);
	}

	evt.Skip();
}
