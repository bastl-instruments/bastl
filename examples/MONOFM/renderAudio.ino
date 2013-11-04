
long modulation[NUMBER_OF_MODULATORS];
boolean gate=true;

int updateAudio(){

  
  long finalModulation;
  int val;
  switch(var[ALGORITHM]) {

  case 0:
    modulation[MOD_1]=(((int)var[MOD_1_AMT]+lfoNow*isDestination(3))*aMod[MOD_1].next());
    modulation[MOD_2]=(((int)var[MOD_2_AMT]+lfoNow*isDestination(5))*aMod[MOD_2].next());
    modulation[MOD_3]=(((int)var[MOD_3_AMT]+lfoNow*isDestination(7))*aMod[MOD_3].next());

    // finalModulation=((modulation[MOD_1]+modulation[MOD_2]+modulation[MOD_3])*(var[MOD_AMT])+lfoNow*isDestination(1))>>8;
    val=((aCar.next()*(int)var[MOD_AMT])+modulation[MOD_1]+modulation[MOD_2]+modulation[MOD_3])>>3;
    break;
  case 1:
    modulation[MOD_1]=((int)var[MOD_1_AMT]+lfoNow*isDestination(3))*aMod[MOD_1].next();
    modulation[MOD_2]=((int)var[MOD_2_AMT]+lfoNow*isDestination(5))*aMod[MOD_2].next();
    modulation[MOD_3]=((int)var[MOD_3_AMT]+lfoNow*isDestination(7))*aMod[MOD_3].next();

    finalModulation=((modulation[MOD_1]+modulation[MOD_2]+modulation[MOD_3])*((int)var[MOD_AMT])+lfoNow*isDestination(1))>>8;
    val=aCar.phMod(finalModulation)<<5;


    break;
  case 2:
    modulation[MOD_1]=(var[MOD_1_AMT]+lfoNow*isDestination(3))*aMod[MOD_1].next();
    modulation[MOD_2]=(var[MOD_2_AMT]+lfoNow*isDestination(5))*aMod[MOD_2].next();
    modulation[MOD_3]=(var[MOD_3_AMT]+lfoNow*isDestination(7))*aMod[MOD_3].next();

    finalModulation=((modulation[MOD_1]+modulation[MOD_2]+modulation[MOD_3])*(var[MOD_AMT])+lfoNow*isDestination(1))>>8;
    val=aCar.phMod(finalModulation)<<5;

    break;
  case 3:
    modulation[MOD_1]=(var[MOD_1_AMT]+lfoNow*isDestination(3))*aMod[MOD_1].phMod(modulation[MOD_1]);
    modulation[MOD_2]=(var[MOD_2_AMT]+lfoNow*isDestination(5))*aMod[MOD_2].phMod(modulation[MOD_2]);
    modulation[MOD_3]=(var[MOD_3_AMT]+lfoNow*isDestination(7))*aMod[MOD_3].phMod(modulation[MOD_3 ]);

    finalModulation=((modulation[MOD_1]+modulation[MOD_2]+modulation[MOD_3])*(var[MOD_AMT])+lfoNow*isDestination(1))>>8;
    val=aCar.phMod(finalModulation)<<5;
    break;
  case 4:
    modulation[MOD_1]=(var[MOD_1_AMT]+lfoNow*isDestination(3))*aMod[MOD_1].next();// +2
    modulation[MOD_2]=(var[MOD_2_AMT]+lfoNow*isDestination(5))*aMod[MOD_2].next();
    modulation[MOD_3]=(var[MOD_3_AMT]+lfoNow*isDestination(7))*aMod[MOD_3].phMod(modulation[MOD_1]+modulation[MOD_2]);

    finalModulation=(modulation[MOD_3])*(var[MOD_AMT]+lfoNow*isDestination(1))>>8;
    val=aCar.phMod(finalModulation)<<5;
    break;
  case 5:

    modulation[MOD_1]=(var[MOD_1_AMT]+lfoNow*isDestination(3))*aMod[MOD_1].phMod(modulation[MOD_1]);// +2
    modulation[MOD_2]=(var[MOD_2_AMT]+lfoNow*isDestination(5))*aMod[MOD_2].phMod(modulation[MOD_2]);
    modulation[MOD_3]=(var[MOD_3_AMT]+lfoNow*isDestination(7))*aMod[MOD_3].phMod(modulation[MOD_1]+modulation[MOD_2]);

    finalModulation=(modulation[MOD_3])*(var[MOD_AMT]+lfoNow*isDestination(1))>>8;
    val=aCar.phMod(finalModulation)<<5;
    break;
  case 6:

    modulation[MOD_1]=(var[MOD_1_AMT]+lfoNow*isDestination(3))*aMod[MOD_1].next(); // + 5
    modulation[MOD_2]=(var[MOD_2_AMT]+lfoNow*isDestination(5))*aMod[MOD_2].phMod(modulation[MOD_1]);
    modulation[MOD_3]=(var[MOD_3_AMT]+lfoNow*isDestination(7))*aMod[MOD_3].next();

    finalModulation=(modulation[MOD_2]+modulation[MOD_3])*(var[MOD_AMT]+lfoNow*isDestination(1))>>8;
    val=aCar.phMod(finalModulation)<<5;
    break;
  case 7:

    modulation[MOD_1]=(var[MOD_1_AMT]+lfoNow*isDestination(3))*aMod[MOD_1].next(); //+5
    modulation[MOD_2]=(var[MOD_2_AMT]+lfoNow*isDestination(5))*aMod[MOD_2].phMod(modulation[MOD_1]);
    modulation[MOD_3]=(var[MOD_3_AMT]+lfoNow*isDestination(7))*aMod[MOD_3].phMod(modulation[MOD_2]);

    finalModulation=modulation[MOD_3]*(var[MOD_AMT]+lfoNow*isDestination(1))>>8;
    val=aCar.phMod(finalModulation)<<5;
    break;

  }

  // long finalModulation=modulation[MOD_1]+modulation[MOD_2]+modulation[MOD_3];
  return val*gate;

}








