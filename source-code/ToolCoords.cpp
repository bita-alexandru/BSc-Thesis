#include "ToolCoords.h"

ToolCoords::ToolCoords(wxWindow* parent, wxBoxSizer* sizer)
{
	std::string label = "(X,Y)=";
	m_Coords = new wxStaticText(parent, wxID_ANY, label);

	sizer->Add(m_Coords, 0, wxALIGN_CENTER_VERTICAL);
}

ToolCoords::~ToolCoords()
{
}

void ToolCoords::SetCoords(int x, int y)
{
	std::string label = "(X,Y)=";

	const int N = Constants::TOTAL_CELLS;

	if (x < N && x > -N && y < N && y > -N)
	{
		label += "(" + std::to_string(x) + "," + std::to_string(y) + ")";
	}
	
	m_Coords->SetLabel(label);
}
