#pragma once

#include "wx/wx.h"
#include "wx/listctrl.h"
#include "wx/statline.h"
#include "wx/splitter.h"

#include "Ids.h"
#include "Sizes.h"
#include "MenuBar.h"
#include "PanelInput.h"
#include "PanelGrid.h"
#include "PanelAlgorithm.h"
#include "EditorStates.h"
#include "EditorRules.h"

class MenuBar;

class Main: public wxFrame
{
public:
	Main();
	~Main();
private:
	MenuBar* m_MenuBar = nullptr;
	PanelInput* m_PanelInput = nullptr;
	PanelGrid* m_PanelGrid = nullptr;
	PanelAlgorithm* m_PanelAlgorithm = nullptr;
	EditorStates* m_EditorStates = nullptr;
	EditorRules* m_EditorRules = nullptr;

	wxSplitterWindow* m_SplitterInputGrid = nullptr;
	wxSplitterWindow* m_SplitterGridAlgorithm = nullptr;

	void BuildInterface();
	void SetShortcuts();
	void PrepareInput();
};