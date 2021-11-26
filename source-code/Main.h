#pragma once
#include "wx/wx.h"
#include "wx/listctrl.h"
#include "wx/statline.h"

#include "Constants.h"
#include "MenuBar.h"
#include "PanelInput.h"
#include "PanelGrid.h"
#include "PanelAlgorithm.h"
#include "EditorStates.h"
#include "EditorNeighbors.h"
#include "EditorRules.h"

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
	EditorNeighbors* m_EditorNeighbors = nullptr;
	EditorRules* m_EditorRules = nullptr;

	void BuildInterface();
	void SetShortcuts();

	wxDECLARE_EVENT_TABLE();

	// MenuBar
	void OnOpenAutomaton(wxCommandEvent& evt);
	void OnOpenAlgorithm(wxCommandEvent& evt);
	void OnSaveAutomaton(wxCommandEvent& evt);
	void OnSaveAlgorithm(wxCommandEvent& evt);
	void OnExit(wxCommandEvent& evt);
	void OnResetAutomaton(wxCommandEvent& evt);
	void OnResetAlgorithm(wxCommandEvent& evt);
	void OnDocumentation(wxCommandEvent& evt);

	// ToolZoom
	void OnZoomIn(wxCommandEvent& evt);
	void OnZoomOut(wxCommandEvent& evt);

	// Editors
	void EditStates(wxCommandEvent& evt);
	void EditNeighbors(wxCommandEvent& evt);
	void EditRules(wxCommandEvent& evt);

	// Save Buttons
	void SaveStates(wxCommandEvent& evt);
};