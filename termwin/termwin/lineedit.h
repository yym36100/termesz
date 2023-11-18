#pragma once


class cstrHist;
class cCmdHandler;

class cLineEdit{
public:
	enum {buff_size = 10};
	char buff[buff_size];
	uint8_t i;
	uint16_t hi;

	cstrHist *pHist;
	cCmdHandler *pCmdH;

	cLineEdit() :i(0), hi(0){
		memset(buff,0,buff_size);
	}



	void handle_char(char c){
		if((c>=0x20)&(c<0x7f)){
			if(i>=buff_size-1) return;
			buff[i] = c;
			printf("%d:%s\n",i,buff);
			if(i<(buff_size-1))	i++;
		} else if(c==0x8){
			if(i>0){
			buff[--i] = 0;
			printf("%d:%s\n",i,buff);
			}
		} else if(c==0x9){
			//tab autocomplete
			pCmdH->findmatch(buff);
			/*
			memset(buff,0,buff_size);
			strcpy(buff,"apple");
			i=5;
			*/
			printf("%d:%s\n",i,buff);
		} else if(c==0xd){
			//exec
			pHist->add(buff);
			pCmdH->exec(buff);
			memset(buff,0,buff_size);
			i=0;		
		} else if(c==0x1b){
			//escape			
			memset(buff,0,buff_size);
			i=0;
			printf("%d:%s\n",i,buff);
		} else if(c==0x1){
			//ctrl+a			
			int i;
			if(hi==0) i= pHist->getPrev();
			else i= pHist->getPrev(hi);
			hi = i;
			printf("prev index=%d\n",i);
			pHist->print_at(i);
		}
	}
	

};
