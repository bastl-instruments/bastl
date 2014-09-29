


//red

/*
page
 
 
 finetune osc
 finetune lfo
 volume
 
 se shiftem
 
 tune osc
 tune lfo
 char
 
 */

// ++ rungler?
boolean sig[3];
int osc[3];



int updateAudio(){
  unsigned char signal=0;
  for(int i=0;i<3;i++){
    osc[i]=aOsc[i].next();

    //COMPARATOR
    if(osc[i]>aOsc[3+i].next()) sig[i]=true;
    else sig[i]=false;




    //LED
    //setLed(sig[i]); PORT BLABLA

    //dodělat nějakej fígl na volume !!!
    if(ledShow){
      if(shift) {
        if(page==i && sig[i] && abs(osc[i])<volume[i]) PORTD &= ~(1<<rgbPins[i]);
        else PORTD |= 1<<rgbPins[i]; 
      }
      else{
        if(sig[i] && abs(osc[i])<volume[i]) PORTD &= ~(1<<rgbPins[i]);
        else PORTD |= 1<<rgbPins[i];
      }
    }



    //volume
    // signal+=osc[i];
    signal+=sig[i]*(volume[i]>>1);
  }

  //SYNC 
  for(int i=0;i<3;i++){
    for(int j=0;j<3;j++){
      if( sig[i] && syncSig[j]==i+1 ){
        aOsc[j].setPhase(0);


      } 
      if(osc[i]>>7 && syncOsc[j]==i+1 ){ //
        aOsc[j].setPhase(osc[i]);
      }
    }
  }

  return int(signal-128) << 5;

}
/*
#define LED_R_PIN 5 //D5
 #define LED_G_PIN 3 //D3
 #define LED_B_PIN 4 //D4
 
 */

/*
un
 high
 PORTD |= 1<<rgbPins[i]; // Set bit 2 high
 
 low
 
 */






