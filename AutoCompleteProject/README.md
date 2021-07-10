Old project of mine which i did way back in 2018

# Autocompletes words you give it with by parsing a word dictionary in a Deterministic Finite Automata

Run by passing the word file as an argument to the .exe

Tested with https://github.com/dwyl/english-words/blob/master/words.txt
Benchmark with this at the moment:
450k words are parsed for 70 sec and take ~600 mb - Could be better

#### Bugs:
* wifstream doesnt seem to parse utf symbols, i spend ~8 hours researching the
issue and found no solution (wcin, wcout work fine)
* No minimalization
* Might not run on Linux :/
* "!" symbol fuck up the program