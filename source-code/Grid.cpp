#include "Grid.h"

wxBEGIN_EVENT_TABLE(Grid, wxHVScrolledWindow)
	EVT_PAINT(Grid::OnPaint)
	EVT_MOUSE_EVENTS(Grid::OnMouse)
	EVT_TIMER(Ids::ID_TIMER_SELECTION, Grid::OnTimerSelection)
	EVT_KEY_DOWN(Grid::OnKeyDown)
	EVT_ERASE_BACKGROUND(Grid::OnEraseBackground)
wxEND_EVENT_TABLE()


Grid::Grid(wxWindow* parent): wxHVScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS)
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

	// still drawing
	if (m_IsDrawing || m_IsErasing)
	{
		m_IsDrawing = false;
		m_IsErasing = false;

		if (m_Cells != m_PrevCells)
		{
			m_ToolUndo->PushBack(m_Cells, m_StatePositions, m_PrevCells, m_PrevStatePositions);
			m_PrevCells = m_Cells;
			m_PrevStatePositions = m_StatePositions;
		}
	}

	// to do, set a variable to mark that the grid has just been resized
	// will assist in drawing optimization

	// get (x,y) located at the center of the screen
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
	// make sure (x,y) will be located at the center of the screen after zooming
	wxPosition visibleBegin = GetVisibleBegin();
	wxPosition visibleEnd = GetVisibleEnd();

	int row = y;
	int column = x;

	row = row - (visibleEnd.GetRow() - visibleBegin.GetRow()) / 2;
	column = column - (visibleEnd.GetCol() - visibleBegin.GetCol()) / 2;

	row = std::max(0, row); row = std::min(Sizes::TOTAL_CELLS - 1, row);
	column = std::max(0, column); column = std::min(Sizes::TOTAL_CELLS - 1, column);

	ScrollToRowColumn(row, column);

	m_RedrawAll = true;
	Refresh(false);
	Update();
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

void Grid::InsertCell(int x, int y, std::string state, wxColour color, bool multiple)
{
	if (GetState(x, y) != "FREE")
	{
		// current cell is of state "FREE" but there's already a cell of another state
		// on this exact position -> remove it
		if (color == wxColour("white")) RemoveCell(x, y, state, color);
		// position is occupied
		else
		{
			// same state -> don't do anything
			if (m_Cells[{x, y}].first == state) return;

			EraseCell(x, y);

			m_Cells[{x, y}] = { state, color };
			m_StatePositions[state].insert({ x,y });

			if (multiple)
			{
				m_RedrawAll = false;
				m_RedrawXYs.push_back({ x,y });
				m_RedrawColors.push_back(color);
			}
			else
			{
				m_RedrawAll = false;
				m_RedrawXY = { x,y };
				m_RedrawColor = color;
				Refresh(false);
				Update();
			}
		}
	}
	// position is available
	else if (color != wxColour("white"))
	{
		m_Cells[{x, y}] = { state, color };
		m_StatePositions[state].insert({ x,y });

		if (multiple)
		{
			m_RedrawAll = false;
			m_RedrawXYs.push_back({ x,y });
			m_RedrawColors.push_back(color);
		}
		else
		{
			m_RedrawAll = false;
			m_RedrawXY = { x,y };
			m_RedrawColor = color;
			Refresh(false);
			Update();
		}
	}
}

