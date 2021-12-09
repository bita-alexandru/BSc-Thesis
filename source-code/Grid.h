#pragma once
#include "wx/wx.h"
#include "wx/vscroll.h"
#include "wx/dcclient.h"
#include "wx/dcmemory.h"
#include "wx/dcbuffer.h"

#include <unordered_set>

#include "Ids.h"
#include "Sizes.h"
#include "ToolZoom.h"
#include "ToolModes.h"
#include "ToolStates.h"
#include "ToolCoords.h"

class ToolZoom;

class Grid: public wxHVScrolledWindow
{
public:
	Grid(wxWindow* parent);
	~Grid();

	int GetSize();
	void SetSize(int size);
	void ScrollToCenter(int x, int y);

	void SetToolZoom(ToolZoom* toolZoom);
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
	ToolModes* m_ToolModes = nullptr;
	ToolStates* m_ToolStates = nullptr;
	ToolCoords* m_ToolCoords = nullptr;
	
	struct PairHash {
		template <class x, class y>
		std::size_t operator() (const std::pair<x, y>& p) const {
			size_t index = p.second * Sizes::TOTAL_CELLS + p.first;
			auto h = std::hash<x>{}(index);

			return h;
		}
	};
	std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, PairHash> m_Cells;
	std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, PairHash>> m_StatePositions;

	wxTimer* m_TimerSelection = nullptr;
	bool m_PrevScrolledCol = false;
	bool m_PrevScrolledRow = false;

	std::pair<int, int> m_PrevCell;

	virtual wxCoord OnGetRowHeight(size_t row) const;
	virtual wxCoord OnGetColumnWidth(size_t row) const;
	void OnPaint(wxPaintEvent& evt);

	void BuildInterface();
	void InitializeTimers();

	std::pair<int, int> GetHoveredCell(wxMouseEvent& evt);
	bool ControlSelectState(wxMouseEvent& evt);
	void ControlScroll(wxMouseEvent& evt);
	bool ControlZoom(wxMouseEvent& evt, int x, int y);
	std::string ControlUpdateCoords(wxMouseEvent& evt, int x, int y);
	bool ModeDraw(wxMouseEvent& evt, int x, int y, char mode);
	bool ModePick(wxMouseEvent& evt, int x, int y, char mode, std::string state);
	bool ModeMove(wxMouseEvent& evt, int x, int y, char mode);

	wxDECLARE_EVENT_TABLE();
	void OnDraw(wxDC& dc);
	void OnMouse(wxMouseEvent& evt);
	void OnTimerSelection(wxTimerEvent& evt);
	void OnTimerPanning(wxTimerEvent& evt);
};

