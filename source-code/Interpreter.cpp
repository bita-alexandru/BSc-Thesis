#include "Interpreter.h"
#include "Sizes.h"

#include <sstream>
#include "wx/log.h"

Interpreter::Interpreter()
{
}

Interpreter::~Interpreter()
{
}

vector<pair<int, string>> Interpreter::Process(string& rules)
{
	m_Transitions.clear();
	unordered_set<string> duplicates;

	// keep count of duplicates/invalid rules
	vector<pair<int, string>> invalid;

	// TO DO, MARCHEAZA COMENTARII CU "!" SI GO TO "\n" WHEN DETECTED

	// add spaces around specific symbols and convert characters to uppercase
	for (int i = 0; i < rules.size(); i++)
	{
		char c = rules[i];

		if (BOTH_SPACED.find(c) != BOTH_SPACED.npos)
		{
			// add space at the both sides of the symbol
			rules.insert(i + 1, " ");
			rules.insert(i, " ");

			i += 2;
			continue;
		}
		if (LEFT_SPACED.find(c) != LEFT_SPACED.npos)
		{
			// add space before the symbol
			rules.insert(i, " ");

			i++;
			continue;
		}
		if (RIGHT_SPACED.find(c) != RIGHT_SPACED.npos)
		{
			// add space after the symbol
			rules.insert(i + 1, " ");

			i += 2;
			continue;
		}

		if (NON_SPACED.find(c) != NON_SPACED.npos) continue;

		if (isalnum(c))
		{
			// convert to uppercase
			rules[i] = toupper(rules[i]);

			continue;
		}

		if (iswspace(c)) continue;

		// character is illegal
		invalid.push_back({ i,"<ILLEGAL CHARACTER>" });
	}

	stringstream ss(rules);
	int cursor = 0;
	int size = 0;
	spaces = 0;

	while (true)
	{
		int chars = 0;
		Transition transition = {};

		string state1;
		string symbol;
		string state2;
		bool valid = true;

		// read transitory state
		ss >> state1;
		//wxLogDebug("state1=<%s>", state1);

		if (!UpdateChars(chars, state1)) MarkInvalid(valid, invalid,  "<SIZE OF RULE SURPASSES MAXIMUM LIMIT>", cursor);
		
		//wxLogDebug("OK-1");
		// check if end of file
		if (!FindWord(cursor, rules, state1)) break;
		//wxLogDebug("OK-2");
		// check if state is invalid
		if (valid && !CheckState(state1)) MarkInvalid(valid, invalid,  "<INVALID FIRST STATE>", cursor);
		//wxLogDebug("OK-3");
		// check if rule is within the size limits

		// read transition symbol "/"
		if (valid)
		{
			ss >> symbol; FindWord(cursor, rules, symbol);
			//wxLogDebug("symbol=<%s>", symbol);

			// check if rule is within the size limits
			if (!UpdateChars(chars, symbol)) MarkInvalid(valid, invalid,  "<SIZE OF RULE SURPASSES MAXIMUM LIMIT>", cursor);
			
			// check if it's the right symbol
			if (valid && symbol != "/") MarkInvalid(valid, invalid,  "<INVALID TRANSITION SYMBOL, EXPECTED '/'>", cursor);
		}

		// read transition state
		if (valid)
		{
			ss >> state2; FindWord(cursor, rules, state2);
			//wxLogDebug("state2=<%s>", state2);

			// check if rule is within the size limits
			if (!UpdateChars(chars, state2)) MarkInvalid(valid, invalid,  "<SIZE OF RULE SURPASSES MAXIMUM LIMIT>", cursor);
			
			// check if state is invalid
			if (valid && !CheckState(state2)) MarkInvalid(valid, invalid,  "<INVALID SECOND STATE>", cursor);

			if (valid)
			{
				//wxLogDebug("hash=<%s>", state1 + "-" + state2);
				// check if transition is a duplicate
				if (duplicates.find(state1 + "-" + state2) != duplicates.end()) MarkInvalid(valid, invalid,  "<DUPLICATE RULE>", cursor);
				else if (state1 == state2) MarkInvalid(valid, invalid,  "<ILLEGAL RULE>", cursor);

				if (valid) duplicates.insert({ state1 + "-" + state2 });
				else //wxLogDebug("cursor=%i spaces=%i", cursor,spaces);

				// assign to transition
				if (valid) transition.state = state2;
			}
		}

		// read symbol ":" or ";"
		if (valid)
		{
			symbol.clear();
			ss >> symbol; FindWord(cursor, rules, symbol);
			//wxLogDebug("symbol=<%s>", symbol);

			// check if rule is within the size limits
			if (!UpdateChars(chars, symbol)) MarkInvalid(valid, invalid,  "<SIZE OF RULE SURPASSES MAXIMUM LIMIT>", cursor);

			if (valid)
			{
				// end of transition rules
				if (symbol == ";")
				{
					// add to transition table
					m_Transitions.push_back({ state1,transition });
				}
				// indicates that a list of rules will follow
				else if (symbol == ":")
				{
					transition.orRules.clear();
					transition.andRules.clear();
					transition.andRules.push_back({});
					
					// read every rule until ";" is detected
					while (valid)
					{
						symbol.clear();
						ss >> symbol; FindWord(cursor, rules, symbol);
						transition.condition += symbol;
						//wxLogDebug("symbol=<%s>", symbol);

						// check if rule is marked accordingly with a "("
						if (symbol != "(") MarkInvalid(valid, invalid,  "<INVALID RULE SYMBOL, EXPECTED '('>", cursor);

						// check if rule is within the size limits
						if (valid && !UpdateChars(chars, symbol)) MarkInvalid(valid, invalid,  "<SIZE OF RULE SURPASSES MAXIMUM LIMIT>", cursor);

						// read neighborhood and conditions
						if (valid)
						{
							symbol.clear();
							ss >> symbol; FindWord(cursor, rules, symbol);
							transition.condition += symbol;

							// check if symbol is "@"
							if (symbol != "@") MarkInvalid(valid, invalid,  "<INVALID NEIGHBORHOOD SYMBOL, EXPECTED '@'", cursor);

							// check if rule is within the size limits
							if (valid && !UpdateChars(chars, symbol)) MarkInvalid(valid, invalid,  "<SIZE OF RULE SURPASSES MAXIMUM LIMIT>", cursor);

							if (valid)
							{
								// could indicate either a group of directions or a specific one
								string neighborhood;
								ss >> neighborhood; FindWord(cursor, rules, neighborhood);
								transition.condition += neighborhood;
								//wxLogDebug("neighborhood=<%s>", neighborhood);

								if (!UpdateChars(chars, neighborhood)) MarkInvalid(valid, invalid,  "<SIZE OF RULE SURPASSES MAXIMUM LIMIT>", cursor);

								// a group of directions, eg. "[n,s,w,e]"
								if (valid && neighborhood == "[")
								{
									NEIGHBORS neighbors;

									// read every direction until "]" is detected
									while (valid)
									{
										string direction;
										ss >> direction; FindWord(cursor, rules, direction);
										transition.condition += direction;
										//wxLogDebug("direction=<%s>", direction);

										// check if rule is within the size limits
										if (valid && !UpdateChars(chars, direction)) MarkInvalid(valid, invalid,  "<SIZE OF RULE SURPASSES MAXIMUM LIMIT>", cursor);

										// check if direction is valid according to the current neighborhood
										if (valid && !CheckDirection(direction)) MarkInvalid(valid, invalid,  "<INVALID DIRECTION>", cursor);

										if (valid) neighbors.push_back(direction);

										if (!valid) break;

										// expect to read either "," or "]"
										symbol.clear();
										ss >> symbol; FindWord(cursor, rules, symbol);
										transition.condition += symbol;
										//wxLogDebug("symbol=<%s>", symbol);

										// more directions to follow
										if (symbol == ",")
										{
											if (!UpdateChars(chars, symbol)) MarkInvalid(valid, invalid,  "<SIZE OF RULE SURPASSES MAXIMUM LIMIT>", cursor);
											continue;
										}
										// neighborhood completed
										else if (symbol == "]")
										{
											if (!UpdateChars(chars, symbol)) MarkInvalid(valid, invalid,  "<SIZE OF RULE SURPASSES MAXIMUM LIMIT>", cursor);

											// check if neighborhood is within the size limit
											if (valid && neighbors.size() > m_Neighbors.size()) MarkInvalid(valid, invalid,  "<INVALID NEIGHBORHOOD SIZE>", cursor);

											// assign to transition
											if (valid) transition.andRules.back().first = neighbors;

											break;
										}
										// invalid symbol
										else
										{
											MarkInvalid(valid, invalid,  "<INVALID SYMBOL, EXPECTED ',' OR ']'>", cursor);
											break;
										}
									}
								}
								// a specific direction or "ALL" meaning every possible valid direction
								else if (valid && (neighborhood == "ALL" || CheckDirection(neighborhood)))
								{
									// assign to transition
									transition.andRules.back().first = { neighborhood };
								}
								// invalid token
								else if (!valid) MarkInvalid(valid, invalid,  "<INVALID NEIGHBORHOOD>", cursor);

								if (!valid) break;

								// read assignment symbol "="
								symbol.clear();
								ss >> symbol; FindWord(cursor, rules, symbol);
								transition.condition += symbol;
								//wxLogDebug("symbol=<%s>", symbol);

								if (symbol != "=") MarkInvalid(valid, invalid,  "<INVALID ASSIGNMENT SYMBOL>", cursor);
								
								if (valid && !UpdateChars(chars, symbol)) MarkInvalid(valid, invalid,  "<SIZE OF RULE SURPASSES MAXIMUM LIMIT>", cursor);
								
								if (!valid) break;

								transition.orConditions.clear();
								transition.andConditions.clear();
								transition.andConditions.push_back({});

								int count = 0;

								// read conditions until ")" is detected
								while (valid)
								{
									// condition could be of different forms
									// eg. "<number>#<state>" / "<sign><number>#<state>"
									// or  "#<state>" / "<sign>#<state>"
									string condition;
									ss >> condition; FindWord(cursor, rules, condition);
									transition.condition += condition;
									//wxLogDebug("condition=<%s>", condition);

									if (!UpdateChars(chars, condition)) MarkInvalid(valid, invalid,  "<SIZE OF RULE SURPASSES MAXIMUM LIMIT>", cursor);

									if (!valid) break;

									// split by "#"
									stringstream tokenizer(condition);
									vector<string> tokens;
									string token;

									while (getline(tokenizer, token, '#')) tokens.push_back(token);

									if (tokens.size() == 2)
									{
										string sign = "";
										string number = tokens.front();
										string state = tokens.back();

										// "#<state>"
										if (number.empty()) number = "1";
										// <sign> is present
										if (number.size() && (number[0] == '+' || number[0] == '-'))
										{
											sign.push_back(number[0]);
											number.erase(number.begin());

											// no number specified
											if (number.empty()) number = "1";
										}

										// check if number is valid
										int n = CheckNumber(number, transition, count);
										if (n == -1) MarkInvalid(valid, invalid,  "<SIZE OF RULE SURPASSES MAXIMUM LIMIT>", cursor);

										// check if state is valid
										if (valid && !CheckState(state)) MarkInvalid(valid, invalid,  "<INVALID CONDITIONAL STATE>", cursor);

										// assign to transition
										if (valid)
										{
											count += n;

											int comparisonType = TYPE_EQUAL;
											if (sign == "+") comparisonType = TYPE_MORE;
											else if (sign == "-")
											{
												comparisonType = TYPE_LESS;
												count -= n;
											}

											// number of cells
											transition.andConditions.back().first.first = n;
											// comparison type
											transition.andConditions.back().first.first = comparisonType;
											// state
											transition.andConditions.back().second = state;
										}
									}
									// invalid tokens
									else MarkInvalid(valid, invalid,  "<INVALID CONDITION>", cursor);

									if (!valid) break;

									// expect to read either an AND/OR symbol or ")"
									symbol.clear();
									ss >> symbol; FindWord(cursor, rules, symbol);
									if (symbol == "AND") transition.condition += " AND ";
									else if (symbol == "OR") transition.condition += " OR ";
									else transition.condition += symbol;
									//wxLogDebug("symbol=<%s>", symbol);
									if (!UpdateChars(chars, symbol)) MarkInvalid(valid, invalid,  "<SIZE OF RULE SURPASSES MAXIMUM LIMIT>", cursor);

									if (find(AND.begin(), AND.end(), symbol) != AND.end())
									{
										transition.andConditions.push_back({});
									}
									else if (find(OR.begin(), OR.end(), symbol) != OR.end())
									{
										transition.orConditions.push_back(transition.andConditions);
										transition.andConditions.clear();
										transition.andConditions.push_back({});
										count = 0;
									}
									else if (symbol == ")")
									{
										transition.orConditions.push_back(transition.andConditions);
										transition.andRules.back().second = transition.orConditions;
										break;
									}
									else valid = false;
								}
							}
						}
					
						if (!valid) break;

						// expect to read either an AND/OR symbol or ";"
						symbol.clear();
						ss >> symbol; FindWord(cursor, rules, symbol);
						if (symbol != ";") transition.condition += symbol;
						//wxLogDebug("symbol=<%s>", symbol);

						if (!UpdateChars(chars, symbol)) MarkInvalid(valid, invalid,  "<SIZE OF RULE SURPASSES MAXIMUM LIMIT>", cursor);

						if (!valid) break;

						if (find(AND.begin(), AND.end(), symbol) != AND.end())
						{
							transition.andRules.push_back({});
						}
						else if (find(OR.begin(), OR.end(), symbol) != OR.end())
						{
							transition.orRules.push_back(transition.andRules);
							transition.andRules.clear();
							transition.andRules.push_back({});
						}
						else if (symbol == ";")
						{
							transition.orRules.push_back(transition.andRules);
							break;
						}
						else MarkInvalid(valid, invalid, "<INVALID CHAINING SYMBOL, EXPECTED EITHER '&', '|' OR ';'>", cursor);
					}
					
					// add to transition table
					if (valid) m_Transitions.push_back({ state1,transition });
				}
				else MarkInvalid(valid, invalid, "<INVALID RULE MARKING SYMBOL, EXPECTED EITHER ':' OR ';'>", cursor);
			}
		}
		
		// go to the next transition if there's any left
		if (!valid)
		{
			if (!NextTransition(cursor, rules, ss)) break;
		}

		//wxLogDebug("valid=%i", valid);
	}

	if (m_Transitions.size() > Sizes::RULES_MAX) invalid = { {-1,"<THE NUMBER OF RULES SURPASSES THE MAXIMUM LIMIT>"} };

	////wxLogDebug("invalid_size=%i", invalid.size());
	return invalid;
}

