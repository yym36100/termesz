#pragma once

typedef int(*tCmdFunc)(int argc, char *argv[]);

typedef struct stCmd{
	const char *name;
	tCmdFunc cmdFunc;

}tstCmd;