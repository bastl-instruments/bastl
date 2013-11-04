
void initMidi(unsigned char _channel){

  MIDI.begin(0);    


  MIDI.setHandleNoteOn(HandleNoteOn);
  MIDI.setHandleNoteOff(HandleNoteOff);


  MIDI.setHandleControlChange(HandleControlChange);
  MIDI.setHandleSystemExclusive(HandleSystemExclusive);

  //MIDI.turnThruOn(Full);
  MIDI.turnThruOff();  
}


void HandleControlChange(byte channel, byte number, byte value){
  if(number<13){
    unsigned char osc=number-10;
    aOsc[osc].setFreq(value<<3);
    hw.setColor(WHITE);
  }
  else if(number==14){
   for(int i=0;i<3;i++) hw.setSwitch(i,bitRead(value,i));
  }
  else if(number==13){
    page=value;
  }

}

void HandleSystemExclusive(byte *array, byte size){
  testing=true;
}


void HandleNoteOn(byte channel, byte note, byte velocity) { 

}

void HandleNoteOff(byte channel, byte note, byte velocity){
}

