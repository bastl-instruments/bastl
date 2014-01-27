#define DOWN_SHIFT 3 // zkusit min


int updateAudio(){
seq.update(audioTicks());
  long output=0;
volume[5]=255;
  for(int voice=0;voice<NUMBER_OF_VOICES;voice++){
    if(aSample[voice].isPlaying()) output+=(volume[voice]*(aSample[voice].next()^crush[voice]))>>8;
  }
 //if(aSample[5].isPlaying()) output+=aSample[5].next();
  //if(metronome){
   // if(aSample[5].isPlaying()) output+=aSample[5].next();
 // }
  return output<<4;

}

