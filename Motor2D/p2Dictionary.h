#ifndef _P2_DICTIONARY_H_
#define _P2_DICTIONARY_H_

#include "p2SString.h"
#include "p2List.h"

template<class varType>
class entry {
	//Variables
private:
	p2SString name;
	varType value;

	//Methods
public:
	entry(p2SString name, varType value);
};

//A class that associates a name with a variable
template<class varType>
class p2Dictionary {
	//Variables
private:
	p2List<entry<varType>> entries;

	//Methods
public:
	void AddEntry(p2SString name, varType value);
	varType GetEntry(p2SString name);
};

#endif
