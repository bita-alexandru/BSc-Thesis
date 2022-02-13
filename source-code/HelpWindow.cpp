#include "HelpWindow.h"

#include "Ids.h"

wxBEGIN_EVENT_TABLE(HelpWindow, wxFrame)
	EVT_CLOSE(HelpWindow::OnCloseEvent)
wxEND_EVENT_TABLE()

HelpWindow::HelpWindow(wxWindow* parent) : wxFrame(parent, wxID_ANY, "CellyGen::Rules", wxDefaultPosition, wxSize(1280, 720))
{
	SetIcon(wxICON(aaaIcon));

	Center();

	BuildInterface();
}

HelpWindow::~HelpWindow()
{
	wxDELETE(m_Html);
}

void HelpWindow::SetPage(std::string)
{
}

void HelpWindow::BuildInterface()
{
	m_Html = new wxHtmlWindow(this);
	m_Html->LoadPage("D:/Diverse/BSc-Thesis/help/index.html");
	m_Html->Bind(wxEVT_HTML_LINK_CLICKED, &HelpWindow::OnLinkClick, this);

	wxButton* prev = new wxButton(this, Ids::ID_PREV_HELP, "<");
	wxButton* next = new wxButton(this, Ids::ID_NEXT_HELP, ">");
	prev->SetToolTip("Previous Page\tCtrl+Left");
	next->SetToolTip("Next Page\tCtrl+Right");

	prev->Bind(wxEVT_BUTTON, &HelpWindow::OnPrev, this);
	next->Bind(wxEVT_BUTTON, &HelpWindow::OnNext, this);

	prev->Disable();
	next->Disable();

	wxButton* contents = new wxButton(this, Ids::ID_CONTENTS_HELP, "Main Contents");
	contents->Bind(wxEVT_BUTTON, &HelpWindow::OnMainMenu, this);

	wxBoxSizer* buttons = new wxBoxSizer(wxHORIZONTAL);
	buttons->Add(prev, 1, wxEXPAND);
	buttons->Add(next, 1, wxEXPAND);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(m_Html, 1, wxEXPAND);
	sizer->Add(buttons, 0, wxEXPAND);
	sizer->Add(contents, 0, wxEXPAND);

	SetSizer(sizer);
}

void HelpWindow::SetShortcuts()
{
	wxAcceleratorEntry entries[3];
	entries[0].Set(wxACCEL_CTRL, WXK_LEFT, Ids::ID_PREV_HELP);
	entries[1].Set(wxACCEL_CTRL, WXK_RIGHT, Ids::ID_NEXT_HELP);
	entries[2].Set(wxACCEL_CTRL, WXK_RETURN, Ids::ID_CONTENTS_HELP);

	wxAcceleratorTable accel(3, entries);
	SetAcceleratorTable(accel);
}

void HelpWindow::OnPrev(wxCommandEvent& evt)
{

}

void HelpWindow::OnNext(wxCommandEvent& evt)
{

}

void HelpWindow::OnMainMenu(wxCommandEvent& evt)
{

}

void HelpWindow::OnClose(wxCommandEvent& evt)
{
	Hide();
}

void HelpWindow::OnCloseEvent(wxCloseEvent& evt)
{
	Hide();
}

void HelpWindow::OnLinkClick(wxHtmlLinkEvent& evt)
{
	wxLogDebug("<%s>", evt.GetLinkInfo().GetHref());

	evt.Skip();
}
