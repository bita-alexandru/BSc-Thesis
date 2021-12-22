#include "Grid.h"

wxBEGIN_EVENT_TABLE(Grid, wxHVScrolledWindow)
	EVT_PAINT(Grid::OnPaint)
	EVT_MOUSE_EVENTS(Grid::OnMouse)
	EVT_TIMER(Ids::ID_TIMER_SELECTION, Grid::OnTimerSelection)
	EVT_KEY_DOWN(Grid::OnKeyDown)
	EVT_ERASE_BACKGROUND(Grid::OnEraseBackground)
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
	for (auto it : m_PrevCells)
	{
		m_RedrawAll = false;
		m_RedrawXYs.push_back({ it.first.first,it.first.second });
		m_RedrawColors.push_back(wxColour("white"));
	}

	m_Cells = cells;
	m_StatePositions = statePositions;
	m_PrevCells = m_Cells;
	m_PrevStatePositions = m_StatePositions;

	for (auto it : m_Cells)
	{
		m_RedrawAll = false;
		m_RedrawXYs.push_back({ it.first.first,it.first.second });
		m_RedrawColors.push_back(it.second.second);
	}
	Refresh(false);
	Update();
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
	if (m_Cells.find({ x, y }) != m_Cells.end())
	{
		// current cell is of state "FREE" but there's already a cell of another state
		// on this exact position -> remove it
		if (color == wxColour("white")) RemoveCell(x, y, state, color);
		// position is occupied
		else
		{
			EraseCell(x, y);

			m_Cells[{x, y}] = { state, color };
			m_StatePositions[state].insert({ x,y });

			m_RedrawAll = false;
			m_RedrawXY = { x,y };
			m_RedrawColor = color;
			Refresh(false);
			Update();
		}
	}
	// position is available
	else if (color != wxColour("white"))
	{
		m_Cells[{x, y}] = { state, color };
		m_StatePositions[state].insert({ x,y });

		m_RedrawAll = false;
		m_RedrawXY = { x,y };
		m_RedrawColor = color;
		Refresh(false);
		Update();
	}
}

void Grid::RemoveCell(int x, int y, std::string state, wxColour color)
{
	if (m_Cells.find({ x, y }) != m_Cells.end())
	{
		EraseCell(x, y);
		Refresh(false);
		Update();
	}
}

void Grid::RemoveState(std::string state)
{
	// cells of this state have been placed on the grid
	if (m_StatePositions.find(state) != m_StatePositions.end())
	{

		// remove every cell of this state from our map
		for (auto it : m_StatePositions[state])
		{
			// remove from m_Cells, one by one
			m_Cells.erase(it);

			m_RedrawAll = false;
			m_RedrawXYs.push_back({ it.first,it.second });
			m_RedrawColors.push_back(wxColour("white"));
		}
		Refresh(false);
		Update();

		// remove the corresponding map
		m_StatePositions.erase(state);

		m_ToolUndo->Reset();
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

				m_RedrawAll = false;
				m_RedrawXYs.push_back({ it.first,it.second });
				m_RedrawColors.push_back(newColor);
			}
			Refresh(false);
			Update();
		}
	}
}

