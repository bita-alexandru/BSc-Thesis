#pragma once
#include "wx/wx.h"
#include "wx/vscroll.h"
#include "wx/dcclient.h"
#include "wx/dcmemory.h"
#include "wx/dcbuffer.h"

#include <unordered_set>

#include "Ids.h"
#include "Sizes.h"
#include "ToolModes.h"
#include "ToolStates.h"
#include "ToolCoords.h"

class Grid: public wxHVScrolledWindow
{
public:
	Grid(wxWindow* parent);
	~Grid();

	int GetSize();
	void SetSize(int size);
	void ScrollToCenter();

	void SetToolModes(ToolModes* toolModes);
	void SetToolStates(ToolStates* toolStates);
	void SetToolCoords(ToolCoords* toolCoords);
private:
	int m_Size = Sizes::CELL_SIZE_DEFAULT;
	const int m_Offset = Sizes::TOTAL_CELLS / 2;
	bool m_Centered = false;

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
	std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, PairHash> m_Cells = std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, PairHash>();
	std::unordered_map<std::string, std::unordered_set<std::pair<int, int>>> m_StatePositions = std::unordered_map<std::string, std::unordered_set<std::pair<int, int>>>();

	wxTimer* m_MouseTimer = nullptr;

	virtual wxCoord OnGetRowHeight(size_t row) const;
	virtual wxCoord OnGetColumnWidth(size_t row) const;

	wxDECLARE_EVENT_TABLE();
	void OnDraw(wxDC& dc);
	void OnPaint(wxPaintEvent& evt);

	void OnMouse(wxMouseEvent& evt);
};

