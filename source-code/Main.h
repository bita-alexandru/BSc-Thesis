#pragma once
#include "wx/wx.h"


class Main: public wxFrame
{
public:
	Main();
	~Main();

private:
	enum ID_CONSTANTS {
		ID_OPEN_C = 10001, ID_OPEN_G,
		ID_SAVE_C, ID_SAVE_G,
		ID_EXIT,
		ID_RESET_C, ID_RESET_G,
		ID_DOCUMENTATION
	};

	wxMenuBar* m_MenuBar = nullptr;

	void BuildMenuBar();
	
	void OnOpenC(wxCommandEvent& evt);
	void OnOpenG(wxCommandEvent& evt);
	void OnSaveC(wxCommandEvent& evt);
	void OnSaveG(wxCommandEvent& evt);
	void OnExit(wxCommandEvent& evt);
	void OnResetC(wxCommandEvent& evt);
	void OnResetG(wxCommandEvent& evt);
	void OnDocumentation(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();
};

