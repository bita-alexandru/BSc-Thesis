#pragma once
#include "wx/wx.h"

class Main: public wxFrame
{
public:
	Main();
	~Main();

private:
	enum ID_CONSTANTS {
		ID_IMPORT_C = 10001, ID_IMPORT_G,
		ID_EXPORT_C, ID_EXPORT_G,
		ID_EXIT,
		ID_RESET_C, ID_RESET_G,
		ID_DOCUMENTATION
	};

	wxMenuBar* m_MenuBar = nullptr;

	void BuildMenuBar();
	
	void OnImportC(wxCommandEvent& evt);
	void OnImportG(wxCommandEvent& evt);
	void OnExportC(wxCommandEvent& evt);
	void OnExportG(wxCommandEvent& evt);
	void OnExit(wxCommandEvent& evt);
	void OnResetC(wxCommandEvent& evt);
	void OnResetG(wxCommandEvent& evt);
	void OnDocumentation(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();
};

