#define DOWN_SHIFT 3 // zkusit min

int updateAudio(){

  unsigned int modulation[NUMBER_OF_VOICES]; //int?
  long output=0;

  for(int voice=0;voice<NUMBER_OF_VOICES;voice++){
    if(ADSR[voice].active()){    
      //   modulation[voice]=modDepth[voice]*aMod[voice].next();
      //output+=(amplitude[voice]*aOsc[voice].phMod(modulation[voice]))>>8;

      if(fm[voice]){
        modulation[voice]=modDepth[voice]*aMod[voice].next();
        output+=(ADSR[voice].next()*aOsc[voice].phMod(modulation[voice]))>>DOWN_SHIFT;
      }
      else  output+=(lpf[voice].next(aOsc[voice].next()>>1)*ADSR[voice].next())>>DOWN_SHIFT;//envelope v filtru
    }
  }


  return output;

}

