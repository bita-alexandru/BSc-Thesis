#include "ListStates.h"

ListStates::ListStates(wxWindow* parent) : wxListCtrl(parent, wxID_ANY, wxDefaultPosition, wxSize(128, 128), wxLC_REPORT | wxLC_VIRTUAL)
{
	AppendColumn("#");
	AppendColumn("State");
}

ListStates::~ListStates()
{
	for (int i = 0; i < attrs.size(); i++) wxDELETE(attrs[i]);
}

wxString ListStates::OnGetItemText(long index, long column) const
{
	std::pair<int, std::string> item = items[index];

	switch (column)
	{
	case 0: return std::to_string(item.first);
	case 1: return item.second;
	default: return "";
	}
}

wxItemAttr* ListStates::OnGetItemAttr(long index) const
{
	if (index < attrs.size()) return attrs[index];

	return nullptr;
}

void ListStates::RefreshAfterUpdate(bool eraseBackground)
{
	SetItemCount(items.size());

	SetColumnWidth(0, wxLIST_AUTOSIZE);
	SetColumnWidth(1, wxLIST_AUTOSIZE_USEHEADER);

	Refresh(eraseBackground);
}

std::vector<std::string> ListStates::GetStates()
{
	std::vector<std::string> states;

	for (int i = 0; i < items.size(); i++) states.push_back(items[i].second);

	return states;
}

long ListStates::GetFirstSelected()
{
	return GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
}

long ListStates::GetNextSelected(long selection)
{
	return GetNextItem(selection, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
}

void ListStates::Select(long index, bool state)
{
	if (state) SetItemState(index, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
	else SetItemState(index, 0, wxLIST_STATE_SELECTED);
}

void ListStates::PushBack(std::pair<int, std::string> item, std::pair<wxColour, wxColour> color)
{
	items.push_back(item);

	attrs.push_back(
		new wxListItemAttr(color.second, color.first, font)
	);

	SetItemCount(items.size());
}

void ListStates::ChangeItemId(int index, int id)
{
	if (index < items.size()) items[index].first = id;
}

void ListStates::ChangeItemState(int index, std::string state)
{
	if (index < items.size()) items[index].second = state;
}

void ListStates::ChangeItemColor(long index, wxColour bgColor, wxColour txtColor)
{
	if (index < attrs.size())
	{
		attrs[index]->SetBackgroundColour(bgColor);
		attrs[index]->SetTextColour(txtColor);
	}
}

void ListStates::Erase(int index)
{
	if (index < items.size())
	{
		items.erase(items.begin() + index);
		
		wxDELETE(attrs[index]);
		attrs.erase(attrs.begin() + index);
	}
}

std::pair<int, std::string> ListStates::Get(int index)
{
	if (index < items.size()) return items[index];

	return {};
}

void ListStates::SetItemColor(int index, wxColour color)
{
	if (index >= 0 && index < items.size())
	{
		attrs[index]->SetBackgroundColour(color);

		// if background is black -> font should be white and viceversa - inequation found on stackoverflow
		wxColour blackwhite = (color.Red() * 0.299 + color.Green() * 0.587 + color.Blue() * 0.114) > 186.0 ? wxColour("black") : wxColour("white");
		attrs[index]->SetTextColour(blackwhite);
	}
}

wxColour ListStates::GetItemColor(int index)
{
	if (index >= 0 && index < attrs.size()) return attrs[index]->GetBackgroundColour();

	return wxColour("white");
}
