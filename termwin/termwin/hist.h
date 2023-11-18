#pragma once

#include "ringbuff.h"


class cHistory{
public:
	enum{noEntries = 10, avgEntrySize = 25};
	cringbuff<char, noEntries * avgEntrySize> texts;
	cringbuff<uint16_t, noEntries> entries;

	bool add(char *t){
		uint16_t tmp;
		if(entries.getSize()==noEntries){
			entries.Remove(&tmp);
		}
		entries.Add(3);

		return true;
	}
};


