#include <iostream>
#include <string>

//Support for console settings
#include <fcntl.h>
//#include <io.h>

#include "Parser.hpp"
#include "Completer.hpp"

using std::wcout;
using std::wcin;
using std::endl;
using std::wcerr;
using std::string;
using std::wstring; //wide string for UTF-16 support

int main(int argc, char** argv) {

	//Setting VS console for UTF-16 io support
	//_setmode(_fileno(stdout), _O_U16TEXT);
	//_setmode(_fileno(stdin), _O_U16TEXT);

//	wstring test;
//	wcin >> test;
//	wcout << test << endl;


	if ((argc < 1) || (!argv[1])) {
		wcerr << L"No valid argument!" << endl;
		return -1;
	}

	Completer completer;
	Parser parser(completer);
	if (!parser.parseFile(argv[1])) { // auto casted
		wcerr << L"No such file or file stream failed!" << endl;
		return -1;
	}
	//completer.printStates();
	parser.parseCMDinput();
	
	return 0;
}