vector<pair<string, Transition>>& Interpreter::GetTransitions()
{
	return m_Transitions;
}

void Interpreter::SetStates(unordered_map<string, string>& states)
{
	m_States = states;
}

void Interpreter::SetNeighbors(unordered_set<string>& neighbors)
{
	m_Neighbors = neighbors;
}

bool Interpreter::FindWord(int& cursor, string &rules, string& s)
{
	int pos = 0;
	bool ret = true;

	// reached end of file	
	if (s.empty())
	{
		pos = rules.size();
		ret = false;
	}
	else
	{
		pos = rules.find(s, cursor);

		// reached end of file
		if (pos == rules.npos)
		{
			pos = rules.size();
			ret = false;
		}
		// not a whole word
		else if (pos > 0 && !iswspace(rules[pos - 1]))
		{
			pos = pos + 1;// + s.size();
			ret = false;
		}
		// not a whole word
		else if (pos + s.size() < rules.size() && !iswspace(rules[pos + s.size()]))
		{
			pos = pos + 1;// + s.size();
			ret = false;
		}
		// whole word -> place cursor at the start of word
		else
		{
			pos = pos + 1;// +s.size();
		}
	}

	// count extra white spaces along the way
	for (int i = cursor; i < pos; i++)
	{
		char c = rules[i];

		if (BOTH_SPACED.find(c) != BOTH_SPACED.npos) spaces += 2;
	}

	////wxLogDebug("token=%s spaces=%i", s, spaces);

	cursor = pos;

	return ret;
}

