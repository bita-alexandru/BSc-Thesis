#pragma once

#include "wx/wx.h"
#include "wx/listctrl.h"
#include "wx/statline.h"
#include "wx/splitter.h"

#include "Ids.h"
#include "Sizes.h"
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
	void BuildMenubar();
	void SetShortcuts();
	void PrepareInput();

	void MenuExit(wxCommandEvent& evt);
	void MenuPerspectiveDefault(wxCommandEvent& evt);
	void MenuPerspectiveInput(wxCommandEvent& evt);
	void MenuPerspectiveGrid(wxCommandEvent& evt);
	void MenuPerspectiveAlgorithm(wxCommandEvent& evt);
	void MenuEditorStates(wxCommandEvent& evt);
	void MenuEditorRules(wxCommandEvent& evt);

	void OnClose(wxCloseEvent& evt);

	wxDECLARE_EVENT_TABLE();
};