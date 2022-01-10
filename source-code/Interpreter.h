#pragma once
#include "Transition.h"

#include <unordered_set>

using namespace std;

class Interpreter
{
public:
	Interpreter();
	~Interpreter();

	vector<int> Process(string& rules);
	unordered_multimap<string, Transition> GetTransitions();
	
	void SetStates(unordered_map<string, string>& states);
	void SetNeighbors(unordered_set<string>& neighbors);
private:
	unordered_multimap<string, Transition> m_Transitions;
	unordered_map<string, string> m_States;
	unordered_set<string> m_Neighbors;

	bool FindWord(int& cursor, string& rules, string& s);
	bool NextTransition(int& cursor, string& rules, stringstream& ss);
	bool CheckState(string& state);
	bool UpdateChars(int& chars, string& s);
	bool UpdateSize(int& size);
	void MarkInvalid(bool& valid, vector<int>& invalid, int& cursor, string& rules, stringstream& ss);
};

