#pragma once
#include "wx/wx.h"
#include "wx/stc/stc.h"

#include "Ids.h"
#include "Sizes.h"

class EditorRules: public wxFrame
{
public:
	EditorRules(wxFrame* parent);
	~EditorRules();

	std::vector<std::string> GetData();
private:
	wxStyledTextCtrl* m_TextCtrl = nullptr;

	void BuildMenuBar();
	void BuildInputPanel();
	void LoadData();
};

