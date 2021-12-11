#include "Grid.h"

wxBEGIN_EVENT_TABLE(Grid, wxHVScrolledWindow)
	EVT_PAINT(Grid::OnPaint)
	EVT_MOUSE_EVENTS(Grid::OnMouse)
	EVT_TIMER(Ids::ID_TIMER_SELECTION, Grid::OnTimerSelection)
	EVT_KEY_DOWN(Grid::OnKeyDown)
wxEND_EVENT_TABLE()


Grid::Grid(wxWindow* parent): wxHVScrolledWindow(parent)
{
	BuildInterface();

	InitializeTimers();
}

Grid::~Grid()
{
	wxDELETE(m_TimerSelection);
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

void Grid::ScrollToCenter(int x, int y)
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

void Grid::SetCells(std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairHash> cells, std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairHash>> statePositions)
{

	m_Cells = cells;
	m_StatePositions = statePositions;
	m_PrevCells = m_Cells;
	m_PrevStatePositions = m_StatePositions;

	Refresh(false);
}

void Grid::SetToolZoom(ToolZoom* toolZoom)
{
	m_ToolZoom = toolZoom;
}

void Grid::SetToolUndo(ToolUndo* toolUndo)
{
	m_ToolUndo = toolUndo;
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
	else if (color != wxColour("white"))
	{
		m_Cells[{x, y}] = { state, color };
		m_StatePositions[state].insert({ x,y });

		Refresh(false);
		//wxClientDC dc(this);
		//DrawCell(dc, x, y, color);
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

		Refresh(false);
		//wxClientDC dc(this);
		//DrawCell(dc, x, y, wxColour("white"));
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

void Grid::OnPaint(wxPaintEvent& evt)
{
	wxBufferedPaintDC dc(this);
	PrepareDC(dc);
	OnDraw(dc);
}

void Grid::BuildInterface()
{
	SetBackgroundColour(wxColor(255, 214, 165));

	SetRowColumnCount(Sizes::TOTAL_CELLS, Sizes::TOTAL_CELLS);

	SetBackgroundStyle(wxBG_STYLE_PAINT);
}

void Grid::InitializeTimers()
{
	m_TimerSelection = new wxTimer(this, Ids::ID_TIMER_SELECTION);
}

std::pair<int, int> Grid::GetHoveredCell(int X, int Y)
{
	// get hovered cell information 
	wxPosition visible = GetVisibleBegin();
	
	int x = X / m_Size + visible.GetCol();
	int y = Y / m_Size + visible.GetRow();

	return { x, y };
}

bool Grid::ControlSelectState(wxMouseEvent& evt)
{
	// shortcut for alternating between states: ALT + L/R Click
	if (wxGetKeyState(WXK_SHIFT))
	{
		if (evt.LeftIsDown() || evt.RightIsDown())
		{
			if (!m_TimerSelection->IsRunning()) m_TimerSelection->Start(80);
		}

		return true;
	}

	return false;
}

bool Grid::ControlZoom(wxMouseEvent& evt, int x, int y)
{
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

		return true;
	}

	return false;
}

std::string Grid::ControlUpdateCoords(int x, int y)
{
	std::string name = "FREE";

	// update coordinates displayed on screen
	if (m_Cells.find({ x, y }) != m_Cells.end())
	{
		name = m_Cells[{x, y}].first;
	}

	m_ToolCoords->Set(x - m_Offset, y - m_Offset, name);

	return name;
}

bool Grid::ModeDraw(wxMouseEvent& evt, int x, int y, char mode)
{
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
			else if (evt.RightIsDown())
			{
				RemoveCell(x, y, state.first, state.second);
			}
		}
		else if (evt.LeftUp() || evt.RightUp())
		{
			if (m_Cells != m_PrevCells)
			{
				m_ToolUndo->PushBack(m_Cells, m_StatePositions, m_PrevCells, m_PrevStatePositions);
				m_PrevCells = m_Cells;
				m_PrevStatePositions = m_StatePositions;
			}
		}

		return true;
	}

	return false;
}

bool Grid::ModePick(wxMouseEvent& evt, int x, int y, char mode, std::string state)
{
	// "pick" mode
	if (mode == 'P')
	{
		if (evt.LeftDown())
		{
			m_ToolStates->SetState(state);
			m_ToolModes->SetMode('D');
		}
		else if (evt.RightDown())
		{
			m_ToolModes->SetMode('D');
		}

		return true;
	}

	return false;
}

bool Grid::ModeMove(wxMouseEvent& evt, int x, int y, char mode)
{
	if ((mode == 'M' && evt.LeftDown()) || evt.MiddleDown())
	{
		SetFocus();

		m_PrevCell = { x,y };

		return true;
	}

	if ((mode == 'M' && evt.LeftIsDown()) || evt.MiddleIsDown())
	{
		if (m_PrevCell.first != x || m_PrevCell.second != y)
		{
			int deltaX = m_PrevCell.first - x;
			int deltaY = m_PrevCell.second - y;

			if (deltaX)
			{
				m_PrevCell.first = x;

				if (m_PrevScrolledCol) m_PrevScrolledCol = false;
				else
				{
					ScrollColumns(deltaX);
					m_PrevScrolledCol = true;
				}
			}
			if (deltaY)
			{
				m_PrevCell.second = y;

				if (m_PrevScrolledRow) m_PrevScrolledRow = false;
				else
				{
					ScrollRows(deltaY);
					m_PrevScrolledRow = true;
				}
			}
		}

		return true;
	}

	if (mode == 'M' && evt.RightDown())
	{
		m_ToolModes->SetMode('D');
		return true;
	}

	return false;
}