void Grid::RemoveCell(int x, int y, std::string state, wxColour color, bool multiple)
{
	if (GetState(x, y) != "FREE")
	{
		EraseCell(x, y, multiple);
		
		if (!multiple)
		{
			Refresh(false);
			Update();
		}
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

std::string Grid::GetState(int x, int y)
{
	if (m_Cells.find({ x,y }) == m_Cells.end()) return "FREE";

	return m_Cells[{x, y}].first;
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

bool Grid::ControlSelectState()
{
	wxMouseState mouse = wxGetMouseState();
	// shortcut for alternating between states: Shift + L/R Click
	if ((mouse.LeftIsDown() || mouse.RightIsDown()) && wxGetKeyState(WXK_SHIFT))
	{
		if (!m_TimerSelection->IsRunning()) m_TimerSelection->Start(80);

		return true;
	}

	return false;
}

bool Grid::ControlZoom(int x, int y, int rotation)
{
	// shortcut for zooming in/out with focus on hovered cell
	if (rotation && wxGetKeyState(WXK_CONTROL))
	{
		if (rotation > 0 && m_ToolZoom->GetSize() < Sizes::CELL_SIZE_MAX)
		{
			ScrollToCenter(x, y);
			m_ToolZoom->ZoomIn();
		}
		if (rotation < 0 && m_ToolZoom->GetSize() > Sizes::CELL_SIZE_MIN)
		{
			ScrollToCenter(x, y);
			m_ToolZoom->ZoomOut();
		}

		wxPoint XY = ScreenToClient(wxGetMouseState().GetPosition());
		ControlUpdateCoords(XY.x, XY.y);

		return true;
	}

	return false;
}

std::string Grid::ControlUpdateCoords(int x, int y)
{
	// update coordinates displayed on screen
	std::string name = GetState(x, y);

	m_ToolCoords->Set(x - m_Offset, y - m_Offset, name);

	return name;
}

bool Grid::ModeDraw(int x, int y, char mode)
{
	// "draw" mode
	if (mode == 'D')
	{
		wxMouseState mouse = wxGetMouseState();

		// left click -> place a cell
		if (mouse.LeftIsDown())
		{
			std::pair<std::string, wxColour> state = m_ToolStates->GetState();
			
			// just clicked
			if (!m_IsDrawing)
			{
				m_IsDrawing = true;
				m_IsErasing = false;
				m_LastDrawn = { x,y };
				
				InsertCell(x, y, state.first, state.second);
			}
			// holding click
			else
			{
				// mouse was dragged so fast that it didn't register
				// some of the cells meant to be drawn
				DrawLine(x, y, state.first, state.second);
			}
		}
		// right click -> remove a cell
		else if (mouse.RightIsDown())
		{
			if (GetState(x, y) == "FREE")
			{
				m_LastDrawn = { x,y };
				return true;
			}

			std::pair<std::string, wxColour> state = m_Cells[{x, y}];

			if (wxGetKeyState(WXK_CONTROL))
			{
				if (wxGetKeyState(WXK_ALT)) DeleteStructure(x, y, "");
				else DeleteStructure(x, y, state.first);
			}

			// just clicked
			if (!m_IsErasing)
			{
				m_IsErasing = true;
				m_IsDrawing = false;
				m_LastDrawn = { x,y };

				RemoveCell(x, y, state.first, state.second);
			}
			// holding click
			else
			{
				// mouse was dragged so fast that it didn't register
				// some of the cells meant to be drawn
				DrawLine(x, y, "FREE", wxColour("white"), true);
			}
		}
		else if (m_IsDrawing || m_IsErasing)
		{
			if (m_Cells != m_PrevCells)
			{
				m_ToolUndo->PushBack(m_Cells, m_StatePositions, m_PrevCells, m_PrevStatePositions);
				m_PrevCells = m_Cells;
				m_PrevStatePositions = m_StatePositions;
			}

			m_IsDrawing = false;
			m_IsErasing = false;
		}

		return true;
	}

	return false;
}

bool Grid::ModePick(int x, int y, char mode, std::string state)
{
	// "pick" mode
	if (mode == 'P')
	{
		// still drawing
		if (m_IsDrawing || m_IsErasing)
		{
			m_IsDrawing = false;
			m_IsErasing = false;

			if (m_Cells != m_PrevCells)
			{
				m_ToolUndo->PushBack(m_Cells, m_StatePositions, m_PrevCells, m_PrevStatePositions);
				m_PrevCells = m_Cells;
				m_PrevStatePositions = m_StatePositions;
			}
		}
		if (m_IsMoving) m_IsMoving = false;

		wxMouseState mouse = wxGetMouseState();

		if (mouse.LeftIsDown())
		{
			m_ToolStates->SetState(state);
			m_ToolModes->SetMode('D');
		}
		else if (mouse.RightIsDown())
		{
			m_ToolModes->SetMode('D');
		}

		return true;
	}

	return false;
}

bool Grid::ModeMove(int x, int y, char mode)
{
	wxMouseState mouse = wxGetMouseState();

	if ((mode == 'M' && mouse.LeftIsDown()) || mouse.MiddleIsDown())
	{
		// still drawing
		if (m_IsDrawing || m_IsErasing)
		{
			m_IsDrawing = false;
			m_IsErasing = false;

			if (m_Cells != m_PrevCells)
			{
				m_ToolUndo->PushBack(m_Cells, m_StatePositions, m_PrevCells, m_PrevStatePositions);
				m_PrevCells = m_Cells;
				m_PrevStatePositions = m_StatePositions;
			}
		}

		if (!m_IsMoving)
		{
			m_IsMoving = true;

			SetFocus();
			m_PrevCell = { x,y };

			return true;
		}
		else
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
	}
	else if (mode == 'M' && mouse.RightIsDown())
	{
		m_IsMoving = false;

		m_ToolModes->SetMode('D');
		return true;
	}
	else m_IsMoving = false;

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

	// to do, optimize drawing furthermore, only redraw changes made after scrolling
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
		
		// still drawing
		if (m_IsDrawing || m_IsErasing)
		{
			m_IsDrawing = false;
			m_IsErasing = false;

			if (m_Cells != m_PrevCells)
			{
				m_ToolUndo->PushBack(m_Cells, m_StatePositions, m_PrevCells, m_PrevStatePositions);
				m_PrevCells = m_Cells;
				m_PrevStatePositions = m_StatePositions;
			}
		}

		return;
	}

	if (ControlSelectState()) return;

	std::pair<int, int> hoveredCell = GetHoveredCell(evt.GetX(), evt.GetY());
	int x = hoveredCell.first;
	int y = hoveredCell.second;

	if (evt.Entering()) m_LastDrawn = { x,y };

	if (ControlZoom(x, y, evt.GetWheelRotation())) return;

	std::string state = ControlUpdateCoords(x, y);

	char mode = m_ToolModes->GetMode();

	if (ModeMove(x, y, mode))
	{
		evt.Skip();
		return;
	}
	if (ModeDraw(x, y, mode))
	{
		evt.Skip();
		return;
	}
	if (ModePick(x, y, mode, state))
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
	switch (evt.GetKeyCode())
	{
	case WXK_UP:
	case 'W':
		ScrollRows(-1);
		break;
	case WXK_DOWN:
	case 'S':
		ScrollRows(1);
		break;
	case WXK_LEFT:
	case 'A':
		ScrollColumns(-1);
		break;
	case WXK_RIGHT:
	case 'D':
		ScrollColumns(1);
		break;
	default:
		return;
	}
	m_JustScrolled = true;

	wxMouseState mouseState = wxGetMouseState();
	wxPoint XY = mouseState.GetPosition();

	int X = ScreenToClient(XY).x;
	int Y = ScreenToClient(XY).y;

	std::pair<int, int> hoveredCell = GetHoveredCell(X, Y);
	int x = hoveredCell.first;
	int y = hoveredCell.second;

	ControlUpdateCoords(x, y);

	ModeDraw(x, y, m_ToolModes->GetMode());

	//evt.Skip();
}

void Grid::OnEraseBackground(wxEraseEvent& evt)
{
}

void Grid::DeleteStructure(int X, int Y, std::string state)
{
	int dx[8] = {0, 1, 1, 1, 0, -1, -1 ,-1};
	int dy[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
	
	std::unordered_set<std::pair<int, int>, Hashes::PairHash> visited;
	std::stack<std::pair<int, int>> neighbors;
	neighbors.push({ X,Y });

	while (neighbors.size())
	{
		std::pair<int, int> neighbor = neighbors.top();
		neighbors.pop();

		visited.insert(neighbor);

		std::string neighborState = GetState(neighbor.first, neighbor.second);

		// doesn't matter if the structure is composed of cells of the same stats
		if (state == "") EraseCell(neighbor.first, neighbor.second, true);
		// otherwise erase only if they share the same state
		else if (state == neighborState) EraseCell(neighbor.first, neighbor.second, true);
		
		for (int d = 0; d < 8; d++)
		{
			int x = neighbor.first + dx[d];
			int y = neighbor.second + dy[d];

			// valid neighbor -> push onto stack
			if (GetState(x, y) != "FREE" && visited.find({ x, y }) == visited.end())
			{
				neighbors.push({ x,y });
			}
		}
	}

	Refresh(false);
	Update();
}

void Grid::DrawLine(int x, int y, std::string state, wxColour color, bool remove)
{
	if (m_LastDrawn != std::make_pair(x, y))
	{
		// apply the following algorithm to draw a line
		// starting from the last drawn {x,y} to the current {x,y}
		int changed = 0;
		int d, ii, jj, di, ai, si, dj, aj, sj;
		di = x - m_LastDrawn.first;
		ai = abs(di) << 1;
		si = (di < 0) ? -1 : 1;
		dj = y - m_LastDrawn.second;
		aj = abs(dj) << 1;
		sj = (dj < 0) ? -1 : 1;

		ii = m_LastDrawn.first;
		jj = m_LastDrawn.second;

		if (ai > aj)
		{
			d = aj - (ai >> 1);
			while (ii != x)
			{
				std::string currState = GetState(ii, jj);
				if (currState != state)
				{
					if (!remove) InsertCell(ii, jj, state, color, true);
					else EraseCell(ii, jj, true);
					changed++;
				}
				if (d >= 0)
				{
					jj += sj;
					d -= ai;
				}
				ii += si;
				d += aj;
			}
		}
		else
		{
			d = ai - (aj >> 1);
			while (jj != y) 
			{
				std::string currState = GetState(ii, jj);
				if (currState != state) 
				{
					if (!remove) InsertCell(ii, jj, state, color, true);
					else EraseCell(ii, jj, true);
					changed++;
				}
				if (d >= 0) 
				{
					ii += si;
					d -= aj;
				}
				jj += sj;
				d += ai;
			}
		}

		m_LastDrawn = { x,y };

		std::string currState = GetState(x, y);
		if (currState != state) 
		{
			if (!remove) InsertCell(ii, jj, state, color, true);
			else EraseCell(ii, jj, true);
			changed++;
		}

		if (changed > 0)
		{
			Refresh(false);
			Update();
		}
	}
}

bool Grid::InBounds(int x, int y)
{
	return (x >= 0 && x < Sizes::TOTAL_CELLS && y >= 0 && y < Sizes::TOTAL_CELLS);
}
