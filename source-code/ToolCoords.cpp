#include "ToolCoords.h"

ToolCoords::ToolCoords(wxWindow* parent) : wxPanel(parent)
{
    //SetBackgroundColour(wxColour(242, 204, 143));

	BuildInterface();
}

ToolCoords::~ToolCoords()
{
}

void ToolCoords::Set(int x, int y, std::string state)
{
	std::string coords = "(X,Y)=(" + std::to_string(x) + "," + std::to_string(y) + ")";
	std::string name = "State=" + state;

	m_Coords->SetLabel(coords);
	m_State->SetLabel(name);
	
	//Layout();
	//Update();
}

void ToolCoords::Reset()
{
	m_Coords->SetLabel("(X,Y)=");
	m_State->SetLabel("State=");

	//Layout();
	//Update();
}

void ToolCoords::BuildInterface()
{
	m_Coords = new wxStaticText(this, wxID_ANY, "(X,Y)=");
	m_State = new wxStaticText(this, wxID_ANY, "State=");

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(m_Coords, 0, wxRIGHT, 256);
	sizer->Add(m_State, 0, wxRIGHT, 256);

	this->SetSizerAndFit(sizer);
}
