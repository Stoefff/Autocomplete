#include "Completer.hpp"
#include <iostream>

using std::wcout;
using std::endl;
using std::make_pair;
using std::wcerr;

const size_t defSuggCount = 5;

Completer::Completer()
	:suggestCount(defSuggCount),
	stateCount(0),
	beginState(0),
	errorState(-1){
	stateCount++;
	automata[beginState]; //insert
}

Completer::~Completer() {}

Completer::State Completer::delta(const State& state,const Char& letter) const {
	DFA::const_iterator dfaIt = automata.find(state);
	if (dfaIt == automata.end())
		return errorState;
	Edges::const_iterator edgeIt = dfaIt->second.find(letter);
	if (edgeIt == dfaIt->second.end())
		return errorState;
	return edgeIt->second;
}

Completer::State Completer::delta(const State& state, const wstring& word) const {
	State currState(state.id);
	for (const Char& letter : word) {
		currState = delta(currState, letter);
		if (currState == errorState)
			return errorState;
	}
	return currState;
}

Completer::State Completer::getPrefixLastState(const wstring& prefix, size_t& index) const {
	index = 0;
	State currState = beginState;
	for (const Char& letter : prefix) {
		State next = delta(currState, letter);
		if (next == errorState) {
			return currState;
		}
		currState = next;
		index++;
	}
	return currState;
}

void Completer::insertWord(const wstring& word) {
	size_t prefixIndex = 0;
	State prefixLast = getPrefixLastState(word, prefixIndex);
	wstring suffix = word.substr(prefixIndex);
	//wcout << "   insertWord: " << suffix << suffix.length() << L"  " << prefixLast.id << endl;
	if (suffix.empty() && (prefixLast.isFinal == true)) {
		return;
	}
	addSuffix(prefixLast, suffix);
}


// undordered_map.insert throw a strange exception
void Completer::addSuffix(State& current, const wstring& suffix) {
	if (suffix.empty()) {
		current.isFinal = true;
		return;
	}
	size_t counter = 1;
	size_t size = suffix.length();
	for (const Char& letter : suffix) {
		State newState(stateCount++);
		if (counter == size) {
			newState.isFinal = true;
		}
		//automata[current][letter] = newState;
		DFA::iterator temp = automata.find(current);
		temp->second.insert({ letter, newState });
		current = newState;
		counter++;
	}
}

void Completer::suggestWords(const wstring& word){
	size_t prefixIndex = 0;
	State prefixLast = getPrefixLastState(word, prefixIndex);
	//Test index and statement validity, -1 maybe
	wstring prefix = word.substr(0, prefixIndex);
	if (prefixIndex != word.length()
		&& delta(prefixLast, word[prefixIndex]) == errorState) {
		wcerr << L"No suggestion found" << endl;
		return;
	}
	size_t counter = 0;
	printSuggestedWords(prefixLast, prefix, counter);
}

void Completer::printSuggestedWords(State curr, const wstring word, size_t& counter){
	if (counter >= suggestCount)
		return;
	if (curr.isFinal == true) {
		wcout << L"  " <<word << endl;
		++counter;
	}
	Edges::const_iterator it = automata[curr].begin();
	//The strange suggestions come from here
	for (it; it != automata[curr].end(); ++it)
		printSuggestedWords(it->second, word + it->first, counter);
}


//god help me
void Completer::minimize() {

}

void Completer::printStates() {
	printStates(beginState);
}

void Completer::printStates(const State curr){
	wcout << L"Current state: " << curr.id << L" and isFinal: "
		<< curr.isFinal <<  L" with edges: " << endl;
	Edges::const_iterator it = automata[curr].begin();
	for (it; it != automata[curr].end(); ++it){
 		wcout << L"		Edge with value " << it->first << L" to state "
		<< it->second.id << endl;
	}
	wcout << L"Stoped wcouting the " << curr.id << L" state." << endl;
	it = automata[curr].begin();
	for (it; it != automata[curr].end(); ++it)
		printStates(it->second);
}

void Completer::setSuggestCount(const size_t count) {
	this->suggestCount = count;
}

size_t Completer::getSuggestCount() const {
	return this->suggestCount;
}
