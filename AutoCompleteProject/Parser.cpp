#include "Parser.hpp"
#include <iostream>
#include <fstream>
#include <ctime>
#include <locale>
#include <codecvt>

using std::wifstream;
using std::getline;
using std::wcin;
using std::wcout;
using std::endl;
using std::wcerr;

const size_t defaultMinWordSize = 3;

Parser::Parser(Completer& completer) : completer(completer),
	minWordSize(defaultMinWordSize){
}

Parser::~Parser() {}

bool Parser::parseFile(const string fileName) {
	time_t startTime = time(NULL);
	// Open in binary because UTF requires so
	wifstream file(fileName, std::ios::binary);
	if (!file.good())
		return false;
	//file.imbue(std::locale(file.getloc(),new std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header>));
	wcout << L"Parsing words from file..." << endl;
	wstring buffer;
	while (getline(file, buffer)) {
//		buffer = buffer.substr(0, buffer.size() - 1); // trimming the /r
/*		Parses also an /r cartrige return ????????????
		wcout << L"         word " << buffer << endl;
		for (const wchar_t& letter : buffer) {
			wcout << letter;
		}
		wcout << buffer.length() << endl;
*/
		completer.insertWord(buffer);
		//completer.printStates();
	}
	file.close();
	completer.minimize();
	wcout << L"Parsed the words in "
		<< time(NULL) - startTime << L"s." << endl;
	return true;
}

void Parser::parseCMDinput() {
	wstring input;
	wcout << L"Commands: " << endl
		<< L"!suggestCount NUMBER - change how many words are suggested. "
		<< L"Default is 5" << endl
		<< L"!minWordSize NUMBER - minimal word length for autocompletion. "
		<< L"Default is 3" << endl
		<< L"!insert WORD" << endl
		<< L"!quit" << endl << endl;
	while (true) {
		wcout << L"$ ";
		wcin >> input;
		if (input == L"!suggestCount") {
			size_t newCount;
			wcin >> newCount;
			completer.setSuggestCount(newCount);
			wcout << completer.getSuggestCount()
				<< " is the new suggest count"<< endl;
		}
		else if ((input == L"!insert")) {
			wstring word;
			wcin >> word;
			completer.insertWord(word);
			wcout << word << " was inserted" << endl;
		}
		else if ((input == L"!minWordSize")) {
			size_t newSize;
			wcin >> newSize;
			minWordSize = newSize;
			wcout << minWordSize
				<< " is the new minimal size for a word" << endl;
		}
		else if ((input == L"!quit")) {
			break;
		}
		else {
			if (input.length() < minWordSize) {
				wcerr << L"Word is not long enough!" << endl;
			}
			else {
				completer.suggestWords(input);
			}
		}
	}
}

