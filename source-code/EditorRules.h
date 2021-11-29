#pragma once
#include "wx/wx.h"
#include "wx/stc/stc.h"

#include <unordered_map>
#include <any>

#include "Ids.h"
#include "Sizes.h"

class EditorRules: public wxFrame
{
public:
	EditorRules(wxFrame* parent);
	~EditorRules();

	std::unordered_map<std::string, std::any> GetData();
private:
	wxStyledTextCtrl* m_TextCtrl = nullptr;

	void BuildMenuBar();
	void BuildInputPanel();
	void LoadData();
};

