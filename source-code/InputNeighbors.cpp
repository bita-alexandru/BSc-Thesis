#include "InputNeighbors.h"

InputNeighbors::InputNeighbors(wxScrolledWindow* parent, wxStaticBoxSizer* sizer)
{
	wxButton* button = new wxButton(parent, ID_EDIT_NEIGHBORS, wxString("Edit Neighbors"));
	wxListView * list = new wxListView(parent, ID_LIST_NEIGHBORS);

	list->AppendColumn("#", wxLIST_FORMAT_LEFT, 32);
	list->AppendColumn("Direction");
	list->AppendColumn("Alias");

	sizer->Add(button, 0, wxEXPAND);
	sizer->Add(list, 1, wxEXPAND);
}

InputNeighbors::~InputNeighbors()
{
}

wxListView* InputNeighbors::GetList()
{
	return m_List;
}