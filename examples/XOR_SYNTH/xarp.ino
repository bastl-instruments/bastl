
#include <midi_Defs.h>


#define GATE_ARP_0 0
#define GATE_ARP_1 1
#define GATE_ARP_2 2
#define NOTE_ARP_UP 3
#define NOTE_ARP_DOWN 4
#define NOTE_ARP_UP_DOWN 5
#define NOTE_ARP_UP_DOWN_EXCLUSIVE 6
#define NOTE_ARP_CAUSAL 7
#define NOTE_ARP_RANDOM 8



unsigned char orderedBuffer[BUFFER_SIZE];

unsigned char lastNotes;
unsigned char notesInOrderedBuffer;
unsigned char lastVoice;
void renderArp(){

  /*
  seq.update(audioTicks());
   seq.setClockDivider(1);
   if(!slave) {
   while(seq.clockReady()) seq.clockIn(), sendClock();
   } 
   */


  if(lastNotes==0 && notesInOrderedBuffer!=0 && !slave) seq.play();  //,seq.jumpToStep(0);

  if(lastNotes!=0 && notesInOrderedBuffer==0 && voiceUse[lastVoice]!=255){
    voiceUse[lastVoice]=255;
    ADSR[lastVoice].noteOff();
  }

  lastNotes=notesInOrderedBuffer;
  if(!slave){

    seq.clockIn();
    sendClock();
  }
  while(seq.stepReady()){
    if(notesInOrderedBuffer!=0) hw.setColor(WHITE);
    seq.stepIn();
    proceedStep(seq.getCurrentStep()); 
  }

  // sendGBclock();

}
//#define CLOCK 8
void sendClock(){
  MIDI.sendRealTime(Clock);
}

//for(int i=0;i<NUMBER_OF_VOICES;i++) if(!ADSR[i].active()) voiceUse[i]=255;


void proceedStep(unsigned char _step){


  // showTempo(_step);
  unsigned char voice,_note;



  switch(arpType){

  default:
    gateArp=true;
    seq.setNumberOfSteps(8);
    if(_step%2==1) gate=false;
    else gate=true;
    break;

  case GATE_ARP_1:
    gateArp=true;
    seq.setNumberOfSteps(8);
    if(_step%3==1) gate=false;
    else gate=true;
    break;

  case GATE_ARP_2:
    gateArp=true;
    seq.setNumberOfSteps(8);
    if(_step<4){
      if(_step%2==0) gate=true;
      else gate=false;
    }
    else if(_step==7) gate=false;
    else  gate=true;
    break;

  case NOTE_ARP_UP:
    gateArp=false;
    gate=true;
    if(notesInOrderedBuffer>0){

      seq.setNumberOfSteps(notesInOrderedBuffer);
      _note=orderedBuffer[_step];
      // lastVoice=voice;
      voiceUse[lastVoice]=255;
      ADSR[lastVoice].noteOff();

      lastVoice=proceedNoteOn(_note,ARP_VELOCITY);

    }

    break;

  case NOTE_ARP_DOWN:
    gateArp=false;
    gate=true;
    if(notesInOrderedBuffer>0){
      seq.setNumberOfSteps(notesInOrderedBuffer);
      _note=orderedBuffer[notesInOrderedBuffer-_step-1];

      // lastVoice=voice;
      voiceUse[lastVoice]=255;
      ADSR[lastVoice].noteOff();

      lastVoice= proceedNoteOn(_note,ARP_VELOCITY);
    }

    break;

  case NOTE_ARP_UP_DOWN:
    gateArp=false;
    gate=true;

    if(notesInOrderedBuffer>0){
      seq.setNumberOfSteps(notesInOrderedBuffer*2);
      if(_step<notesInOrderedBuffer) _note=orderedBuffer[_step];
      else _note=orderedBuffer[(2*notesInOrderedBuffer)-_step-1];

      // lastVoice=voice;
      voiceUse[lastVoice]=255;
      ADSR[lastVoice].noteOff();

      lastVoice=proceedNoteOn(_note,ARP_VELOCITY);
    }

    break;

  case NOTE_ARP_UP_DOWN_EXCLUSIVE:
    gateArp=false;
    gate=true;

    if(notesInOrderedBuffer>0){
      seq.setNumberOfSteps((notesInOrderedBuffer*2) - 2);
      if(_step<(notesInOrderedBuffer-1)) _note=orderedBuffer[_step];
      else _note=orderedBuffer[(2*notesInOrderedBuffer)-_step-2];

      // lastVoice=voice;
      voiceUse[lastVoice]=255;
      ADSR[lastVoice].noteOff();

      lastVoice=proceedNoteOn(_note,ARP_VELOCITY);
    }

    break;

  case NOTE_ARP_CAUSAL:
    gateArp=false;
    gate=true;
    if(!sustainPedal) causaliseBuffer;

    if(notesInOrderedBuffer>0){
      seq.setNumberOfSteps(notesInOrderedBuffer);
      _note=orderedBuffer[_step];

      // lastVoice=voice;
      voiceUse[lastVoice]=255;
      ADSR[lastVoice].noteOff();

      lastVoice=proceedNoteOn(_note,ARP_VELOCITY);
      ;

    }

    break;

  case NOTE_ARP_RANDOM:
    gateArp=false;
    gate=true;

    if(notesInOrderedBuffer>0){
      seq.setNumberOfSteps(notesInOrderedBuffer);
      _note=orderedBuffer[rand(notesInOrderedBuffer)];
      //   lastVoice=voice;
      voiceUse[lastVoice]=255;
      ADSR[lastVoice].noteOff();

      lastVoice=proceedNoteOn(_note,ARP_VELOCITY);
    }

    break;

  }

}

void causaliseBuffer(){
  for(int j=0;j<BUFFER_SIZE;j++)  orderedBuffer[j]=midiBuffer[j];

  notesInOrderedBuffer=notesInBuffer;
}


void orderBuffer(){

  for(int j=0;j<BUFFER_SIZE;j++)  orderedBuffer[j]=0;

  unsigned char nextOne=255;
  for(int i=0;i<notesInBuffer;i++){
    if(midiBuffer[i] < nextOne) nextOne=midiBuffer[i];
  } 
  orderedBuffer[0]=nextOne;

  for(int j=1;j<notesInBuffer;j++){
    nextOne=255;
    for(int i=0;i<notesInBuffer;i++){
      if(midiBuffer[i]<nextOne && midiBuffer[i] > orderedBuffer[j-1] ) nextOne=midiBuffer[i];
    } 
    orderedBuffer[j]=nextOne;
  }

  notesInOrderedBuffer=notesInBuffer;

}






























