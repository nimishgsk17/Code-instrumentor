#include <stdio.h>
#include <iostream>
#include <vector>
#include <queue>

#include "jugs.h"

using namespace std;


State::State() : prevState(NULL) {std::cout << "Visiting function 'State' at line: 11" << std::endl; std::cout << "Visiting constructor 'State' at line: 11" << std::endl; std::cout << "Visiting block at line: 11" << std::endl; 
}

State::State(State *state) : prevState(state) {std::cout << "Visiting function 'State' at line: 14" << std::endl; std::cout << "Visiting constructor 'State' at line: 14" << std::endl; std::cout << "Visiting block at line: 14" << std::endl; 
	contents = state->contents; 
}
	
State::~State() {std::cout << "Visiting function '~State' at line: 18" << std::endl; std::cout << "Visiting block at line: 18" << std::endl;  
	std::cout << "Visiting for loop at line: 19" << std::endl; for(unsigned int i = 0; i < nextStates.size(); i++) {std::cout << "Visiting block at line: 19" << std::endl; 
		delete(nextStates[i]);
	}
	nextStates.clear(); 
}

bool State::contains(unsigned int value) {std::cout << "Visiting function 'contains' at line: 25" << std::endl; std::cout << "Visiting block at line: 25" << std::endl;  
	std::cout << "Visiting for loop at line: 26" << std::endl; for(unsigned int i = 0; i < contents.size(); i++) {std::cout << "Visiting block at line: 26" << std::endl;  
		std::cout << "Visiting if statement at line: 27" << std::endl; if (contents[i] == value) {std::cout << "Visiting block at line: 27" << std::endl;  
			return true;
		}
	}
	return false;
}

bool State::operator == (State& s) {std::cout << "Visiting function 'operator==' at line: 34" << std::endl; std::cout << "Visiting block at line: 34" << std::endl;  
	std::cout << "Visiting if statement at line: 35" << std::endl; if(contents == s.contents) {std::cout << "Visiting block at line: 35" << std::endl;  
		return true;
	}
	return false;
}

vector<State *> State::getTrace() {std::cout << "Visiting function 'getTrace' at line: 41" << std::endl; std::cout << "Visiting block at line: 41" << std::endl;  
	vector<State *> trace; 
	std::cout << "Visiting if statement at line: 43" << std::endl; if(prevState) {std::cout << "Visiting block at line: 43" << std::endl;  
		vector<State *> ptrace = prevState->getTrace(); 
		std::cout << "Visiting for loop at line: 45" << std::endl; for(unsigned int i = 0; i < ptrace.size(); i++) {std::cout << "Visiting block at line: 45" << std::endl;  
			trace.push_back(ptrace[i]); 
		}
	}
	trace.push_back(this); 
	return trace;
}

// bool State::operator == (State& s) { 
// 	if(contents == s.contents) { 
// 		return true;
// 	}
// 	return false;
// }

void State::setContent(unsigned int value, unsigned int i) {std::cout << "Visiting function 'setContent' at line: 60" << std::endl; std::cout << "Visiting block at line: 60" << std::endl;  
	contents[i] = value; 
}

unsigned int State::getContent(unsigned int i) {std::cout << "Visiting function 'getContent' at line: 64" << std::endl; std::cout << "Visiting block at line: 64" << std::endl;  
	return contents[i];
}

void State::print() {std::cout << "Visiting function 'print' at line: 68" << std::endl; std::cout << "Visiting block at line: 68" << std::endl;  
	cout << '(';
	std::cout << "Visiting for loop at line: 70" << std::endl; for(unsigned int i = 0; i < contents.size(); i++) {std::cout << "Visiting block at line: 70" << std::endl;  
		cout << contents[i];
		std::cout << "Visiting if statement at line: 72" << std::endl; if(i < contents.size() - 1) {std::cout << "Visiting block at line: 72" << std::endl;  
			cout << ", ";
		}
	}
	cout << ')' << endl;
}

