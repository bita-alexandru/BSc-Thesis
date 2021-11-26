#pragma once
#include "wx/wx.h"
#include "wx/statline.h"

#include "InputStates.h"
#include "InputNeighbors.h"
#include "InputRules.h"

class PanelInput : public wxScrolledWindow
{
public:
	PanelInput(wxWindow* parent);
	~PanelInput();

	InputStates* GetInputStates();
	InputNeighbors* GetInputNeighbors();
	InputRules* GetInputRules();
private:
	InputStates* m_InputStates = nullptr;
	InputNeighbors* m_InputNeighbors = nullptr;
	InputRules* m_InputRules = nullptr;
};

