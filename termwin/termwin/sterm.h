#pragma once
#include <string.h>
#include <stdio.h>

typedef int(*tCmdFunc)(int argc, char *argv[]);

typedef struct stCmd{
	const char *name;
	tCmdFunc cmdFunc;
}tstCmd;

class cCmdHandler{
public:
	enum {maxArgs = 10,};
	char *pArgs[maxArgs];
	int argc;
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
	void listAllCmds(void){
		printf("All available commands:\n");
		for(int i=0;i<size;i++){
			printf("\t%s\n",pCmds[i].name);
		}		
	}

	bool isPrefix(const char* str1, const char* str2) {
		while (*str1 != '\0') {
			// If characters don't match or str2 is shorter than str1
			if (*str1 != *str2 || *str2 == '\0') {
				return false;
			}
			str1++;
			str2++;
		}
		// All characters matched till the end of str1
		return true;
	}

	int findmatch(char *str){
		printf("find match for %s\n", str);
		int j=-1;
		for(int i=0;i<size;i++){
			if(isPrefix(str,pCmds[i].name)){
				printf("%s -> %s\n", str, pCmds[i].name);
				j=i;
			}
		}
		return j;
	}

	void tokenize(char *str){
		argc = 0;
		int tokenfound = 0;
		char *tokenStart = str;
		char br = 0;
		while(argc<maxArgs){
			if(*str==0) br=1;
			if(*str<0x21){
				//whitespace char found this delimits the tokens				
				if(tokenfound){
					pArgs[argc++] = tokenStart;
					tokenfound = 0;
					if(*str) *str = 0;
				}				
			} else { //normal char
				if(!tokenfound){
					tokenfound = 1;
					tokenStart = str;
				}
			}				
			if(br)break;
			str++;
		}		
	}

	int exec(char *str){
		int res=-1;
		if(*str==0){
			//handle empty command, nothing
			printf("\n");
			return 0;
		}
		printf("exec %s\n",str);
		tokenize(str);
		printf("argc=%d\n",argc);
		for(int i=0;i<argc;i++){
			printf("argv[%d] = >%s<\n",i,pArgs[i]);
		}

		int c = getCmdByName(pArgs[0]);
		if(c!=-1){
			res = pCmds[c].cmdFunc(argc,pArgs);
		} else {
			printf("command not found, see help for available commands\n");
		}
		return res;
	}
};