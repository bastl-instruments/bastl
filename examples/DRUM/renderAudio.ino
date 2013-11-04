#define DOWN_SHIFT 3 // zkusit min


int updateAudio(){

  long output=0;

  for(int voice=0;voice<NUMBER_OF_VOICES;voice++){
   if(aSample[voice].isPlaying()) output+=(volume[voice]*(aSample[voice].next()^crush[voice]))>>7;
  }


  return output<<5;

}

