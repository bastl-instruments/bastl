#define DOWN_SHIFT 3 // zkusit min
unsigned char shiftRight=4;
int p1,p2,p3;
unsigned char varNow[6];

void renderSound(){

  unsigned char _SOUND=sound;
  if(_SOUND!=0){
    _SOUND--;
    getTypes(_SOUND);
    oscType=getBits(OSC_TYPE);
    for(int i=0;i<6;i++){
      if(globalize[i]) varNow[i]=globalVar[i];
      else varNow[i]=variable[_SOUND][i];
    }
    // aOsc[OSC_A].setTable(WAVE_TABLES[getBits(OSC_PARAM_1)]),
    setLFO();
    setADSR();
    setTables();
    switch(oscType){
    default:
      //case OSC3:
      // setTables();
      for(int i=0;i<3;i++) aOsc[i].setFreq(varNow[i]<<2+getLfoNow(i));
      break;
    case FM:
      {
        //setTables();
        int carrierFreq=mtof(varNow[OSC_A]>>1)+getLfoNow(OSC_A);
        aOsc[OSC_A].setFreq(carrierFreq);
        float ratio= (float) (varNow[OSC_B]<<2+getLfoNow(OSC_B))/64;
        float modFreq=(float)carrierFreq*ratio;
        aOsc[OSC_B].setFreq(modFreq);
        modAmt=varNow[OSC_C]+getLfoNow(OSC_C)>>2;
      }
      break;
    case LPF:
      {
        aOsc[OSC_A].setTable(WAVE_TABLES[(getBits(OSC_PARAM_1)>>2)+1]);

        aOsc[OSC_A].setFreq(mtof(varNow[OSC_A]>>1)+getLfoNow(OSC_A));
        int lpfVal=int(varNow[OSC_B]<<2+getLfoNow(OSC_B))>>3;
        if(lpfVal>200) lpfVal=200;
        if(lpfVal<0) lpfVal=0;
        lpf.setCutoffFreq(lpfVal);
        lpfVal=int((varNow[OSC_C]<<2+getLfoNow(OSC_C))>>2)+1;
        if(lpfVal>255) lpfVal=255;
        if(lpfVal<0) lpfVal=0;
        lpf.setResonance(lpfVal);
      }
      break;
    case AUDU:

      syncPhaseInc = mapMidi(1023-varNow[OSC_A]<<2+getLfoNow(OSC_A));
      grainPhaseInc  = mapPhaseInc(1023-varNow[OSC_B]<<2+getLfoNow(OSC_B)) / 2;
      grainDecay     =(1023-getBits(OSC_PARAM_1)<<8) / 8;
      grain2PhaseInc = mapPhaseInc(1023-varNow[OSC_C]<<2+getLfoNow(OSC_C)) / 2;
      grain2Decay    = (1023-getBits(OSC_PARAM_2)<<9) / 8;
      break;
      /*
    case AM:
       //   setTables();
       for(int i=0;i<3;i++) aOsc[i].setFreq(getVar(_SOUND,i)+getLfoNow(i));
       
       break;
       */
    case XOR:
      //   setTables();
      for(int i=0;i<2;i++) aOsc[i].setFreq(varNow[i]<<2+getLfoNow(i));
      modAmt=varNow[OSC_C]+getLfoNow(OSC_C)>>2;
      break;
    case ONE_LINER:
      p1=255-varNow[OSC_A];
      p2=varNow[OSC_B];
      p3=varNow[OSC_C];
      break;
    case FM2:
      aOsc[OSC_A].setTable(WAVE_TABLES[getBits(OSC_PARAM_1)>>1]);
      aOsc[OSC_B].setTable(WAVE_TABLES[getBits(OSC_PARAM_1)>>1]);
      aOsc[OSC_C].setTable(WAVE_TABLES[getBits(OSC_PARAM_1)>>1]);
      int oscF=mtof(varNow[OSC_A]>>1)+getLfoNow(OSC_A);
      int detune=(varNow[OSC_B]<<2+getLfoNow(OSC_B));
      aOsc[OSC_A].setFreq(oscF);
      aOsc[OSC_B].setFreq(oscF+detune);
      aOsc[OSC_C].setFreq(oscF-detune);



      break;
    }
  }
}

void setLFO(){
  LFO.setAll((getBits(LFO_RATE)+1)<<5,getBits(LFO_SHAPE)*2,32);
  lfoDest=getBits(LFO_DEST);
  if(var[1]==0) lfoAmt=0;//, lfoNow=0;
  else lfoAmt=(getBits(LFO_AMT)+1)<<5; 
}
void setADSR(){
  ADSR.setAttack(getBits(ADSR_ATTACK)<<5) ;
  adsrType=getBits(ADSR_TYPE);
  switch(adsrType)
  {
  case 0:
    ADSR.setAttack(0);
    ADSR.setDecay(0);
    ADSR.setSustain(255);
    ADSR.setRelease(getBits(ADSR_RELEASE)<<5) ;
    break;
  case 1:
    ADSR.setDecay(127);
    ADSR.setSustain(127);
    ADSR.setRelease(getBits(ADSR_RELEASE)<<5) ;
    break;
  case 2:
    ADSR.setDecay(20);
    ADSR.setSustain(127);
    ADSR.setRelease(getBits(ADSR_RELEASE)<<5) ;
    break;
  case 3:
    ADSR.setDecay(getBits(ADSR_RELEASE)<<5) ;
    ADSR.setRelease(0);
    ADSR.setSustain(0);
    break;

  } 
}
void setTables(){
  aOsc[OSC_A].setTable(WAVE_TABLES[getBits(OSC_PARAM_1)>>1]);
  aOsc[OSC_B].setTable(WAVE_TABLES[getBits(OSC_PARAM_2)]); 
  aOsc[OSC_C].setTable(WAVE_TABLES[getBits(OSC_PARAM_2)]); 
}

