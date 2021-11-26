#pragma once
#include "wx/wx.h"
#include "wx/stc/stc.h"

#include "Constants.h"

class EditorStates: public wxFrame
{
public:
	EditorStates(wxFrame* parent);
	~EditorStates();
private:
	void BuildMenuBar();
	void BuildInputPanel();
	void LoadData();
};

