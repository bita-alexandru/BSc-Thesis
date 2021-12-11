#pragma once
#include "wx/wx.h"
#include "wx/listctrl.h"

class ListStates : public wxListCtrl
{
public:
	ListStates(wxWindow* parent);
	~ListStates();

	virtual wxString OnGetItemText(long index, long column) const;
	virtual wxItemAttr* OnGetItemAttr(long index) const;
	void RefreshAfterUpdate(bool eraseBackground = true);

	long GetFirstSelected();
	long GetNextSelected(long selection);
	void Select(long index, bool state = true);

	void PushBack(std::pair<int, std::string> item, std::pair<wxColour, wxColour> color);
	void ChangeItemId(int index, int id);
	void ChangeItemState(int index, std::string state);
	void ChangeItemColor(long index, wxColour bgColor, wxColour txtColor);
	void Erase(int index);

	std::pair<int, std::string> Get(int index);

	void SetItemColor(int index, wxColour color);
private:
	std::vector<std::pair<int, std::string>> items;
	std::vector<wxListItemAttr*> attrs;

	wxFont font = wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
};

