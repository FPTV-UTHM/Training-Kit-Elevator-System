extern const int led3D, pb3D, pb3, pbO, A4988EN, LEDO, CLK, BUZ;
extern const int led2U, pb2U, pb2, pbC, A4988ST, LEDC, RES;
extern const int led2D, pb2D, pb1, pbE, A4988DI;
extern const int led1U, pb1U, pbG;
extern const int led1D, pb1D;
extern const int ledGU, pbGU;

extern int initHome;
bool flagEmergency=0;

void oledDisplay(uint8_t mode, uint8_t floor);


int lantai=0,bil=0;
void beep(int bilangan, int tempoh)
{ for(int i=0;i<bilangan;i++)
  { tone(BUZ,5000); delay(tempoh);  
    noTone(BUZ);    delay(tempoh);
  }
}
void LEDs(int x)
{ digitalWrite(led3D,(x&(1<<0)));
  digitalWrite(led2U,(x&(1<<1)));
  digitalWrite(led2D,(x&(1<<2)));
  digitalWrite(led1U,(x&(1<<3)));
  digitalWrite(led1D,(x&(1<<4)));
  digitalWrite(ledGU,(x&(1<<5)));
}
int getPBLOP()
{ int x=0;
  if(digitalRead(pb3D)==0) x|=(1<<0);
  if(digitalRead(pb2U)==0) x|=(1<<1);
  if(digitalRead(pb2D)==0) x|=(1<<2);
  if(digitalRead(pb1U)==0) x|=(1<<3);
  if(digitalRead(pb1D)==0) x|=(1<<4);
  if(digitalRead(pbGU)==0) x|=(1<<5);
  return x;
}
int getPBCOP()
{ int x=0;
  if(digitalRead(pb3)==0) x|=(1<<0);
  if(digitalRead(pb2)==0) x|=(1<<1);
  if(digitalRead(pb1)==0) x|=(1<<2);
  if(digitalRead(pbG)==0) x|=(1<<3);
  return x;
}
void emergency()
{ while(1)
  { beep(1,100);
    if(lantai==0) oledDisplay(3,0);
    if(lantai==1) oledDisplay(3,0);
    if(lantai==2) oledDisplay(3,1);
    if(lantai==3) oledDisplay(3,2);
    LEDs(63); delay(100);
    LEDs(0);  delay(100);
    if(getPBLOP()==32) setup();
  }
}
void resetNumber()
{ digitalWrite(RES,HIGH); delay(10);
  digitalWrite(RES,LOW);
}
void setNumber(int nombor)
{ resetNumber();
  for(int i=0;i<nombor;i++)
  { digitalWrite(CLK,HIGH); delay(1);
    digitalWrite(CLK,LOW);
  }
}
void moveStepper(int arah, int bilangan)
{ digitalWrite(A4988EN,LOW);              //turn ON A4988
  digitalWrite(A4988DI,arah);             //direction
  
  for(int i=0;i<bilangan;i++)             //moving STEPS to desired FLOOR
  { if(digitalRead(pbE)==0) flagEmergency=1;
    digitalWrite(A4988ST,HIGH); delay(1);
    digitalWrite(A4988ST,LOW);  delay(1);
    if(arah==1) 
    { if(((i%600)==0) && initHome==1) 
      { oledDisplay(arah,lantai); setNumber(lantai); lantai++; 
        if(flagEmergency==1) emergency(); 
      }
    }
    if(arah==0) 
    { if(((i%600)==0) && initHome==1) 
      { oledDisplay(arah,lantai); setNumber(lantai); lantai--; 
        if(flagEmergency==1) emergency(); 
      }
    }
  }
  digitalWrite(A4988EN,HIGH);             //turn OFF A4988
}
void doorStatus (String status)
{ if(status=="open")  
  { delay(500);
    digitalWrite(LEDO,HIGH); 
    digitalWrite(LEDC,LOW);  
    beep(2,50); delay(500);
    for(int i=0;i<5;i++)
    { beep(1,200);
      delay(200); 
    }
    digitalWrite(LEDO,LOW);  
    digitalWrite(LEDC,HIGH); beep(2,50);   
  }
  if(status=="openD")  
  { digitalWrite(LEDO,HIGH); digitalWrite(LEDC,LOW);  }
  if(status=="closeD") 
  { digitalWrite(LEDO,LOW);  digitalWrite(LEDC,HIGH); }
}
