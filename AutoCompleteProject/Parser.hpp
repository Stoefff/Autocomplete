#pragma once

#include <string>
#include "Completer.hpp"

using std::string;
using std::wstring;

class Parser {
public:
	Parser(Completer& completer);
	Parser(const Parser& other) = delete;
	Parser& operator=(const Parser& rhs) = delete;
	~Parser();

	bool parseFile(const string fileName);
	void parseCMDinput();

private:
	Completer& completer;
	size_t minWordSize;
};
