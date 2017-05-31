// PL homework: hw1
// fsa.cc

#include <iostream>
#include <utility>
#include <set>

#include "fsa.h"

#define DISABLE_LOG true
#define LOG \
    if (DISABLE_LOG) {} else std::cerr

using namespace std;
bool Contain_Sym(const FiniteStateAutomaton& fsa, char ch)
{
	for (std::set<char>::iterator it = fsa.symbol.begin(); it != fsa.symbol.end(); ++it)
		if (*it == ch)
			return true;
	return false;
}
bool goNextState(const FiniteStateAutomaton& fsa, const string& str, newState *current)
{
	if (!Contain_Sym(fsa, str.at(0))) return false;

	if (str.length() == 0) return current->accept;
	for (int i = 0; i < current->transition.size(); i++)
	{
		if (current->transition[i].first == str.at(0))
		{
			return goNextState(fsa, str.substr(1, str.length()), fsa.state_set[current->transition[i].second - 1]);
		}
	}
}

bool RunFSA(const FiniteStateAutomaton& fsa, const string& str) {
  // Implement this function.

  return goNextState(fsa,str,fsa.state_set[0]);
}

std::set<int> findClosure(const std::set<int> state, const std::vector<FSATableElement>& elements)
{
	std::set<int> closure;
	int num = 0;
	closure = state;

	while (true)
	{
		num = closure.size();
		for (int i = 0; i < elements.size(); i++)
		{
			for (std::set<int>::iterator it = closure.begin(); it != closure.end(); ++it)
				if (elements[i].str.length() == 0 && *it == elements[i].state)
				{
					closure.insert(elements[i].next_state);
				}
		}
		if (num == closure.size()) break;
	}
	return closure;
}
std::set<int> findNextstate(const std:: set<int> state, const std::vector<FSATableElement>&elements, char trans)
{
	std::set<int> nextstate;


	for (int i = 0; i < elements.size(); i++)
	{
		for (std::set<int>::iterator it = state.begin(); it != state.end(); ++it)
			if (*it == elements[i].state)
			{
				for (int j = 0; j<elements[i].str.length(); j++)
					if(trans == elements[i].str.at(j))
						nextstate.insert(elements[i].next_state);
			}
	}
	
	return nextstate;
}

bool checkAccept(FiniteStateAutomaton* fsa, newState* a)
{
	for (int i=0;i<fsa->accept_state.size();i++)
		for (std::set<int>::iterator it = a->element.begin(); it != a->element.end(); ++it)
		{
			if (*it == fsa->accept_state[i])
				return true;
		}
	return false;
}

int findSameState(FiniteStateAutomaton* fsa, newState* a)
{
	for (int i = 0; i < fsa->state_set.size(); i++)
	{
		if (fsa->state_set[i]->element == a->element)
			return i;
	}
	return -1;
}

void printFSA(FiniteStateAutomaton* fsa)
{
	newState *s;
	for (int i = 0; i < fsa->state_set.size(); i++)
	{
		s = fsa->state_set[i];
		printf("newstate num %d include elements", s->num);
		for (std::set<int>::iterator it = s->element.begin(); it != s->element.end(); ++it)
		{
			printf("%d, ", *it);
		}
		cout << endl;
		for (int j = 0; j < s->transition.size(); j++)
			printf("transtion [ %c ] [ %d ]\n", s->transition[j].first, s->transition[j].second);
		printf(" this state is ");
		if (s->accept) printf("accept\n");
		else printf("not accept\n");
	}
}

int makeNewState(const std::vector<FSATableElement>& elements
	,FiniteStateAutomaton* fsa, newState* before, char trans)
{
	newState *s = new newState;
	s->num = fsa->state_set.size() + 1;
	s->element = findClosure(findNextstate(before->element, elements, trans), elements);
	

	int t = findSameState(fsa, s);
	if (t != -1)
	{
		before->transition.push_back(make_pair(trans, fsa->state_set[t]->num));
		return before->num;
	}
	before->transition.push_back(make_pair(trans, s->num));
	fsa->state_set.push_back(s);
	s->accept = checkAccept(fsa, s);

	for (std::set<char>::iterator it = fsa->symbol.begin(); it != fsa->symbol.end(); ++it)
	{
		makeNewState(elements, fsa, s, *it);
	}
	return before->num;
}

bool BuildFSA(const std::vector<FSATableElement>& elements,
              const std::vector<int>& accept_states,
              FiniteStateAutomaton* fsa) {
  // Implement this function.
  LOG << "num_elements: " << elements.size()
      << ", accept_states: " << accept_states.size() << endl;

  int k=0;

  fsa->start_state = elements[0].state;
  for(int i=0;i<elements.size();i++)
	  for (int j = 0; j < elements[i].str.length(); j++)
	  {
		  fsa->symbol.insert(elements[i].str.at(j));
	  }
  for (int i = 0; i < accept_states.size(); i++)
	  fsa->accept_state.push_back(accept_states[i]);

  newState *start = new newState;
  start->num = 1;
  start->element.insert(fsa->start_state);
  start->element = findClosure(start->element, elements);
  start->accept = checkAccept(fsa, start);
  fsa->state_set.push_back(start);

  for (std::set<char>::iterator it = fsa->symbol.begin(); it != fsa->symbol.end(); ++it)
  {
	  k=makeNewState(elements, fsa, start, *it);
  }
  //printFSA(fsa);

  if (k == 1)
	  return true;

  return false;
}

