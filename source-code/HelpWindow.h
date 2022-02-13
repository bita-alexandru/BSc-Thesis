#pragma once

#include "wx/wx.h"
#include "wx/wxhtml.h"

#include <string>
#include <deque>

class HelpWindow : public wxFrame
{
public:
	HelpWindow(wxWindow* parent);
	~HelpWindow();

	void SetPage(std::string);
private:
	wxHtmlWindow* m_Html = nullptr;

	std::deque<wxString> m_Undo;
	std::deque<wxString> m_Redo;

	void BuildInterface();
	void SetShortcuts();

	wxDECLARE_EVENT_TABLE();

	void OnPrev(wxCommandEvent& evt);
	void OnNext(wxCommandEvent& evt);
	void OnMainMenu(wxCommandEvent& evt);
	void OnClose(wxCommandEvent& evt);
	void OnCloseEvent(wxCloseEvent& evt);

	void OnLinkClick(wxHtmlLinkEvent& evt);
};

