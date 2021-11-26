#pragma once
#include "wx/wx.h"
#include "wx/stc/stc.h"

#include "Ids.h"
#include "Sizes.h"

class EditorStates: public wxFrame
{
public:
	EditorStates(wxFrame* parent);
	~EditorStates();

	std::vector<std::string> GetData();
private:
	wxStyledTextCtrl* m_TextCtrl = nullptr;

	void BuildMenuBar();
	void BuildInputPanel();
	void LoadData();
};

