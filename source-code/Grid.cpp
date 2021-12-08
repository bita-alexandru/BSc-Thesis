#include "Grid.h"

#define cout(x) wxMessageBox(x, "debug")

wxBEGIN_EVENT_TABLE(Grid, wxHVScrolledWindow)
	EVT_PAINT(Grid::OnPaint)
	EVT_MOUSE_EVENTS(Grid::OnMouse)
	EVT_TIMER(Ids::ID_TIMER_SELECTION, Grid::OnTimer)
wxEND_EVENT_TABLE()


Grid::Grid(wxWindow* parent): wxHVScrolledWindow(parent)
{
    SetBackgroundColour(wxColor(255, 214, 165));

	SetRowColumnCount(Sizes::TOTAL_CELLS, Sizes::TOTAL_CELLS);

	SetBackgroundStyle(wxBG_STYLE_PAINT);

	m_Timer = new wxTimer(this, Ids::ID_TIMER_SELECTION);
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

	wxPosition visibleBegin = GetVisibleBegin();
	wxPosition visibleEnd = GetVisibleEnd();

	int x = visibleEnd.GetCol() - (visibleEnd.GetCol() - visibleBegin.GetCol() - 1) / 2;
	int y = visibleEnd.GetRow() - (visibleEnd.GetRow() - visibleBegin.GetRow() + 1) / 2;

	wxVarHScrollHelper::RefreshAll();
	wxVarVScrollHelper::RefreshAll();

	ScrollToCenter(x , y);
}

void Grid::ScrollToCenter(int x = Sizes::TOTAL_CELLS/2, int y = Sizes::TOTAL_CELLS/2)
{
	wxPosition visibleBegin = GetVisibleBegin();
	wxPosition visibleEnd = GetVisibleEnd();

	int row = y;
	int column = x;

	row = row - (visibleEnd.GetRow() - visibleBegin.GetRow()) / 2;
	column = column - (visibleEnd.GetCol() - visibleBegin.GetCol()) / 2;

	row = std::max(0, row); row = std::min(Sizes::TOTAL_CELLS - 1, row);
	column = std::max(0, column); column = std::min(Sizes::TOTAL_CELLS - 1, column);

	ScrollToRowColumn(row, column);
}

void Grid::SetToolZoom(ToolZoom* toolZoom)
{
	m_ToolZoom = toolZoom;
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

void Grid::InsertCell(int x, int y, std::string state, wxColour color)
{
	// current cell is of state "FREE" but there's already a cell of another state
	// on this exact position -> remove it
	if (m_Cells.find({ x, y }) != m_Cells.end() && color == wxColour("white"))
	{
		RemoveCell(x, y, state, color);
	}
	// position is available
	else
	{
		m_Cells[{x, y}] = { state, color };
		m_StatePositions[state].insert({ x,y });
	}
}

void Grid::RemoveCell(int x, int y, std::string state, wxColour color)
{
	if (m_Cells.find({ x, y }) != m_Cells.end())
	{
		m_Cells.erase({ x,y });
		m_StatePositions[state].erase({ x,y });

		// there are no more cells of this state anymore -> remove it from our map
		if (m_StatePositions[state].size() == 0)
		{
			m_StatePositions.erase(state);
		}
	}
}

void Grid::RemoveState(std::string state, wxColour color)
{
	// cells of this state have been placed on the grid
	if (m_StatePositions.find(state) != m_StatePositions.end())
	{
		// remove every cell of this state from our map
		for (auto it : m_StatePositions[state])
		{
			// remove from m_Cells, one by one
			m_Cells.erase(it);
			Refresh(false);
		}

		// remove the corresponding map
		m_StatePositions.erase(state);
	}
}

void Grid::UpdateState(std::string oldState, wxColour oldColor, std::string newState, wxColour newColor)
{
	// cells of this state need their names & colors updated
	if (m_StatePositions.find(oldState) != m_StatePositions.end())
	{
		// same state but a new color
		if (oldState == newState)
		{
			for (auto it : m_StatePositions[oldState])
			{
				m_Cells[it].second = newColor;
				Refresh(false);
			}
		}
		// just a regular update
		else
		{
			// update both the name and the color + make a new map
			for (auto it : m_StatePositions[oldState])
			{
				m_Cells[it] = { newState, newColor };
				Refresh(false);
				m_StatePositions[newState].insert(it);
			}

			// remove the old map
			m_StatePositions.erase(oldState);
		}
	}
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
	if (m_Size == Sizes::CELL_SIZE_MIN) dc.SetPen(*wxTRANSPARENT_PEN);

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
	// gain focus on mouse hover
	if (evt.Entering())
	{
		this->SetFocus();

		return;
	}

	if (evt.Leaving())
	{
		m_ToolCoords->Reset();

		return;
	}

	// shortcut for alternating between states: ALT + L/R Click
	if (wxGetKeyState(WXK_SHIFT))
	{
		if (evt.LeftIsDown() || evt.RightIsDown())
		{
			if (!m_Timer->IsRunning()) m_Timer->Start(80);
		}

		return;
	}

	// get hovered cell information 
	wxPosition visible = GetVisibleBegin();

	int x = evt.GetX() / m_Size + visible.GetCol();
	int y = evt.GetY() / m_Size + visible.GetRow();

	// shortcut for zooming in/out with focus on hovered cell
	if (wxGetKeyState(WXK_CONTROL))
	{
		int wheelRotation = evt.GetWheelRotation();
		if (wheelRotation > 0 && m_ToolZoom->GetSize() < Sizes::CELL_SIZE_MAX)
		{
			
			ScrollToCenter(x, y);
			m_ToolZoom->ZoomIn();
		}
		if (wheelRotation < 0 && m_ToolZoom->GetSize() > Sizes::CELL_SIZE_MIN)
		{
			ScrollToCenter(x, y);
			m_ToolZoom->ZoomOut();
		}

		return;
	}
	std::string name = "FREE";

	// update coordinates displayed on screen
	if (m_Cells.find({ x, y }) != m_Cells.end())
	{
		name = m_Cells[{x, y}].first;
	}
	m_ToolCoords->Set(x - m_Offset, y - m_Offset, name);

	char mode = m_ToolModes->GetMode();

	// "draw" mode
	if (mode == 'D')
	{
		if (evt.LeftIsDown() || evt.RightIsDown())
		{
			std::pair<std::string, wxColour> state = m_ToolStates->GetState();

			// left click -> place a cell
			if (evt.LeftIsDown())
			{
				InsertCell(x, y, state.first, state.second);
			}
			// right click -> remove a cell
			else
			{
				RemoveCell(x, y, state.first, state.second);
			}

			this->Refresh(false);
		}
	}
	// "pick" mode
	else if (mode == 'P')
	{
		if (evt.LeftDown())
		{
			std::pair<std::string, wxColour> state = m_ToolStates->GetState();

			m_ToolStates->SetState(name);
			m_ToolModes->SetMode('D');
		}
		else if (evt.RightDown())
		{
			m_ToolModes->SetMode('D');
		}
	}

	evt.Skip();
}

void Grid::OnTimer(wxTimerEvent& evt)
{
	wxMouseState mouseState = wxGetMouseState();
	if (!wxGetKeyState(WXK_SHIFT) || (!mouseState.LeftIsDown() && !mouseState.RightIsDown()))
	{
		m_Timer->Stop();
		return;
	}

	if (mouseState.LeftIsDown()) m_ToolStates->SelectPrevState();
	if (mouseState.RightIsDown()) m_ToolStates->SelectNextState();
}
