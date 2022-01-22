#pragma once
#include "wx/thread.h"
#include "wx/colour.h"

#include "Hashes.h"
#include "Transition.h"

#include <string>
#include <vector>
#include <utility>
#include <unordered_set>
#include <unordered_map>

//#include "Grid.h"

//class Grid;

class ThreadRuleApplier : public wxThread
{
public:
	ThreadRuleApplier(std::string mode);
	~ThreadRuleApplier();

	void SetRange(int rowStart, int rowSize, int colStart, int colSize);
	void SetRange(int itStart, int itSize);
	void SetRule(std::pair<const std::string, Transition>* rule);

	void SetState(std::string state);
	void SetNeighbors(std::unordered_set<std::string>* neighbors);

	void SetApplied(std::vector<std::pair<int, int>>* applied);
	void SetVisited(std::unordered_set<std::pair<int, int>, Hashes::PairInt>* visited);
	void SetCells(std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairInt>* cells);
	void SetStatePositions(std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairInt>>* statePositions);

	void SetCriticalApplied(wxCriticalSection* appliedCS);
	void SetCriticalVisited(wxCriticalSection* visitedCS);
	void SetCriticalStatePosition(wxCriticalSection* statePositionsCS);

	virtual ExitCode Entry();
private:
	std::string m_Mode;

	int m_RowStart = 0;
	int m_RowSize = 0;
	int m_ColStart = 0;
	int m_ColSize = 0;
	int m_itStart = 0;
	int m_itSize = 0;

	std::string m_State;

	std::pair<const std::string, Transition>* m_Rule = nullptr;
	std::unordered_set<std::string>* m_Neighbors = nullptr;

	std::vector<std::pair<int, int>>* m_Applied = nullptr;
	std::unordered_set<std::pair<int, int>, Hashes::PairInt>* m_Visited = nullptr;
	std::unordered_map<std::pair<int, int>, std::pair<std::string, wxColour>, Hashes::PairInt>* m_Cells = nullptr;
	std::unordered_map<std::string, std::unordered_set<std::pair<int, int>, Hashes::PairInt>>* m_StatePositions = nullptr;

	wxCriticalSection* m_AppliedCS = nullptr;
	wxCriticalSection* m_VisitedCS = nullptr;
	wxCriticalSection* m_StatePositionsCS = nullptr;

	bool ApplyOnCell(int x, int y);
	bool InBounds(int x, int y);

	std::string GetState(int x, int y);
	std::unordered_map<std::string, std::string> GetNeighborhood(int x, int y);
};

