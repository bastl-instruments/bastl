
#ifndef QUATIZER_8_BIT
#define QUATIZER_8_BIT
#include <Arduino.h>
#include "basic.h"
class quantizer8bit {

public:
//	void init();
	uint8_t quantizeNote(uint8_t note, uint8_t _scale);
	uint8_t quantize(uint8_t input, uint8_t _scale);



private:

};


const bool scale[8][12]{
		{1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,1,0,1,1,0,1,0,1,0,1},
		{1,0,1,1,0,1,0,1,0,1,1,0},
		{1,0,1,0,1,1,0,1,0,1,0,1},
		{1,0,0,0,0,1,0,0,0,0,0,0},
		{1,0,0,0,0,1,0,0,0,0,0,0},
		{1,0,0,0,0,1,0,0,0,0,0,0},
		{1,0,0,0,0,1,0,0,0,0,0,0},
};



uint8_t noteTo8bit[61]={
		0,4,9,13,17,21,26,30,34,38,43,47,51,55,60,64,68,72,77,81,85,89,94,98,
		102,106,111,115,119,123,128,132,136,140,145,149,153,157,162,166,170,
		174,179,183,187,191,196,200,204,208,213,217,221,225,230,234,238,242,247,251,255};

inline uint8_t quantizeNote(uint8_t note, uint8_t _scale){

	if(scale[_scale][note%12]){
		return note;
	}
	else {
		uint8_t higher=0;
		while(!scale[_scale][(note+higher)%12]){
			higher++;
		}
		uint8_t lower=0;
		while(!scale[_scale][(note-lower)%12]){
			lower++;
		}
		if(higher<lower) return note+higher;
		else return note-lower;
	}

	return note;
}

inline uint8_t quantize(uint8_t input,uint8_t _scale){
	if(_scale==0){
		return input;
	}
	else{
		_scale--;
		uint8_t note=1;//map(input,0,255,0,60);
		//note=quantizeNote(1,2);//note,_scale);
		return noteTo8bit[note];
	}
}

#endif