Process::Process(vector<unsigned int> c, unsigned int f) : capacities(c), final(f) {std::cout << "Visiting function 'Process' at line: 79" << std::endl; std::cout << "Visiting constructor 'Process' at line: 79" << std::endl; std::cout << "Visiting block at line: 79" << std::endl; 
	std::cout << "Visiting for loop at line: 80" << std::endl; for(unsigned int i = 0; i < capacities.size(); i++) {std::cout << "Visiting block at line: 80" << std::endl;  
		initial.contents.push_back(0); 
	}
}

Process::~Process() {std::cout << "Visiting function '~Process' at line: 85" << std::endl; std::cout << "Visiting block at line: 85" << std::endl; 
	std::cout << "Visiting for loop at line: 86" << std::endl; for(unsigned int i = 0; i < initial.nextStates.size(); i++) {std::cout << "Visiting block at line: 86" << std::endl;  
		delete(initial.nextStates[i]); 
	} 
	initial.nextStates.clear(); 
}
	
bool Process::isNew(State *root, State *s) {std::cout << "Visiting function 'isNew' at line: 92" << std::endl; std::cout << "Visiting block at line: 92" << std::endl;  
	std::cout << "Visiting if statement at line: 93" << std::endl; if((*root) == (*s)) {std::cout << "Visiting block at line: 93" << std::endl;  
		return false;
	}
	std::cout << "Visiting for loop at line: 96" << std::endl; for(unsigned int i = 0; i < root->nextStates.size(); i++) {std::cout << "Visiting block at line: 96" << std::endl;  
		std::cout << "Visiting if statement at line: 97" << std::endl; if(isNew(root->nextStates[i], s) == false) {std::cout << "Visiting block at line: 97" << std::endl;  
			return false;
		}
	}
	return true;
}

State *Process::empty(unsigned int i, State *state) {std::cout << "Visiting function 'empty' at line: 104" << std::endl; std::cout << "Visiting block at line: 104" << std::endl; 
	State *newstate = new State(state);
	newstate->setContent(0, i);
	return newstate;
}

State *Process::fill(unsigned int i, State* state) {std::cout << "Visiting function 'fill' at line: 110" << std::endl; std::cout << "Visiting block at line: 110" << std::endl; 
	State *newstate = new State(state);
	newstate->setContent(capacities[i], i);
	return newstate;
}

unsigned int Process::min(unsigned int a, unsigned int b) {std::cout << "Visiting function 'min' at line: 116" << std::endl; std::cout << "Visiting block at line: 116" << std::endl; 
	std::cout << "Visiting if statement at line: 117" << std::endl; if(a < b) {std::cout << "Visiting block at line: 117" << std::endl; 
		return a;
	}
	return b;
}

State *Process::transfer(unsigned int i, unsigned int j, State *state) {std::cout << "Visiting function 'transfer' at line: 123" << std::endl; std::cout << "Visiting block at line: 123" << std::endl; 
	State *newstate = new State(state);
	unsigned int amount = min(state->getContent(i), capacities[j] - state->getContent(j));
	newstate->setContent(state->getContent(i) - amount, i);
	newstate->setContent(state->getContent(j) + amount, j);
	return newstate;
}

State *Process::finishUp(State *state, unsigned int final) {std::cout << "Visiting function 'finishUp' at line: 131" << std::endl; std::cout << "Visiting block at line: 131" << std::endl; 
	std::cout << "Visiting if statement at line: 132" << std::endl; if(state->contains(final)) {std::cout << "Visiting block at line: 132" << std::endl; 
		State *anotherstate = state;
		std::cout << "Visiting for loop at line: 134" << std::endl; for(unsigned j = 0; j < capacities.size(); j++) {std::cout << "Visiting block at line: 134" << std::endl; 
			std::cout << "Visiting if statement at line: 135" << std::endl; if(anotherstate->getContent(j) != final) {std::cout << "Visiting block at line: 135" << std::endl; 
				State *s = empty(j, anotherstate);
				anotherstate->nextStates.push_back(s);
				anotherstate = s;
			}
		}
		return anotherstate;
	}
	return state;
}

