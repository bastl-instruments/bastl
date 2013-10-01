int updateAudio(){
  int out=0;
  if (aSample.isPlaying()) out =aSample.next();
  return out<<6; // return an int signal centred around 0
}