int count,t;
int updateAudio(){
  seq.update(audioTicks());
  long output=0;
  switch(oscType){
  default: //OSC3 FM2
    //case OSC3:
    for(int i=0;i<3;i++) output+=aOsc[i].next();
    shiftRight=5;
    break;
  case FM:
    output=aOsc[0].phMod(modAmt*aOsc[1].next());
    shiftRight=4;
    break;
  case LPF:
    output=lpf.next(aOsc[OSC_A].next());
    shiftRight=4;
    break;
  case AUDU:
    output=auduino();
    shiftRight=5;
    break;

  case AM:
    output=((aOsc[2].next()*aOsc[1].next()>>8)*aOsc[0].next())>>4;
    shiftRight=5;
    break;
  case XOR:
    output=((aOsc[0].next()|aOsc[1].next()>>8)&modAmt);
    shiftRight=4;

    break;
  case ONE_LINER:
    count++;
    if(count>p1) t++, count=0;
    output=(t|(t>>p3|t>>7)*p2*t&(t>>11|t>>9));
    //output=oneLiner();
    shiftRight=5;
    break;
    /*
  case FM2:
     for(int i=0;i<3;i++) output+=aOsc[i].next();
     shiftRight=5;
     break;
     */

  }
  output*=ADSR.next();
  if(renderM) output+= aOsc[METRONOME_OSC].next()<<7;
  output = output >> (shiftRight-1);
  return output;



}



PROGMEM prog_uint16_t antilogTable[] = {
  64830,64132,63441,62757,62081,61413,60751,60097,59449,58809,58176,57549,56929,56316,55709,55109,
  54515,53928,53347,52773,52204,51642,51085,50535,49991,49452,48920,48393,47871,47356,46846,46341,
  45842,45348,44859,44376,43898,43425,42958,42495,42037,41584,41136,40693,40255,39821,39392,38968,
  38548,38133,37722,37316,36914,36516,36123,35734,35349,34968,34591,34219,33850,33486,33125,32768
};
uint16_t mapPhaseInc(uint16_t input) {
  return (pgm_read_word_near(antilogTable + (input & 0x3f)) >> (input >> 6));
}

// Stepped chromatic mapping
//

PROGMEM prog_uint16_t midiTable[] = {
  17,18,19,20,22,23,24,26,27,29,31,32,34,36,38,41,43,46,48,51,54,58,61,65,69,73,
  77,82,86,92,97,103,109,115,122,129,137,145,154,163,173,183,194,206,218,231,
  244,259,274,291,308,326,346,366,388,411,435,461,489,518,549,581,616,652,691,
  732,776,822,871,923,978,1036,1097,1163,1232,1305,1383,1465,1552,1644,1742,
  1845,1955,2071,2195,2325,2463,2610,2765,2930,3104,3288,3484,3691,3910,4143,
  4389,4650,4927,5220,5530,5859,6207,6577,6968,7382,7821,8286,8779,9301,9854,
  10440,11060,11718,12415,13153,13935,14764,15642,16572,17557,18601,19708,20879,
  22121,23436,24830,26306
};
uint16_t mapMidi(uint16_t input) {
  return pgm_read_word_near(midiTable + ((1023-input) >> 3));
}

uint8_t amplitude = 255;

int auduino(){
  uint8_t value;
  uint16_t output;

  syncPhaseAcc += syncPhaseInc;

  if (syncPhaseAcc < syncPhaseInc) {
    // Time to start the next grain
    grainPhaseAcc = 0;
    grainAmp = 0x7fff;
    grain2PhaseAcc = 0;
    grain2Amp = 0x7fff;
    // if (isSound && amplitude > 0) {
    //     LED_PORT ^= 1 << LED_BIT; // Faster than using digitalWrite
    //}
  }

  // Increment the phase of the grain oscillators
  grainPhaseAcc += grainPhaseInc;
  grain2PhaseAcc += grain2PhaseInc;

  // Convert phase into a triangle wave
  value = (grainPhaseAcc >> 7) & 0xff;
  if (grainPhaseAcc & 0x8000) value = ~value;
  // Multiply by current grain amplitude to get sample
  output = value * (grainAmp >> 8);

  // Repeat for second grain

  value = (grain2PhaseAcc >> 7) & 0xff;
  if (grain2PhaseAcc & 0x8000) value = ~value;
  output += value * (grain2Amp >> 8);

  // Make the grain amplitudes decay by a factor every sample (exponential decay)
  grainAmp -= (grainAmp >> 8) * grainDecay;
  grain2Amp -= (grain2Amp >> 8) * grain2Decay;
  // Scale output to the available range, clipping if necessary
  output >>= 1;
  // if (amplitude < 1 || !isSound) {
  //  LED_PORT &= ~(1 << LED_BIT);
  // }
  //  output *= amplitude;
  //  output >>= 9;

  // if (output > 255) output = 255;
  return output; 
}
















