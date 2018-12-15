#include "p2Dictionary.h"
#include "p2SString.h"

template <class varType>
void p2Dictionary<varType>::AddEntry(p2SString name, varType value) {
	//TODO: Check for repeated names
	entries.add(entry<varType>(name,value));
}

template <class varType>
varType p2Dictionary<varType>::GetEntry(p2SString name) {
	for (p2List_item<varType> iterator = iterator != nullptr; iterator = iterator->next) {
		if (iterator->data->name->GetString() == name->GetString()) {
			return iterator->data->value;
		}
	}
	return nullptr;
}