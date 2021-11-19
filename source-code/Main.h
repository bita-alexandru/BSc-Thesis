#pragma once
#include "wx/wx.h"
#include "wx/listctrl.h"
#include "wx/statline.h"

#include "MenuBar.h"
#include "PanelInput.h"
#include "PanelGrid.h"
#include "PanelAlgorithm.h"

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

	enum CONSTANTS
	{
		ID_FIND = 10400, ID_REPLACE, ID_COPY, ID_CUT, ID_PASTE,
		ID_SAVE_STATES, ID_SAVE_RULES, ID_SAVE_NEIGHBORS,
		ID_LIST_STATES, ID_LIST_RULES, ID_LIST_NEIGHBORS
	};

	void BuildInterface();
	void SetShortcuts();

	void BuildLayout();

	wxDECLARE_EVENT_TABLE();

	void OnOpenAutomaton(wxCommandEvent& evt);
	void OnOpenAlgorithm(wxCommandEvent& evt);
	void OnSaveAutomaton(wxCommandEvent& evt);
	void OnSaveAlgorithm(wxCommandEvent& evt);
	void OnExit(wxCommandEvent& evt);
	void OnResetAutomaton(wxCommandEvent& evt);
	void OnResetAlgorithm(wxCommandEvent& evt);
	void OnDocumentation(wxCommandEvent& evt);
};