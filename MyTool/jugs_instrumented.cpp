#include <stdio.h>
#include <iostream>
#include <vector>
#include <queue>

#include "jugs.h"

using namespace std;


State::State() : prevState(NULL) {
}

State::State(State *state) : prevState(std::cout << "Line 14 executed." << std::endl;state) {
        contents = state->contents; 
}

State::~State() { 
        std::cout << "Line 19 executed." << std::endl;for(unsigned int i = 0; i < nextStates.size(); i++) {
                delete(nextStates[i]);
        }
        nextStates.clear(); 
}

bool State::contains(unsigned int value) { 
        std::cout << "Line 26 executed." << std::endl;for(unsigned int i = 0; i < contents.size(); i++) { 
                std::cout << "Line 27 executed." << std::endl;if (contents[i] == value) { 
                        std::cout << "Line 28 executed." << std::endl;return true;
                }
        }
        std::cout << "Line 31 executed." << std::endl;return false;
}

bool State::operator == (State& s) { 
        std::cout << "Line 35 executed." << std::endl;if(contents == s.contents) { 
                std::cout << "Line 36 executed." << std::endl;return true;
        }
        std::cout << "Line 38 executed." << std::endl;return false;
}

vector<State *> State::getTrace() { 
        std::cout << "Line 42 executed." << std::endl;vector<State *> trace; 
        std::cout << "Line 43 executed." << std::endl;if(prevState) { 
                std::cout << "Line 44 executed." << std::endl;vector<State *> ptrace = prevState->getTrace(); 
                std::cout << "Line 45 executed." << std::endl;for(unsigned int i = 0; i < ptrace.size(); i++) { 
                        std::cout << "Line 46 executed." << std::endl;trace.push_back(ptrace[i]); 
                }
        }
        std::cout << "Line 49 executed." << std::endl;trace.push_back(this); 
        std::cout << "Line 50 executed." << std::endl;return trace;
}

// bool State::operator == (State& s) { 
//      if(contents == s.contents) { 
//              return true;
//      }
//      return false;
// }

void State::setContent(unsigned int value, unsigned int i) { 
        contents[i] = value; 
}

unsigned int State::getContent(unsigned int i) { 
        return contents[i];
}

void State::print() { 
        std::cout << "Line 69 executed." << std::endl;cout << '(';
        std::cout << "Line 70 executed." << std::endl;for(unsigned int i = 0; i < contents.size(); i++) { 
                cout << contents[i];
                std::cout << "Line 72 executed." << std::endl;if(i < contents.size() - 1) { 
                        std::cout << "Line 73 executed." << std::endl;cout << ", ";
                }
        }
        std::cout << "Line 76 executed." << std::endl;cout << ')' << endl;
}

Process::Process(vector<unsigned int> c, unsigned int f) : capacities(c), final(std::cout << "Line 79 executed." << std::endl;f) {
        std::cout << "Line 80 executed." << std::endl;for(unsigned int i = 0; i < capacities.size(); i++) { 
                initial.contents.push_back(0); 
        }
}

Process::~Process() {
        std::cout << "Line 86 executed." << std::endl;for(unsigned int i = 0; i < initial.nextStates.size(); i++) { 
                delete(initial.nextStates[i]); 
        } 
        initial.nextStates.clear(); 
}

bool Process::isNew(State *root, State *s) { 
        std::cout << "Line 93 executed." << std::endl;if((*root) == (*s)) { 
                std::cout << "Line 94 executed." << std::endl;return false;
        }
        std::cout << "Line 96 executed." << std::endl;for(unsigned int i = 0; i < root->nextStates.size(); i++) { 
                std::cout << "Line 97 executed." << std::endl;if(isNew(root->nextStates[i], s) == false) { 
                        std::cout << "Line 98 executed." << std::endl;return false;
                }
        }
        std::cout << "Line 101 executed." << std::endl;return true;
}

State *Process::empty(unsigned int i, State *state) {
        std::cout << "Line 105 executed." << std::endl;State *newstate = new State(state);
        std::cout << "Line 106 executed." << std::endl;newstate->setContent(0, i);
        std::cout << "Line 107 executed." << std::endl;return newstate;
}

State *Process::fill(unsigned int i, State* state) {
        std::cout << "Line 111 executed." << std::endl;State *newstate = new State(state);
        newstate->setContent(capacities[i], i);
        std::cout << "Line 113 executed." << std::endl;return newstate;
}

unsigned int Process::min(unsigned int a, unsigned int b) {
        std::cout << "Line 117 executed." << std::endl;if(a < b) {
                std::cout << "Line 118 executed." << std::endl;return a;
        }
        std::cout << "Line 120 executed." << std::endl;return b;
}

State *Process::transfer(unsigned int i, unsigned int j, State *state) {
        std::cout << "Line 124 executed." << std::endl;State *newstate = new State(state);
        std::cout << "Line 125 executed." << std::endl;unsigned int amount = min(state->getContent(i), capacities[j] - state->getContent(j));
        std::cout << "Line 126 executed." << std::endl;newstate->setContent(state->getContent(i) - amount, i);
        std::cout << "Line 127 executed." << std::endl;newstate->setContent(state->getContent(j) + amount, j);
        std::cout << "Line 128 executed." << std::endl;return newstate;
}