State *Process::singleStep() {std::cout << "Visiting function 'singleStep' at line: 146" << std::endl; std::cout << "Visiting block at line: 146" << std::endl; 
	State *state = theQ.front();
	std::cout << "Visiting for loop at line: 148" << std::endl; for(unsigned int i = 0; i < capacities.size(); i++) {std::cout << "Visiting block at line: 148" << std::endl; 
		State *next = empty(i, state);
		std::cout << "Visiting if statement at line: 150" << std::endl; if(isNew(&initial, next)) {std::cout << "Visiting block at line: 150" << std::endl; 
			theQ.push(next);
			state->nextStates.push_back(next);
			std::cout << "Visiting if statement at line: 153" << std::endl; if(next->contains(final)) {std::cout << "Visiting block at line: 153" << std::endl; 
				return finishUp(next, final);
			}
		}
		else {std::cout << "Visiting block at line: 157" << std::endl; 
			delete next;

		}
	}
	std::cout << "Visiting for loop at line: 162" << std::endl; for(unsigned int i = 0; i < capacities.size(); i++) {std::cout << "Visiting block at line: 162" << std::endl; 
		State *next = fill(i, state);
		std::cout << "Visiting if statement at line: 164" << std::endl; if(isNew(&initial, next)) {std::cout << "Visiting block at line: 164" << std::endl; 
			theQ.push(next);
			state->nextStates.push_back(next);
			std::cout << "Visiting if statement at line: 167" << std::endl; if(next->contains(final)) {std::cout << "Visiting block at line: 167" << std::endl; 
				return finishUp(next, final);
			}
		}
		else {std::cout << "Visiting block at line: 171" << std::endl; 
			delete next;
		}
	}
	std::cout << "Visiting for loop at line: 175" << std::endl; for(unsigned int i = 0; i < capacities.size(); i++) {std::cout << "Visiting block at line: 175" << std::endl;  
		std::cout << "Visiting for loop at line: 176" << std::endl; for(unsigned int j = 0; j < capacities.size(); j++) {std::cout << "Visiting block at line: 176" << std::endl; 
			std::cout << "Visiting if statement at line: 177" << std::endl; if(i != j) {std::cout << "Visiting block at line: 177" << std::endl; 
				State *next = transfer(i, j, state);
				std::cout << "Visiting if statement at line: 179" << std::endl; if(isNew(&initial, next)) {std::cout << "Visiting block at line: 179" << std::endl; 
					theQ.push(next);
					state->nextStates.push_back(next);
					std::cout << "Visiting if statement at line: 182" << std::endl; if(next->contains(final)) {std::cout << "Visiting block at line: 182" << std::endl; 
						return finishUp(next, final);
					}
				}
				else {std::cout << "Visiting block at line: 186" << std::endl; 
					delete(next);
				}
			}
		}
	}
	theQ.pop();
	return NULL;
}

void Process::go() {std::cout << "Visiting function 'go' at line: 196" << std::endl; std::cout << "Visiting block at line: 196" << std::endl; 
	theQ.push(&initial);
	std::cout << "Visiting while loop at line: 198" << std::endl; while(!theQ.empty()) {std::cout << "Visiting block at line: 198" << std::endl; 
		State *state = singleStep();
		std::cout << "Visiting if statement at line: 200" << std::endl; if(state != NULL) {std::cout << "Visiting block at line: 200" << std::endl; 
			vector<State *> trace = state->getTrace();
			std::cout << "Visiting for loop at line: 202" << std::endl; for(unsigned int i = 0; i < trace.size(); i++) {std::cout << "Visiting block at line: 202" << std::endl; 
				trace[i]->print();
			}
			return;
		}
	}
	cout << "Couldn't find solution!" << endl;
}

Process *readInput(int argc, char **argv) {std::cout << "Visiting function 'readInput' at line: 211" << std::endl; std::cout << "Visiting block at line: 211" << std::endl; 
vector<unsigned int> capacities;
unsigned int final;
	std::cout << "Visiting for loop at line: 214" << std::endl; for(int i = 0; i < argc - 2; i++) {std::cout << "Visiting block at line: 214" << std::endl;  
		unsigned int capacity;
		sscanf(argv[i + 1], "%u", &capacity);
		capacities.push_back(capacity);
	}

	sscanf(argv[argc - 1], "%u", &final);
	return new Process(capacities, final);
}
