#pragma once
#include "Transition.h"

#include "wx/colour.h"

#include <unordered_set>

using namespace std;

class Interpreter
{
public:
	Interpreter();
	~Interpreter();

	vector<int> Process(string& rules);
	unordered_multimap<string, Transition> GetTransitionTable();
	
	void SetStates(unordered_map<string, pair<string, wxColour>>& states);
	void SetNeighbors(unordered_set<string>& neighbors);
private:
	unordered_multimap<string, Transition> m_TransitionTable;
	unordered_map<string, pair<string, wxColour>> m_States;
	unordered_set<string> m_Neighbors;

	bool UpdateCursor(int& cursor, stringstream& ss);
	bool NextTransition(int& cursor, stringstream& ss);
	bool CheckState(string& state);
	bool UpdateChars(int& chars, string& s);
	bool UpdateSize(int& size);
};

