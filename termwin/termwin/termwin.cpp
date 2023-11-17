
#include <stdio.h>
#include "sterm.h"

int f1(int argc, char* argv[]){ return 1;}
int f2(int argc, char* argv[]){ return 2;}

int f_add(int argc, char* argv[]){
	if(argc!=3) printf("wrong usage\n");
	return ((*argv[1])-'0')+((*argv[2])-'0');
}

char test_args[]="add    4		8    ";
tstCmd astCmds[]={
	{"first",(tCmdFunc)f1},
	{"second",(tCmdFunc)f2},
	{"add",(tCmdFunc)f_add},
};

cCmdHandler CmdH(astCmds,sizeof(astCmds)/sizeof(astCmds[0]));

int main(int argc, char* argv[])
{	
	int a,b;
	b = CmdH.exec(test_args);
	CmdH.listAllCmds();
	a = CmdH.getCmdByName("second");
	printf("a=%d\n",a);
	printf("cmd func result=%d\n",b);
	return 0;
}

