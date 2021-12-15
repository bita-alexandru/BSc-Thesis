#pragma once
#include "wx/wx.h"
#include "wx/listctrl.h"

class ListRules : public wxListCtrl
{
public:
	ListRules(wxWindow* parent);
	~ListRules();

	virtual wxString OnGetItemText(long index, long column) const;
	virtual wxItemAttr* OnGetItemAttr(long index) const;
	virtual wxItemAttr* OnGetItemColumnAttr(long index, long column) const;
	void RefreshAfterUpdate(bool eraseBackground = true);

	long GetFirstSelected();
	long GetNextSelected(long selection);
	void Select(long index, bool state = true);

	void PushBack(std::tuple<int, std::string, std::string, std::string> item, std::pair<wxColour, wxColour> colorsA, std::pair<wxColour, wxColour> colorsB);
	void ChangeItemId(int index, int id);
	void ChangeItemState1(int index, std::string state);
	void ChangeItemState2(int index, std::string state);
	void ChangeItemCond(int index, std::string cond);
	void ChangeItemColor(long index, std::pair<wxColour, wxColour> colorsA, std::pair<wxColour, wxColour> colorsB);
	void Erase(int index);

	int GetIndex(int index);
	std::string GetState1(int index);
	std::string GetState2(int index);
	std::string GetCond(int index);

	wxColour GetColorA(int index);
	wxColour GetColorB(int index);
private:
	std::vector<std::tuple<int, std::string, std::string, std::string>> items;
	std::vector<wxItemAttr*> itmAttrs;
	std::vector<wxItemAttr*> aColAttrs;
	std::vector<wxItemAttr*> bColAttrs;

	wxFont font = wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
};

