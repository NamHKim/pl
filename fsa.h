// PL homework: hw1
// fsa.h

#ifndef _PL_HOMEWORK_FSA_H_
#define _PL_HOMEWORK_FSA_H_

#include <vector>
#include <string>
#include <utility>
#include <set>

// Valid characters are alphanumeric and underscore (A-Z,a-z,0-9,_).
// Epsilon moves in NFA are represented by empty strings.

struct FSATableElement {
  int state;
  int next_state;
  std::string str;
};

struct newState {
	int num;
	std::set<int> element;
	std::vector<std::pair<char, int>> transition;
	bool accept = false;
};

struct FiniteStateAutomaton {
  // Make your own FSA struct here.
	int start_state;
	std::vector<newState*> state_set;
	std::vector<int> accept_state;
	std::set<char> symbol;
};

// Run FSA and return true if str is matched by fsa, and false otherwise.
bool RunFSA(const FiniteStateAutomaton& fsa, const std::string& str);

bool BuildFSA(const std::vector<FSATableElement>& elements,
              const std::vector<int>& accept_states,
              FiniteStateAutomaton* fsa);

#endif //_PL_HOMEWORK_FSA_H_

