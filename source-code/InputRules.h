#pragma once
#include "wx/wx.h"
#include "wx/srchctrl.h"

#include "Ids.h"
#include "ListRules.h"
#include "EditorRules.h"

#include <any>

class EditorRules;

class InputRules : public wxPanel
{
public:
	InputRules(wxWindow* parent);
	~InputRules();

	ListRules* GetList();
	std::unordered_map<std::string, std::unordered_map<std::string, std::any>>& GetRules();
	
	void SetEditorRules(EditorRules* editorRules);

	void SetRules(std::vector<std::string> rules);
	void SetStates(std::unordered_map<std::string, std::string>& states);
private:
	ListRules* m_List = nullptr;
	EditorRules* m_EditorRules = nullptr;

	std::unordered_map<std::string, std::unordered_map<std::string, std::any>> m_Rules;
	std::unordered_map<std::string, std::string> m_States;

	void BuildInterface();

	void Search(wxCommandEvent& evt);
	void SearchEnter(wxCommandEvent& evt);

	void OnEdit(wxCommandEvent& evt);
};

