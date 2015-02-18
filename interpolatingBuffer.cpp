/*
 * interpolatingBuffer.cpp

 *
 *  Created on: Jan 28, 2015
 *      Author: dasvaclav
 */
#include "interpolatingBuffer.h"
#include "basic.h"
#include <Line.h>
//#include <quantizer8bit.h>
//quantizer8bit qnt;
Line <float> linear[6];
void interpolatingBuffer::init(uint8_t channel){
	intCh=channel;
}
void interpolatingBuffer::sync(){

}

uint8_t interpolatingBuffer::getCurrentValue(){
	if(interpolate){
		if(interpolatedSteps<interpolateInSteps){
			interpolatedSteps++;
			return linear[intCh].next();
		}
		else{
			if(interpolateStep==31) return buffer[0];
			else return buffer[interpolateStep+1];
		}
	}
	else if(smoothingAmount==0) return buffer[0];
	else return calculateSmoothAverage();
}

void interpolatingBuffer::update(){
	//linear.update();
	smallSteps++;
	if(smallSteps>=periodInSteps){
		smallSteps=0;
		if(step<31){
			step++;
		}
		else{
			step=0;
		}
		if(!interpolate){
			if(!loopState) addRandomStep(); //?
			else rotateBuffer();
		}
	}

	smoothSteps++;
	//if(smoothingAmount>127){
	uint16_t smoothingSkip=map(smoothingAmount,0,2047,0,periodInSteps);
	if(smoothSteps>=smoothingSkip){
		smoothSteps=0;
		addSmoothStep(buffer[0]);
	}

}

void interpolatingBuffer::setStep(uint8_t _step){
	interpolateStep=_step;
	interpolateInSteps=map(interpolateAmount,0,255,1,periodInSteps);
	if(interpolate){
		if(_step<31){
		//	linear[intCh].set(qnt.quantize(buffer[_step],_scale),qnt.quantize(buffer[_step+1],_scale),interpolateInSteps);
		}
		else{
			//linear[intCh].set(qnt.quantize(buffer[_step],_scale),qnt.quantize(buffer[0],_scale),interpolateInSteps);
		}
	}
	interpolatedSteps=0;
}
void interpolatingBuffer::addRandomStep(){
	rotateBuffer();
	int16_t randomNumber=random(0,64)-31+buffer[1];
	if(randomNumber>255) randomNumber=255;
	if(randomNumber<0) randomNumber=0;
	buffer[0]=randomNumber;
}

void interpolatingBuffer::addSmoothStep(uint8_t _value){
	rotateSmoothBuffer();
	smoothBuffer[0]=_value;
}

void interpolatingBuffer::rotateBuffer(){
	uint8_t temp=buffer[31];
		for(uint8_t i=31;i>0;i--){
			buffer[i]=buffer[i-1];
		}
	buffer[0]=temp;
}

void interpolatingBuffer::rotateSmoothBuffer(){
	uint8_t temp=smoothBuffer[31];
		for(uint8_t i=31;i>0;i--){
			smoothBuffer[i]=smoothBuffer[i-1];
		}
	smoothBuffer[0]=temp;
}
uint8_t interpolatingBuffer::calculateSmoothAverage(){
	uint16_t sum=0;
	for(int i=0;i<32;i++){
		sum+=smoothBuffer[i];
	}
	return sum/32;
}
