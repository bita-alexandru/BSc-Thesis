#pragma once
#include "wx/wx.h"
#include "wx/listctrl.h"
#include "wx/statline.h"

#include "IDs.h"
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

	void BuildInterface();
	void SetShortcuts();

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