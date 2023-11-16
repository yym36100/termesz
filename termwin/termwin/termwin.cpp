
#include <stdio.h>
#include "sterm.h"

int f1(int argc, char* argv[]){ return 1;}
int f2(int argc, char* argv[]){ return 2;}

tstCmd astCmds[]={
	{"first",(tCmdFunc)f1},
	{"second",(tCmdFunc)f2},
};

cCmdHandler CmdH(astCmds,sizeof(astCmds)/sizeof(astCmds[0]));

int main(int argc, char* argv[])
{	
	int a;
	a = CmdH.getCmdByName("second");
	printf("a=%d\n",a);
	printf("cmd func result=%d\n",astCmds[1].cmdFunc(0,0));
	return 0;
}

