#pragma once
#include "wx/wx.h"

#include "Ids.h"
#include "Main.h"

class Main;

class MenuBar: public wxMenuBar
{
public:
	MenuBar();
	~MenuBar();
private:
	void BuildInterface();
	void SetFunctions();

	void OnExit(wxCommandEvent& evt);
};

