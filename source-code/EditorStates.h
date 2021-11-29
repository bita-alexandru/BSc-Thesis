#pragma once
#include "wx/wx.h"
#include "wx/stc/stc.h"
#include "wx/listctrl.h"

#include "Ids.h"
#include "Sizes.h"

class EditorStates: public wxFrame
{
public:
	EditorStates(wxFrame* parent);
	~EditorStates();

	std::vector<std::string> GetData();
	void LoadData(wxListView* list);
private:
	wxStyledTextCtrl* m_TextCtrl = nullptr;

	void BuildMenuBar();
	void BuildInputPanel();
	
	void OnClose(wxCloseEvent& evt);
	void OnShow(wxShowEvent& evt);

	wxDECLARE_EVENT_TABLE();
};

