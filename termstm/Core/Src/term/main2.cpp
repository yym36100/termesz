
#include "sterm.h"

extern "C" void main_init(void){
}

extern "C" void main_loop(void);


#include <stdio.h>
#include <stdlib.h>

#include "sterm.h"

#define OPTPARSE_IMPLEMENTATION
#include "optparse.h"

int f1(int argc, char* argv[]){ return 1;}
int f2(int argc, char* argv[]){ return 2;}

int f_add(int argc, char* argv[]){
	if(argc!=3) printf("wrong usage\n");
	return ((*argv[1])-'0')+((*argv[2])-'0');
}

int opt_main(int argc, char **argv){
    bool amend = false;
    bool brief = false;
    const char *color = "white";
    int delay = 0;

    char *arg;
    int option;
    struct optparse options;

    (void)argc;
    optparse_init(&options, argv);
    while ((option = optparse(&options, "abc:d::")) != -1) {
        switch (option) {
        case 'a':
            amend = true;
			printf("opt a ok\n");
            break;
        case 'b':
            brief = true;
			printf("opt b ok\n");
            break;
        case 'c':
            color = options.optarg;
			printf("opt c=%s ok\n",color);
            break;
        case 'd':
            delay = options.optarg ? atoi(options.optarg) : 1;
			printf("opt d=%d ok\n",delay);
            break;
        case '?':
           // fprintf(stderr, "%s: %s\n", argv[0], options.errmsg);
            exit(0);
        }
    }

    /* Print remaining arguments. */
    while ((arg = optparse_arg(&options)))
		printf("remaining args: %s\n", arg);
    return 0;
}


char test_args[]="opt_main    -a	-b 9 -cszep	-d8    ";
tstCmd astCmds[]={
	{"first",(tCmdFunc)f1},
	{"second",(tCmdFunc)f2},
	{"add",(tCmdFunc)f_add},
	{"opt_main",(tCmdFunc)opt_main},
};

cCmdHandler CmdH(astCmds,sizeof(astCmds)/sizeof(astCmds[0]));

void main_loop()
{
	int a,b;
	b = CmdH.exec(test_args);
	CmdH.listAllCmds();
	a = CmdH.getCmdByName("second");
	printf("a=%d\n",a);
	printf("cmd func result=%d\n",b);
	return;
}

