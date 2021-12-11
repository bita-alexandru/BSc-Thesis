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

class ToolZoom;
class ToolUndo;

class Grid: public wxHVScrolledWindow
{
public:
	Grid(wxWindow* parent);
	~Grid();

	int GetSize();
	void SetSize(int size);
	void ScrollToCenter(int x = Sizes::TOTAL_CELLS / 2, int y = Sizes::TOTAL_CELLS / 2);

	void SetCells(
		std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairHash> cells,
		std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairHash>> statePositions
	);

	void SetToolZoom(ToolZoom* toolZoom);
	void SetToolUndo(ToolUndo* toolUndo);
	void SetToolModes(ToolModes* toolModes);
	void SetToolStates(ToolStates* toolStates);
	void SetToolCoords(ToolCoords* toolCoords);

	void InsertCell(int x, int y, std::string state, wxColour color);
	void RemoveCell(int x, int y, std::string state, wxColour color);
	void RemoveState(std::string state, wxColour color);
	void UpdateState(std::string oldState, wxColour oldColor, std::string newState, wxColour newColor);
private:
	int m_Size = Sizes::CELL_SIZE_DEFAULT;
	const int m_Offset = Sizes::TOTAL_CELLS / 2;
	bool m_Centered = false;

	ToolZoom* m_ToolZoom = nullptr;
	ToolUndo* m_ToolUndo = nullptr;
	ToolModes* m_ToolModes = nullptr;
	ToolStates* m_ToolStates = nullptr;
	ToolCoords* m_ToolCoords = nullptr;
	
	std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairHash> m_Cells;
	std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairHash>> m_StatePositions;
	std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairHash> m_PrevCells;
	std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairHash>> m_PrevStatePositions;

	wxTimer* m_TimerSelection = nullptr;
	bool m_PrevScrolledCol = false;
	bool m_PrevScrolledRow = false;
	bool m_PrevUpdated = false;

	std::pair<int, int> m_MouseXY;

	std::pair<int, int> m_PrevCell;

	virtual wxCoord OnGetRowHeight(size_t row) const;
	virtual wxCoord OnGetColumnWidth(size_t row) const;
	void OnPaint(wxPaintEvent& evt);

	void BuildInterface();
	void InitializeTimers();

	std::pair<int, int> GetHoveredCell(int X, int Y);
	bool ControlSelectState(wxMouseEvent& evt);
	bool ControlZoom(wxMouseEvent& evt, int x, int y);
	std::string ControlUpdateCoords(int x, int y);
	bool ModeDraw(wxMouseEvent& evt, int x, int y, char mode);
	bool ModePick(wxMouseEvent& evt, int x, int y, char mode, std::string state);
	bool ModeMove(wxMouseEvent& evt, int x, int y, char mode);

	void DrawCell(wxDC& dc, int x, int y, wxColour color);

	wxDECLARE_EVENT_TABLE();
	void OnDraw(wxDC& dc);
	void OnMouse(wxMouseEvent& evt);
	void OnTimerSelection(wxTimerEvent& evt);
	void OnKeyDown(wxKeyEvent& evt);
};

