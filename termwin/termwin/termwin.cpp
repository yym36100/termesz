
#include <stdio.h>
#include <stdlib.h>

#include <conio.h>

#include "sterm.h"
#include "ringbuff.h"

#define OPTPARSE_IMPLEMENTATION
#include "optparse.h"

#include "hist.h"
#include "lineedit.h"



int f1(int argc, char* argv[]){ return 1;}
int f2(int argc, char* argv[]){ return 2;}
int donut_main(int argc, char **argv);

int f_quit(int argc, char* argv[]){ exit(0);return 2;}

int f_add(int argc, char* argv[]){
	if(argc!=3) {
		printf("wrong usage\n");
		return -1;
	}
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
//            fprintf(stderr, "%s: %s\n", argv[0], options.errmsg);
            exit(0);
        }
    }

    /* Print remaining arguments. */
    while ((arg = optparse_arg(&options)))
		printf("remaining args: %s\n", arg);
    return 0;
}

int f_help(int argc, char* argv[]);
int f_hist(int argc, char* argv[]);

char test_args[]="add 1 2";
tstCmd astCmds[]={
	{"first",(tCmdFunc)f1},
	{"second",(tCmdFunc)f2},
	{"add",(tCmdFunc)f_add},
	{"opt_main",(tCmdFunc)opt_main},
	{"help",(tCmdFunc)f_help},
	{"hello",(tCmdFunc)f_help},
	{"quit",(tCmdFunc)f_quit},
	{"hist",(tCmdFunc)f_hist},
	{"donut",(tCmdFunc)donut_main},
};

cCmdHandler CmdH(astCmds,sizeof(astCmds)/sizeof(astCmds[0]));

cLineEdit lineEdit;
cstrHist h;

int f_help(int argc, char* argv[]){
	CmdH.listAllCmds();
	return 0;
}
int f_hist(int argc, char* argv[]){
	h.list_all();
	return 0;
}




int main(int argc, char* argv[])
{
	lineEdit.pCmdH = &CmdH;
	lineEdit.pHist = &h;
	char a;
	while(1){
		a=_getch();
		lineEdit.handle_char(a);
		//printf("%02x ",a);
	}
#if 0
	char t[10];
	int a;
	cstrHist h;
	h.add(test_args);
//	h.list_all();

	CmdH.findmatch("h");
	CmdH.exec("help");
	h.add("help");
	a=CmdH.exec(test_args);
	h.add(test_args);

	printf("a=%d\n",a);

	h.list_all();


//#if 0
	printf("add abc ---------\n");
	termHist.Addn("abc", 3);
	termHist.list();
	termHist.Removen(t,3);	
	printf("add 123 ---------\n");
	termHist.Addn("123", 3);
	termHist.list();

	termHist.Addn("A", 1);
	termHist.Remove(t);
	termHist.Remove(t);
	
	termHist.Addn("mn", 2);
	termHist.list();
	

	int a,b;
	b = CmdH.exec(test_args);
	CmdH.listAllCmds();
	a = CmdH.getCmdByName("second");
	printf("a=%d\n",a);
	printf("cmd func result=%d\n",b);
	return 0;
#endif
}

