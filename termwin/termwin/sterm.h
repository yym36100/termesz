#pragma once
#include <string.h>

typedef int(*tCmdFunc)(int argc, char *argv[]);

typedef struct stCmd{
	const char *name;
	tCmdFunc cmdFunc;
}tstCmd;

class cCmdHandler{
public:
	tstCmd	*pCmds;
	int		size;

	cCmdHandler(tstCmd	*_pCmds,int _size): pCmds(_pCmds),size(_size){}

	int getCmdByName(char *name){
		for(int i=0;i<size;i++){
			if (strcmp(name,pCmds[i].name)==0)
				return i;
		}
		return -1;
	}
};