#define DOWN_SHIFT 8 // zkusit min

#define XOR 0
#define OR 1
#define AND 2
#define HYPER_XOR 3
#define HYPER_OR 4
//#define HYPER_AND 3

int updateAudio(){


  long output=0;




  // output+=aOsc[voice].next();  
  //   modulation[voice]=modDepth[voice]*aMod[voice].next();
  //  output+=((aOsc[voice].next()^xorAmt)*ADSR[voice].next())>>DOWN_SHIFT;

  switch(xorType){

  case XOR:
    for(int voice=0;voice<NUMBER_OF_VOICES;voice++){
      if(ADSR[voice].active()){  
        output+=((aOsc[voice].next()^xorAmt)*ADSR[voice].next())>>DOWN_SHIFT;
      }
    }
    break;
  case OR:
    for(int voice=0;voice<NUMBER_OF_VOICES;voice++){
      if(ADSR[voice].active()){  
        output+=((aOsc[voice].next()|xorAmt)*ADSR[voice].next())>>DOWN_SHIFT;
      }
    }
    break;
  case AND:
    for(int voice=0;voice<NUMBER_OF_VOICES;voice++){
      if(ADSR[voice].active()){  
        output+=((aOsc[voice].next()&xorAmt)*ADSR[voice].next())>>DOWN_SHIFT;
      }
    }
    break;
  case HYPER_XOR:
    for(int voice=0;voice<NUMBER_OF_VOICES;voice++){
      if(ADSR[voice].active()){  
        output^=((aOsc[voice].next()^xorAmt)*ADSR[voice].next())>>DOWN_SHIFT;
      }
    }
    case HYPER_OR:
    for(int voice=0;voice<NUMBER_OF_VOICES;voice++){
      if(ADSR[voice].active()){  
        output|=((aOsc[voice].next()|xorAmt)*ADSR[voice].next())>>DOWN_SHIFT;
      }
    }
   
    break;



  }

  // 
  //  amplitude = (255+lfoNow);//*gate;
//  amplitude=255;
  output=(output*amplitude)>>3;//<<5;//=(output*lfoNow)>>3;

  // output*=gate;

  return output;

}







