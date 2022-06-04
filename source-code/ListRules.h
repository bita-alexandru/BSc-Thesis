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
	void RefreshAfterUpdate(bool eraseBackground = false);

	std::vector<std::string> GetRules();

	long GetFirstSelected();
	long GetNextSelected(long selection);
	void Select(long index, bool state = true);

	void PushBack(std::tuple<int, std::string, std::string, std::string> item, wxColour colorA, wxColour colorB);
	void ChangeItemId(int index, int id);
	void ChangeItemState1(int index, std::string state);
	void ChangeItemState2(int index, std::string state);
	void ChangeItemCond(int index, std::string cond);
	void ChangeItemColor(long index, wxColour colorA, wxColour colorB);
	void Erase(int index);

	int GetIndex(int index);
	std::string GetState1(int index);
	std::string GetState2(int index);
	std::string GetCond(int index);

	wxColour GetColor1(int index);
	wxColour GetColor2(int index);
	void ChangeColor1(int index, wxColour color);
	void ChangeColor2(int index, wxColour color);
private:
	std::vector<std::tuple<int, std::string, std::string, std::string>> items;
	std::vector<wxItemAttr*> itmAttrs;
	std::vector<wxItemAttr*> aColAttrs;
	std::vector<wxItemAttr*> bColAttrs;

	wxFont font = wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
};

