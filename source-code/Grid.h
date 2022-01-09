#pragma once
#include "wx/wx.h"
#include "wx/vscroll.h"
#include "wx/dcclient.h"
#include "wx/dcmemory.h"
#include "wx/dcbuffer.h"

#include <unordered_set>

#include "Ids.h"
#include "Sizes.h"
#include "Hashes.h"
#include "ToolZoom.h"
#include "ToolUndo.h"
#include "ToolModes.h"
#include "ToolStates.h"
#include "ToolCoords.h"
#include "StatusCells.h"

class ToolZoom;
class ToolUndo;
class ToolStates;
class ToolModes;

class Grid : public wxHVScrolledWindow
{
public:
	Grid(wxWindow* parent);
	~Grid();

	int GetSize();
	void SetSize(int size, bool center = true);
	void ScrollToCenter(int x = Sizes::TOTAL_CELLS / 2, int y = Sizes::TOTAL_CELLS / 2);

	void SetCells(
		std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairInt> cells,
		std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairInt>> statePositions
	);

	void SetToolZoom(ToolZoom* toolZoom);
	void SetToolUndo(ToolUndo* toolUndo);
	void SetToolModes(ToolModes* toolModes);
	void SetToolStates(ToolStates* toolStates);
	void SetToolCoords(ToolCoords* toolCoords);
	void SetStatusCells(StatusCells* statusCells);

	void InsertCell(int x, int y, std::string state, wxColour color, bool multiple = false);
	void RemoveCell(int x, int y, std::string state, wxColour color, bool multiple = false);
	void RemoveState(std::string state);
	void UpdateState(std::string oldState, wxColour oldColor, std::string newState, wxColour newColor);
	void EraseCell(int x, int y, bool multiple = false);
	std::string GetState(int x, int y);

	void Reset();
	std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairInt> GetCells();
	std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairInt>> GetStatePositions();
	std::unordered_map<std::string, wxColour>& GetColors();
private:
	int m_Size = Sizes::CELL_SIZE_DEFAULT;
	const int m_Offset = Sizes::TOTAL_CELLS / 2;
	bool m_Centered = false;

	ToolZoom* m_ToolZoom = nullptr;
	ToolUndo* m_ToolUndo = nullptr;
	ToolModes* m_ToolModes = nullptr;
	ToolStates* m_ToolStates = nullptr;
	ToolCoords* m_ToolCoords = nullptr;

	StatusCells* m_StatusCells = nullptr;

	std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairInt> m_Cells;
	std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairInt>> m_StatePositions;
	std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairInt> m_PrevCells;
	std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairInt>> m_PrevStatePositions;

	wxTimer* m_TimerSelection = nullptr;

	bool m_PrevScrolledCol = false;
	bool m_PrevScrolledRow = false;
	bool m_PrevUpdated = false;
	bool m_AlreadyDrawn = false;
	bool m_IsDrawing = false;
	bool m_IsErasing = false;
	bool m_IsMoving = false;

	std::pair<int, int> m_PrevCell;
	std::pair<int, int> m_LastDrawn;
	std::pair<int, int> m_JustScrolled = { 0,0 };

	std::unordered_set<int> m_Keys;

	bool m_RedrawAll = true;
	bool m_JustResized = false;

	std::pair<int, int> m_RedrawXY;
	std::vector<std::pair<int, int>> m_RedrawXYs;
	std::vector<wxColour> m_RedrawColors;
	wxColour m_RedrawColor;

	virtual wxCoord OnGetRowHeight(size_t row) const;
	virtual wxCoord OnGetColumnWidth(size_t row) const;
	void OnPaint(wxPaintEvent& evt);

	void BuildInterface();
	void InitializeTimers();

	std::pair<int, int> GetHoveredCell(int X, int Y);
	bool ControlSelectState();
	bool ControlZoom(int x, int y, int rotation);
	std::string ControlUpdateCoords(int x, int y);
	bool ModeDraw(int x, int y, char mode);
	bool ModePick(int x, int y, char mode, std::string state);
	bool ModeMove(int x, int y, char mode);
	void ProcessKeys();

	wxDECLARE_EVENT_TABLE();
	void OnDraw(wxDC& dc);
	void OnMouse(wxMouseEvent& evt);
	void OnTimerSelection(wxTimerEvent& evt);
	void OnKeyDown(wxKeyEvent& evt);
	void OnKeyUp(wxKeyEvent& evt);
	void OnEraseBackground(wxEraseEvent& evt);
	void OnScroll(wxScrollWinEvent& evt);
	void OnSize(wxSizeEvent& evt);

	void DeleteStructure(int x, int y, std::string state = "");
	void DrawLine(int x, int y, std::string state, wxColour color, bool remove = false);
	bool InBounds(int x, int y);
};
