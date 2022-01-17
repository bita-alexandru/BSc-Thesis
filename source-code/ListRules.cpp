#include "ListRules.h"

ListRules::ListRules(wxWindow* parent) : wxListCtrl(parent, wxID_ANY, wxDefaultPosition, wxSize(128, 128), wxLC_REPORT | wxLC_VIRTUAL)
{
	AppendColumn("#");
	AppendColumn("State A");
	AppendColumn("State B");
	AppendColumn("Condition");

	SetColumnWidth(0, wxLIST_AUTOSIZE);
	SetColumnWidth(1, wxLIST_AUTOSIZE_USEHEADER);
	SetColumnWidth(2, wxLIST_AUTOSIZE_USEHEADER);
	SetColumnWidth(3, wxLIST_AUTOSIZE_USEHEADER);
}

ListRules::~ListRules()
{
	for (int i = 0; i < items.size(); i++)
	{
		wxDELETE(itmAttrs[i]);
		wxDELETE(aColAttrs[i]);
		wxDELETE(bColAttrs[i]);
	}
}

wxString ListRules::OnGetItemText(long index, long column) const
{
	std::tuple<int, std::string, std::string, std::string> item = items[index];

	switch (column)
	{
	case 0: return std::to_string(std::get<0>(item));
	case 1: return std::get<1>(item);
	case 2: return std::get<2>(item);
	case 3: return std::get<3>(item);
	default: return "";
	}
}

wxItemAttr* ListRules::OnGetItemAttr(long index) const
{
	if (index < itmAttrs.size()) return itmAttrs[index];

	return nullptr;
}

wxItemAttr* ListRules::OnGetItemColumnAttr(long index, long column) const
{
	if (index < items.size())
	{
		if (column == 1) return aColAttrs[index];
		if (column == 2) return bColAttrs[index];
		return itmAttrs[index];
	}

	return nullptr;
}

void ListRules::RefreshAfterUpdate(bool eraseBackground)
{
	SetItemCount(items.size());

	SetColumnWidth(0, wxLIST_AUTOSIZE);
	SetColumnWidth(1, wxLIST_AUTOSIZE);
	SetColumnWidth(2, wxLIST_AUTOSIZE);
	SetColumnWidth(3, wxLIST_AUTOSIZE_USEHEADER);

	Refresh(eraseBackground);
}

long ListRules::GetFirstSelected()
{
	return GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
}

long ListRules::GetNextSelected(long selection)
{
	return GetNextItem(selection, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
}

void ListRules::Select(long index, bool state)
{
	if (state) SetItemState(index, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
	else SetItemState(index, 0, wxLIST_STATE_SELECTED);
}

void ListRules::PushBack(std::tuple<int, std::string, std::string, std::string> item, wxColour colorA, wxColour colorB)
{
	items.push_back(item);

	wxColour txtcolorA = (colorA.Red() * 0.299 + colorA.Green() * 0.587 + colorA.Blue() * 0.114) > 186.0 ? wxColour("black") : wxColour("white");
	wxColour txtcolorB = (colorB.Red() * 0.299 + colorB.Green() * 0.587 + colorB.Blue() * 0.114) > 186.0 ? wxColour("black") : wxColour("white");

	itmAttrs.push_back(
		new wxListItemAttr(wxColour("black"), wxColour("white"), font)
	);
	aColAttrs.push_back(
		new wxListItemAttr(txtcolorA, colorA, font)
	);
	bColAttrs.push_back(
		new wxListItemAttr(txtcolorB, colorB, font)
	);
}

void ListRules::ChangeItemId(int index, int id)
{
	if (index < items.size()) std::get<0>(items[index]) = id;
}

void ListRules::ChangeItemState1(int index, std::string state)
{
	if (index < items.size()) std::get<1>(items[index]) = state;
}

void ListRules::ChangeItemState2(int index, std::string state)
{
	if (index < items.size()) std::get<2>(items[index]) = state;
}

void ListRules::ChangeItemCond(int index, std::string cond)
{
	if (index < items.size()) std::get<3>(items[index]) = cond;
}

void ListRules::ChangeItemColor(long index, wxColour colorA, wxColour colorB)
{
	if (index < items.size())
	{
		wxColour txtcolorA = (colorA.Red() * 0.299 + colorA.Green() * 0.587 + colorA.Blue() * 0.114) > 186.0 ? wxColour("black") : wxColour("white");
		wxColour txtcolorB = (colorB.Red() * 0.299 + colorB.Green() * 0.587 + colorB.Blue() * 0.114) > 186.0 ? wxColour("black") : wxColour("white");

		aColAttrs[index]->SetBackgroundColour(colorA);
		aColAttrs[index]->SetTextColour(txtcolorA);

		bColAttrs[index]->SetBackgroundColour(colorB);
		bColAttrs[index]->SetTextColour(txtcolorB);
	}
}

void ListRules::Erase(int index)
{
	if (index < items.size())
	{
		wxDELETE(itmAttrs[index]);
		wxDELETE(aColAttrs[index]);
		wxDELETE(bColAttrs[index]);

		items.erase(items.begin() + index);
		itmAttrs.erase(itmAttrs.begin() + index);
		aColAttrs.erase(aColAttrs.begin() + index);
		bColAttrs.erase(bColAttrs.begin() + index);
	}
}

int ListRules::GetIndex(int index)
{
	if (index < items.size()) return std::get<0>(items[index]);

	return 0;
}

std::string ListRules::GetState1(int index)
{
	if (index < items.size()) return std::get<1>(items[index]);

	return "";
}

std::string ListRules::GetState2(int index)
{
	if (index < items.size()) return std::get<2>(items[index]);

	return "";
}

std::string ListRules::GetCond(int index)
{
	if (index < items.size()) return std::get<3>(items[index]);

	return "";
}

void ListRules::ChangeColor1(int index, wxColour color)
{
	if (index < items.size())
	{
		wxColour txtcolor = (color.Red() * 0.299 + color.Green() * 0.587 + color.Blue() * 0.114) > 186.0 ? wxColour("black") : wxColour("white");
		aColAttrs[index]->SetBackgroundColour(color);
		aColAttrs[index]->SetTextColour(txtcolor);
	}
}

void ListRules::ChangeColor2(int index, wxColour color)
{
	if (index < items.size())
	{
		wxColour txtcolor = (color.Red() * 0.299 + color.Green() * 0.587 + color.Blue() * 0.114) > 186.0 ? wxColour("black") : wxColour("white");
		bColAttrs[index]->SetBackgroundColour(color);
		bColAttrs[index]->SetTextColour(txtcolor);
	}
}

wxColour ListRules::GetColor1(int index)
{
	if (index < items.size()) return aColAttrs[index]->GetBackgroundColour();

	return wxColour("white");
}

wxColour ListRules::GetColor2(int index)
{
	if (index < items.size()) return bColAttrs[index]->GetBackgroundColour();

	return wxColour("white");
}