void Grid::EraseCell(int x, int y, bool multiple)
{
	std::string state = m_Cells[{x, y}].first;

	m_Cells.erase({ x,y });
	m_StatePositions[state].erase({ x,y });

	// there are no more cells of this state anymore -> remove it from our map
	if (m_StatePositions[state].size() == 0)
	{
		m_StatePositions.erase(state);
	}

	m_RedrawAll = false;

	if (multiple)
	{
		m_RedrawXYs.push_back({ x,y });
		m_RedrawColors.push_back(wxColour("white"));
	}
	else
	{
		m_RedrawXY = { x,y };
		m_RedrawColor = wxColour("white");
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
	// shortcut for alternating between states: Shift + L/R Click
	if ((evt.LeftIsDown() || evt.RightIsDown()) && wxGetKeyState(WXK_SHIFT))
	{
		if (!m_TimerSelection->IsRunning()) m_TimerSelection->Start(80);

		return true;
	}

	return false;
}

bool Grid::ControlZoom(wxMouseEvent& evt, int x, int y)
{
	// shortcut for zooming in/out with focus on hovered cell
	int wheelRotation = evt.GetWheelRotation();

	if (wheelRotation && wxGetKeyState(WXK_CONTROL))
	{
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
		// left click -> place a cell
		if (evt.LeftIsDown())
		{
			std::pair<std::string, wxColour> state = m_ToolStates->GetState();
			InsertCell(x, y, state.first, state.second);
		}
		// right click -> remove a cell
		else if (evt.RightIsDown())
		{
			if (m_Cells.find({ x,y }) == m_Cells.end()) return false;

			std::pair<std::string, wxColour> state = m_Cells[{x, y}];

			if (wxGetKeyState(WXK_CONTROL))
			{
				if (wxGetKeyState(WXK_ALT))
				{
					DeleteStructure(x, y, std::unordered_set<std::pair<int, int>, Hashes::PairHash>(), "");
					Refresh(false);
					Update();
				}
				else
				{
					DeleteStructure(x, y, std::unordered_set<std::pair<int, int>, Hashes::PairHash>(), state.first);
					Refresh(false);
					Update();
				}
			}

			RemoveCell(x, y, state.first, state.second);
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

void Grid::OnDraw(wxDC& dc)
{
	wxBrush brush = dc.GetBrush();
	wxPen pen = dc.GetPen();

	pen.SetStyle(wxPENSTYLE_SOLID);
	pen.SetColour(wxColour(200, 200, 200));

	dc.SetPen(pen);
	if (m_Size == Sizes::CELL_SIZE_MIN) dc.SetPen(*wxTRANSPARENT_PEN);

	if (m_JustScrolled) m_JustScrolled = false;
	else if (!m_RedrawAll)
	{
		m_RedrawAll = true;

		// more than 1 cell to be redrawn
		if (m_RedrawXYs.size())
		{
			for (int i = 0; i < m_RedrawXYs.size(); i++)
			{
				brush.SetColour(m_RedrawColors[i]);
				dc.SetBrush(brush);

				int x = m_RedrawXYs[i].first;
				int y = m_RedrawXYs[i].second;

				dc.DrawRectangle(x * m_Size, y * m_Size, m_Size, m_Size);
			}

			m_RedrawXYs.clear();
			m_RedrawColors.clear();
			return;
		}

		brush.SetColour(m_RedrawColor);
		dc.SetBrush(brush);

		int x = m_RedrawXY.first;
		int y = m_RedrawXY.second;

		dc.DrawRectangle(x * m_Size, y * m_Size, m_Size, m_Size);
		return;
	}

	brush.SetColour(wxColour("white"));
	dc.SetBrush(brush);

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
	m_JustScrolled = true;

	if (m_ToolModes->GetMode() == 'D')
	{
		wxMouseState mouseState = wxGetMouseState();

		std::pair<int, int> hoveredCell = GetHoveredCell(m_MouseXY.first, m_MouseXY.second);
		int x = hoveredCell.first;
		int y = hoveredCell.second;

		ControlUpdateCoords(x, y);

		if (mouseState.LeftIsDown())
		{
			std::pair<std::string, wxColour> state = m_ToolStates->GetState();
			InsertCell(x, y, state.first, state.second);
		}
		else if (mouseState.RightIsDown())
		{
			if (m_Cells.find({ x,y }) == m_Cells.end()) return;

			std::pair<std::string, wxColour> state = m_Cells[{x, y}];
			RemoveCell(x, y, state.first, state.second);
		}
	}

	evt.Skip();
}

void Grid::OnEraseBackground(wxEraseEvent& evt)
{
}

void Grid::DeleteStructure(int X, int Y, std::unordered_set<std::pair<int,int>, Hashes::PairHash> visited, std::string state)
{
	visited.insert({ X,Y });

	int dx[8] = {0, 1, 1, 1, 0, -1, -1 ,-1};
	int dy[8] = {-1, -1, 0, 1, 1, 1, 0, -1};

	// delete a whole structure no matter the states it is composed of
	if (state.empty())
	{
		for (int d = 0; d < 8; d++)
		{
			int x = X + dx[d];
			int y = Y + dy[d];

			if (m_Cells.find({ x,y }) != m_Cells.end() && visited.find({ x, y }) == visited.end())
			{
				DeleteStructure(x, y, visited, "");
			}
		}
	}
	// otherwise, make sure to respect the given input
	else
	{
		for (int d = 0; d < 8; d++)
		{
			int x = X + dx[d];
			int y = Y + dy[d];

			if (m_StatePositions[state].find({ x,y }) != m_StatePositions[state].end() && visited.find({ x, y }) == visited.end())
			{
				DeleteStructure(x, y, visited, state);
			}
		}
	}

	if (state.empty()) state = m_Cells[{X, Y}].first;
	
	// TO DO, add function EraseCell(x,y,state,color) for dealing with this
	EraseCell(X, Y, true);
}
