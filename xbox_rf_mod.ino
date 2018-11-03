// uncomment if you want an additional button to turn off connected controllers
 #define __OFFBTN__

//pin definitions

//clock and data
uint16_t XSCL=4;
uint16_t XSDA=5;
//xbox module button
uint16_t XBTN=6;
//button to turn off all controllers
#ifdef __OFFBTN__
uint16_t OFFBTN=7;
#endif


//messages
uint16_t XSYNC=0x004;
uint16_t XBOOTANIM=0x085;
uint16_t XGREEN=0x0A1;
uint16_t XGREENOFF=0x0A0;
uint16_t XTURNOFFCONTROLLER=0x009;

void sendcmd(uint16_t cmd){
  //sda needs to be brought low before command is sent
  digitalWrite(XSDA,LOW);
  uint8_t index=0;

  while (index<10){
    while(digitalRead(XSCL)==HIGH) {;}

    if ( (cmd & (0x200 >> index)) == 0 ){
      digitalWrite(XSDA,LOW);
    }
    else{
      digitalWrite(XSDA,HIGH);
    }

    while(digitalRead(XSCL)==LOW) {;}

    index++;
  }
  digitalWrite(XSDA,HIGH);
}

void setup() {
  //pin setup
  pinMode(XSCL,INPUT);
  pinMode(XSDA,OUTPUT);
  pinMode(XBTN,INPUT_PULLUP);
  #ifdef __OFFBTN__
  pinMode(OFFBTN,INPUT_PULLUP);
  #endif
  //gives board some time to boot up
  delay(3000);
  digitalWrite(XSDA,HIGH);
  digitalWrite(XSCL,HIGH); //apparently, you need to get the clock high first.
  //turn leds off... in case they were on upon Arduino reboot
  sendcmd(XGREENOFF);
  delay(50);
  //send boot anim, give it some time to play, then turn a green LED on (or more of them, refer to XGREEN definition)
  sendcmd(XBOOTANIM);
  delay(5000);
  sendcmd(XGREEN);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  if(digitalRead(XBTN)==LOW){
    sendcmd(XSYNC);
    delay(15000);
    sendcmd(XGREEN);
  }
  #ifdef __OFFBTN__
  if(digitalRead(OFFBTN)==LOW){
    sendcmd(XTURNOFFCONTROLLER);
    delay(2000);
  }
  #endif
}
