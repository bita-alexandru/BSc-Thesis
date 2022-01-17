#pragma once
#include "Transition.h"

#include <unordered_set>

using namespace std;

class Interpreter
{
public:
	Interpreter();
	~Interpreter();

	vector<pair<int, string>> Process(string& rules);
	vector<pair<string, Transition>>& GetTransitions();
private:
	vector<pair<string, Transition>> m_Transitions;

	vector<string> AND = { ",", "&", "AND" };
	vector<string> OR = { "|", "OR" };

	string LEFT_SPACED = "";
	string RIGHT_SPACED = "";
	string BOTH_SPACED = "/:()[]@=,&|;!";
	string NON_SPACED = "+-#";
	int spaces = 0;
	int cursorBeforeComment = 0;
	bool isComment = false;

	bool FindWord(int& cursor, string& rules, string& s, bool comment = false);
	bool NextTransition(int& cursor, string& rules, stringstream& ss);
	bool CheckState(string& state);
	bool UpdateChars(int& chars, string& s);
	bool UpdateSize(int& size);
	void MarkInvalid(bool& valid, vector<pair<int, string>>& invalid, string& s, string reason, int& cursor);
	bool CheckDirection(string& direction);
	int CheckNumber(string& number, Transition& transition, int& count);
	bool SkipIfComment(int& cursor, string& rules, stringstream& ss, string& s);
};

