#pragma once
#include "wx/wx.h"
#include "wx/tglbtn.h"

#include "Ids.h"

class InputNeighbors : public wxPanel
{
public:
	InputNeighbors(wxWindow* parent);
	~InputNeighbors();
private:
	wxToggleButton* m_NW = nullptr;
	wxToggleButton* m_N = nullptr;
	wxToggleButton* m_NE = nullptr;
	wxToggleButton* m_E = nullptr;
	wxToggleButton* m_SE = nullptr;
	wxToggleButton* m_S = nullptr;
	wxToggleButton* m_SW = nullptr;
	wxToggleButton* m_W = nullptr;
	wxToggleButton* m_C = nullptr;
};

