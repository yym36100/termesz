#pragma once

#include "stdint.h"

#define log(...)	printf(__VA_ARGS__);

template<class T, size_t N> 
class cringbuff{
public:
	enum { size = N+1 };
	enum {stop=0, overwrite};

	uint16_t ri, wi;
	T data[size];

	cringbuff() :ri(0), wi(0){
		for (int i = 0; i < size; i++){
			data[i] = '_';
		}
	}

	bool isEmpty() { return ri == wi; }
	bool isFull() { return next(wi) == ri; }

	bool getAt(uint16_t offs, T*d){
		if(offs>getSize())return false;
		uint16_t tr = ri + offs;
		if (tr >= size)tr -= size;
		*d = data[tr];
	}


	bool Addn(T* d, uint16_t n){
		if (getFree() < n) return false;
		if ((wi + n) < size){
			// one block
			memcpy(&data[wi], d, n);
			wi += n;
		}
		else {
			// two blocks
			uint16_t size1 = size - wi;
			uint16_t size2 = n - size1;

			memcpy(&data[wi], d, size1);
			memcpy(&data[0], d+size1, size2);
			wi = size2;
		}
	}
	
	bool Add(T d){
		if (!isFull()){
			data[wi] = d;
			wi = next(wi);
			return true;
		}
		return false;
	}

	bool Remove(T *d){		
		if (!isEmpty()){
			T res = data[ri];
			data[ri] = '*';
			ri = next(ri);
			if(d)*d=res;			
			return true;
		}
		return false;
	}

	bool Removen(T*d,uint16_t n){
		if(getSize()<n) return false;

		if ((ri + n) < size){
			// one block
			memcpy(d,&data[ri], n);
			ri += n;
		}
		else {
			// two blocks
			uint16_t size1 = size - ri;
			uint16_t size2 = n - size1;

			memcpy(d,&data[ri], size1);
			memcpy(d+size1,&data[0],size2);
			ri = size2;
		}

	}

	uint16_t getSize(){
		int16_t delta = wi - ri;
		if (delta < 0) delta += size;
		return delta;
	}
	uint16_t getFree(){ return size - getSize()-1; }

	bool peek(T *d,uint16_t offset){
		if (offset < getSize()){
			offset += ri;
			if (offset >= size)offset -= size;
			if(d)*d=data[offset];
			return true;			
		}
		else{		
			return false;
		}
	}

	bool discard(uint16_t noelements){		
		if (noelements > getSize()){
			noelements = getSize();			
		}
		ri += noelements;
		if (ri >= size)ri -= size;		
		return true;
	}

	void list(){
		uint16_t tr = ri, tw = wi;
		if (tr == tw){
			log("RB list:empty\n");
			return;
		}
		log("RB list: {size=%d free=%d ri=%d wi=%d} ", getSize(),getFree(), ri, wi);
		while (1){
			printf("%c",data[tr]);
			if (next(tr) == tw)break;
			tr = next(tr);
		}
		log("\nraw storage:\n")
			for (int i = 0; i < size; i++){
				log("[%d]=%c ", i, data[i]);
			}
		log("\n");
	}

private:
	uint16_t next(uint16_t n){
		uint16_t t = n + 1;
		if (t == size) t = 0;
		return t;
	}
};
