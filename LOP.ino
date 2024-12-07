#include "Lift.h"
#include "OLEDLift.h"

const int led3D=A0, pb3D=A6,  pb3=2,  pbO=17, A4988EN=12, LEDO=7, CLK=13, BUZ=9;
const int led2U=A1, pb2U=A7,  pb2=14, pbC=18, A4988ST=11, LEDC=8, RES=6;
const int led2D=A2, pb2D=A8,  pb1=15, pbE=19, A4988DI=10;
const int led1U=A3, pb1U=A9,  pbG=16;
const int led1D=A4, pb1D=A10;
const int ledGU=A5, pbGU=A11;

extern int lantai,bil;
int carFloor=0,callingFloor=0,carDirection=0,LOPPBstatus=0;;
int initHome=0, flow=0;

void setup() 
{ pinMode(pb3D,INPUT_PULLUP); pinMode(led3D,OUTPUT); pinMode(LEDO,OUTPUT); pinMode(CLK,OUTPUT);
  pinMode(pb2U,INPUT_PULLUP); pinMode(led2U,OUTPUT); pinMode(LEDC,OUTPUT); pinMode(RES,OUTPUT);
  pinMode(pb2D,INPUT_PULLUP); pinMode(led2D,OUTPUT); pinMode(BUZ,OUTPUT);
  pinMode(pb1U,INPUT_PULLUP); pinMode(led1U,OUTPUT);
  pinMode(pb1D,INPUT_PULLUP); pinMode(led1D,OUTPUT);
  pinMode(pbGU,INPUT_PULLUP); pinMode(ledGU,OUTPUT);

  pinMode(pb3,INPUT_PULLUP);  pinMode(pbO,INPUT_PULLUP);
  pinMode(pb2,INPUT_PULLUP);  pinMode(pbC,INPUT_PULLUP);
  pinMode(pb1,INPUT_PULLUP);  pinMode(pbE,INPUT_PULLUP);
  pinMode(pbG,INPUT_PULLUP);

  for(int i=10;i<=12;i++) pinMode(i,OUTPUT);
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  home();
}

void home()
{ LEDs(initHome);
  beep(3,50);
  digitalWrite(A4988EN,HIGH);           //turn OFF A4988
  moveStepper(1,200);                   //stepper ke atas
  while(analogRead(A12)>700)            //ke bawah selagi nilai>700
    moveStepper(0,1);
  
  oledDisplay(2,initHome);
  setNumber(initHome);
  
  digitalWrite(LEDC,HIGH);
  beep(2,50);
  initHome=1;
}
void LOP()
{ if(getPBLOP()!=0)
  { if(digitalRead(pb3D)==0) { callingFloor=3; LOPPBstatus=1; LEDs(1);  }  
    if(digitalRead(pb2U)==0) { callingFloor=2; LOPPBstatus=2; LEDs(2);  }
    if(digitalRead(pb2D)==0) { callingFloor=2; LOPPBstatus=3; LEDs(4);  }
    if(digitalRead(pb1U)==0) { callingFloor=1; LOPPBstatus=4; LEDs(8);  }
    if(digitalRead(pb1D)==0) { callingFloor=1; LOPPBstatus=5; LEDs(16); }
    if(digitalRead(pbGU)==0) { callingFloor=0; LOPPBstatus=6; LEDs(32); }    

    beep(1,50);
    //Car going up
    if((callingFloor-carFloor)==3)  moveStepper(1,1800); 
    if((callingFloor-carFloor)==2)  moveStepper(1,1200);    
    if((callingFloor-carFloor)==1)  moveStepper(1,600);

    //Car going down
    if((callingFloor-carFloor)==-3) moveStepper(0,1800); 
    if((callingFloor-carFloor)==-2) moveStepper(0,1200);    
    if((callingFloor-carFloor)==-1) moveStepper(0,600);
  
    setNumber(callingFloor); 
    oledDisplay(2,callingFloor);
    carFloor=callingFloor;
    doorStatus("open");
    flow=1;
  }
}

void loop() 
{ if(flow==0) LOP();
}


