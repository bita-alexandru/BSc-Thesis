#include "ToolCoords.h"

ToolCoords::ToolCoords(wxWindow* parent) : wxPanel(parent)
{
	BuildInterface();
}

ToolCoords::~ToolCoords()
{
}

void ToolCoords::SetCoords(int x, int y)
{
	std::string label = "(X,Y)=";

	const int N = Sizes::TOTAL_CELLS;

	if (x < N && x > -N && y < N && y > -N)
	{
		label += "(" + std::to_string(x) + "," + std::to_string(y) + ")";
	}
	
	m_Coords->SetLabel(label);
}

void ToolCoords::BuildInterface()
{
	std::string label = "(X,Y)=";
	m_Coords = new wxStaticText(this, wxID_ANY, label);

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(m_Coords, 0, wxALIGN_CENTER_VERTICAL);

	this->SetSizerAndFit(sizer);
}
