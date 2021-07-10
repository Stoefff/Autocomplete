#pragma once

#include <string>
//#include <Vector>
#include <unordered_map>
#include <unordered_set>

using std::wstring;
using std::pair;
using std::unordered_map;
using std::unordered_set;
//using std::vector;

class Completer {
public:
	Completer();
	Completer(const Completer& other) = delete;
	Completer& operator=(const Completer& rhs) = delete;
	~Completer();

	void insertWord(const wstring& word);
	void suggestWords(const wstring& prefix);
	void minimize();

	void setSuggestCount(const size_t count);
	size_t getSuggestCount() const;

	void printStates(); //wrapper

private:
	struct State {
		int id;
		bool isFinal;

		//maybe get rid of the default and fix addSuffix
		//insert not working
		State() :id(-1), isFinal(false) {}

		State(const int id, bool isFinal = false):
			id(id),
			isFinal(isFinal){
		}
		//Making State hashable
		bool operator==(const State& other) const {
			return this->id == other.id;
		}
	};
	//Making State hashable part 2
	struct hashFunc {
		size_t operator() (const State& state) const {
			return std::hash<size_t>()(state.id);
		}
	};
	using Char = wchar_t;
	using Edges = unordered_map<Char, State>;
	using DFA = unordered_map<State, Edges, hashFunc>;

private:
	State delta(const State& state, const Char& letter) const;
	State delta(const State& state, const wstring& word) const;
	State getPrefixLastState(const wstring& prefix, size_t& index) const;
	void addSuffix(State& start, const wstring& suffix);
	void printSuggestedWords(State curr, const wstring prefix, size_t& counter);
	void printStates(const State curr);

private:
	DFA automata;
	const State beginState;
	const State errorState;

	size_t stateCount;
	size_t suggestCount;
};