State *Process::finishUp(State *state, unsigned int final) {
        std::cout << "Line 132 executed." << std::endl;if(state->contains(final)) {
                std::cout << "Line 133 executed." << std::endl;State *anotherstate = state;
                std::cout << "Line 134 executed." << std::endl;for(unsigned j = 0; j < capacities.size(); j++) {
                        std::cout << "Line 135 executed." << std::endl;if(anotherstate->getContent(j) != final) {
                                std::cout << "Line 136 executed." << std::endl;State *s = empty(j, anotherstate);
                                std::cout << "Line 137 executed." << std::endl;anotherstate->nextStates.push_back(s);
                                std::cout << "Line 138 executed." << std::endl;anotherstate = s;
                        }
                }
                std::cout << "Line 141 executed." << std::endl;return anotherstate;
        }
        std::cout << "Line 143 executed." << std::endl;return state;
}

State *Process::singleStep() {
        std::cout << "Line 147 executed." << std::endl;State *state = theQ.front();
        std::cout << "Line 148 executed." << std::endl;for(unsigned int i = 0; i < capacities.size(); i++) {
                std::cout << "Line 149 executed." << std::endl;State *next = empty(i, state);
                std::cout << "Line 150 executed." << std::endl;if(isNew(&initial, next)) {
                        theQ.push(next);
                        std::cout << "Line 152 executed." << std::endl;state->nextStates.push_back(next);
                        std::cout << "Line 153 executed." << std::endl;if(next->contains(final)) {
                                std::cout << "Line 154 executed." << std::endl;return finishUp(next, final);
                        }
                }
                else {
                        std::cout << "Line 158 executed." << std::endl;delete next;

                }
        }
        std::cout << "Line 162 executed." << std::endl;for(unsigned int i = 0; i < capacities.size(); i++) {
                std::cout << "Line 163 executed." << std::endl;State *next = fill(i, state);
                std::cout << "Line 164 executed." << std::endl;if(isNew(&initial, next)) {
                        theQ.push(next);
                        std::cout << "Line 166 executed." << std::endl;state->nextStates.push_back(next);
                        std::cout << "Line 167 executed." << std::endl;if(next->contains(final)) {
                                std::cout << "Line 168 executed." << std::endl;return finishUp(next, final);
                        }
                }
                else {
                        std::cout << "Line 172 executed." << std::endl;delete next;
                }
        }
        std::cout << "Line 175 executed." << std::endl;for(unsigned int i = 0; i < capacities.size(); i++) { 
                std::cout << "Line 176 executed." << std::endl;for(unsigned int j = 0; j < capacities.size(); j++) {
                        std::cout << "Line 177 executed." << std::endl;if(i != j) {
                                std::cout << "Line 178 executed." << std::endl;State *next = transfer(i, j, state);
                                std::cout << "Line 179 executed." << std::endl;if(isNew(&initial, next)) {
                                        theQ.push(next);
                                        std::cout << "Line 181 executed." << std::endl;state->nextStates.push_back(next);
                                        std::cout << "Line 182 executed." << std::endl;if(next->contains(final)) {
                                                std::cout << "Line 183 executed." << std::endl;return finishUp(next, final);
                                        }
                                }
                                else {
                                        std::cout << "Line 187 executed." << std::endl;delete(next);
                                }
                        }
                }
        }
        theQ.pop();
        return NULL;
}

void Process::go() {
        theQ.push(&initial);
        std::cout << "Line 198 executed." << std::endl;while(!theQ.empty()) {
                std::cout << "Line 199 executed." << std::endl;State *state = singleStep();
                std::cout << "Line 200 executed." << std::endl;if(state != NULL) {
                        std::cout << "Line 201 executed." << std::endl;vector<State *> trace = state->getTrace();
                        std::cout << "Line 202 executed." << std::endl;for(unsigned int i = 0; i < trace.size(); i++) {
                                std::cout << "Line 203 executed." << std::endl;trace[i]->print();
                        }
                        std::cout << "Line 205 executed." << std::endl;return;
                }
        }
        std::cout << "Line 208 executed." << std::endl;cout << "Couldn't find solution!" << endl;
}

Process *readInput(int argc, char **argv) {
std::cout << "Line 212 executed." << std::endl;vector<unsigned int> capacities;
std::cout << "Line 213 executed." << std::endl;unsigned int final;
        std::cout << "Line 214 executed." << std::endl;for(int i = 0; i < argc - 2; i++) { 
                std::cout << "Line 215 executed." << std::endl;unsigned int capacity;
                std::cout << "Line 216 executed." << std::endl;sscanf(argv[i + 1], "%u", &capacity);
                std::cout << "Line 217 executed." << std::endl;capacities.push_back(capacity);
        }

        std::cout << "Line 220 executed." << std::endl;sscanf(argv[argc - 1], "%u", &final);
        std::cout << "Line 221 executed." << std::endl;return new Process(capacities, final);
}