bool Interpreter::NextTransition(int& cursor, string& rules, stringstream& ss)
{
	string semicolon = ";";

	// end of file
	if (!FindWord(cursor, rules, semicolon)) return false;

	// otherwise, update stream position
	ss.seekg(cursor);

	return true;
}

bool Interpreter::CheckState(string& state)
{
	return state.size() >= Sizes::CHARS_STATE_MIN
			&& state.size() <= Sizes::CHARS_STATE_MAX
			&& find_if(state.begin(), state.end(), [](char c) { return !(isalnum(c) || (c == ' ')); }) == state.end()
			&& m_States.find(state) != m_States.end();
}

bool Interpreter::UpdateChars(int& chars, string& s)
{
	chars += s.size();

	/*if (s.size() && BOTH_SPACED.find(s) != BOTH_SPACED.npos)
	{
		spaces += 2;
	}*/
	//wxLogDebug("symbol=%s spaces=%i", s,spaces);

	return chars <= Sizes::CHARS_RULE_MAX;
}

bool Interpreter::UpdateSize(int& size)
{
	size++;

	return size <= Sizes::RULES_MAX;
}

void Interpreter::MarkInvalid(bool& valid, vector<pair<int, string>>& invalid, string reason, int& cursor)
{
	// mark it in our list
	invalid.push_back({ cursor - spaces, reason });

	valid = false;
}

bool Interpreter::CheckDirection(string& direction)
{
	return m_Neighbors.find(direction) != m_Neighbors.end();
}

int Interpreter::CheckNumber(string& number, Transition& transition, int& count)
{
	int n = -1;

	// convert to number if valid
	if (number.size() && number.find_first_not_of("0123456789") == string::npos) n = stoi(number);

	// check that it does not surpass the neighborhood size limit
	// also keep count of the total number tracked by the previous "AND" condition
	if (transition.andRules.back().first[0] == "ALL" && n + count > m_Neighbors.size()) n = -1;
	else if (transition.andRules.back().first[0] != "ALL" && n + count > transition.andRules.back().first.size()) n = -1;

	return n;
}