void Grid::DrawCell(wxDC& dc, int x, int y, wxColour color)
{
	wxBrush brush = dc.GetBrush();
	wxPen pen = dc.GetPen();
	if (m_Size == Sizes::CELL_SIZE_MIN) dc.SetPen(*wxTRANSPARENT_PEN);

	brush.SetColour(color);

	pen.SetStyle(wxPENSTYLE_SOLID);
	pen.SetColour(wxColour(200, 200, 200));

	dc.SetPen(pen);
	dc.SetBrush(brush);

	wxRect rect(x * m_Size, y * m_Size, m_Size, m_Size);
	dc.DrawRectangle(rect);

	//RefreshRect(rect, false);
	//Update();
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

	//for (int y = visibleBegin.GetRow(); y < visibleEnd.GetRow(); y++)
	//	for (int x = visibleBegin.GetCol(); x < visibleEnd.GetCol(); x++)
	//		dc.DrawRectangle(x * m_Size, y * m_Size, m_Size, m_Size);

	//for (auto it : m_Cells)
	//{
	//	int x = it.first.first;
	//	int y = it.first.second;

	//	wxColour color = it.second.second;

	//	brush.SetColour(color);
	//	dc.SetBrush(brush);

	//	dc.DrawRectangle(x * m_Size, y * m_Size, m_Size, m_Size);
	//}

	for (int y = visibleBegin.GetRow(); y < visibleEnd.GetRow(); y++)
	{
		for (int x = visibleBegin.GetCol(); x < visibleEnd.GetCol(); x++)
		{
			if (m_Cells.find({ x, y }) != m_Cells.end())
			{
				wxColour color = m_Cells[{x, y}].second;
				brush.SetColour(color);
				dc.SetBrush(brush);

				//DrawCell(dc, x, y, color);

				dc.DrawRectangle(x * m_Size, y * m_Size, m_Size, m_Size);

				brush.SetColour(wxColour("white"));
				dc.SetBrush(brush);
				continue;
			}
			//DrawCell(dc, x, y, wxColour("white"));
			dc.DrawRectangle(x * m_Size, y * m_Size, m_Size, m_Size);
		}
	}

	if (!m_Centered)
	{
		m_Centered = true;
		ScrollToCenter();
	}
}

void Grid::OnMouse(wxMouseEvent& evt)
{
	if (evt.Leaving())
	{
		m_ToolCoords->Reset();

		return;
	}

	if (ControlSelectState(evt)) return;

	m_MouseXY = { evt.GetX(), evt.GetY() };

	std::pair<int, int> hoveredCell = GetHoveredCell(evt.GetX(), evt.GetY());
	int x = hoveredCell.first;
	int y = hoveredCell.second;

	if (ControlZoom(evt, x, y)) return;

	std::string state = ControlUpdateCoords(x, y);

	char mode = m_ToolModes->GetMode();
	
	if (ModeMove(evt, x, y, mode))
	{
		evt.Skip();
		return;
	}
	if (ModeDraw(evt, x, y, mode))
	{
		evt.Skip();
		return;
	}
	if (ModePick(evt, x, y, mode, state))
	{
		evt.Skip();
		return;
	}
}

void Grid::OnTimerSelection(wxTimerEvent& evt)
{
	wxMouseState mouseState = wxGetMouseState();
	if (!wxGetKeyState(WXK_SHIFT) || (!mouseState.LeftIsDown() && !mouseState.RightIsDown()))
	{
		m_TimerSelection->Stop();
		return;
	}

	if (mouseState.LeftIsDown()) m_ToolStates->SelectPrevState();
	if (mouseState.RightIsDown()) m_ToolStates->SelectNextState();
}

void Grid::OnKeyDown(wxKeyEvent& evt)
{
	if (evt.GetKeyCode() == (int)'W')
	{
		ScrollRows(-1);
	}
	else if (evt.GetKeyCode() == (int)'A')
	{
		ScrollColumns(-1);
	}
	else if (evt.GetKeyCode() == (int)'S')
	{
		ScrollRows(1);
	}
	else if (evt.GetKeyCode() == (int)'D')
	{
		ScrollColumns(1);
	}
	else
	{
		evt.Skip();
		return;
	}

	if (m_ToolModes->GetMode() == 'D')
	{
		wxMouseState mouseState = wxGetMouseState();

		std::pair<int, int> hoveredCell = GetHoveredCell(m_MouseXY.first, m_MouseXY.second);
		int x = hoveredCell.first;
		int y = hoveredCell.second;

		ControlUpdateCoords(x, y);

		std::pair<std::string, wxColour> state = m_ToolStates->GetState();
		if (mouseState.LeftIsDown()) InsertCell(x, y, state.first, state.second);
		else if (mouseState.RightIsDown()) RemoveCell(x, y, state.first, state.second);
	}

	evt.Skip();
}
