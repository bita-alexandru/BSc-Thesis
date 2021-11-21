#include "GridStatus.h"

GridStatus::GridStatus(wxWindow* parent): wxPanel(parent)
{
}

GridStatus::~GridStatus()
{
}

StatusSpeed* GridStatus::GetStatusSpeed()
{
	return m_StatusSpeed;
}

StatusControls* GridStatus::GetStatusControls()
{
	return m_StatusControls;
}

StatusCells* GridStatus::GetStatusCells()
{
	return m_StatusCells;
}
