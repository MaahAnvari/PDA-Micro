
//timer1 will interrupt at 1Hz  //ghavi

#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


//#define k 11  //buffer size
#define cons 0.001
#define sizeR 60
#define thr 10000
#define segA 2//connecting segment A to PIN2
#define segB 3// connecting segment B to PIN3
#define segC 4// connecting segment C to PIN4
#define segD 5// connecting segment D to PIN5
#define segE 6// connecting segment E to PIN6
#define segF 7// connecting segment F to PIN7
#define segG 8// connecting segment G to PIN8

//storage variables
boolean toggle0 = 0;
boolean toggle1 = 0;
boolean toggle2 = 0;
int g=0;
int x;    //input signal
int k =0 ; //buffer size == 50
int c=0;  //counter
int data[sizeR];    //first signal input
int R[sizeR];       //conclusion
int iteration=0;
int notcount=0;
//int cons;   // each time part
int thFlag;
int RFlag;
float frequence; 
int i,j; 
int lcdFlag =0;
char notchar,notnum;
const int k2 = 4;
String notbuffer[k2];
const int soundPin = A0; //sound sensor attach to A0

int digitValue ; 
void setup(){
  
  pinMode(soundPin, INPUT);  //analog mic input
  pinMode(1,OUTPUT);//set pin0 as OUTPUT
 // pinMode(13,OUTPUT);
  Serial.begin(9600); //initialize serial
  lcd.begin(16, 2);
   
cli();//stop interrupts


  
//set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15;// = (16*10^6) / (500*1024) - 1 (must be <65536) //15624
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

sei();//allow interrupts

}//end setup

ISR(TIMER1_COMPA_vect){//timer1 interrupt 1Hz
  iteration++;
  data[k] = analogRead(soundPin);//read the value of A0
 // Serial.print(data[k]);//print the value
 // Serial.print("-");
  k++;
  if(k == sizeR )
  {
    //Serial.println();
     k = 0;
  }
}
void loop(){
   /*digitValue=digitalRead(2);
  if (digitValue==LOW)
  {
    digitalWrite (13, HIGH);
    delay(50);
    }   
  else 
  {
    digitalWrite (13, LOW);
    delay(10);
    }*/
  
  if ( iteration > sizeR-1 )    //generating Rx when every 50 signal recived
  { cli();
///////////////////////////////////////
//Serial.println("R Buffer");
    for (i=0 ; i<sizeR ; i++)   // for Rx(l)
      {
        for(j=0 ; j<sizeR ; j++)  // for x(n)*x(n-l)
        {
          if (j>= i){
          R[i] += data[j] * data[j-i];
          }
         /* Serial.print(R[i]);
          Serial.print("+");*/
        }  
        
       /* Serial.print(R[i]);
        Serial.print("-");*/
      }
      
       // Serial.println();
    thFlag=0;       // First rise edge  > Th
    for (RFlag=1; RFlag<sizeR ; RFlag++)   
    {
     // Serial.print(R[RFlag]);
       // Serial.print("-");
            if (R[RFlag]>thr && !thFlag) 
            {notcount =0;
              freqdetect(RFlag);
              notbuffer[2] = notbuffer[0];
              convert_sequence(frequence+1);
              thFlag = 1;
            }
    
    }
    
//////////////////////////////////////    
        iteration =0;
   
   if(notbuffer[0] != notbuffer[2]) {
      Serial.print(notbuffer[0]);
      Serial.print(notbuffer[1]);
      Serial.print("-"); 
     
     if(lcdFlag == 0)
         {lcd.setCursor(0,0);
          lcd.print("                ");
          lcd.setCursor(0,0);
          }
      lcdFlag++;
     if(lcdFlag == 2)
         {lcd.setCursor(0,1);
         lcd.print("                ");
          lcd.setCursor(0,1);
         lcdFlag = 0;}
         
      lcd.print(notbuffer[0]);
      lcd.print(notbuffer[1]);
      lcd.print(" ");
      
     }
      
     for(i=0;i<sizeR;i++) {   
      R[i] =0;
      }
      
  }
 if(iteration == 0) sei();
}

void freqdetect(int tt)     // calculate T 
{
  frequence = 1/(tt * cons);
}

void convert_sequence(float freq)
{
  notcount = 0;
      /*if(freq>16.3 && freq <18.35)
      {
        notbuffer[notcount++]='C';
        notbuffer[notcount++] ='0';
      }
      else if(freq>18.34 && freq <20.6)
      {
        notbuffer[notcount++]='D';
        notbuffer[notcount++] ='0';
      }
      else if(freq>20.59 && freq <21.83)
      {
        notbuffer[notcount++]='E';
        notbuffer[notcount++] ='0';
      }
      else if(freq>21.82 && freq <24.5)
      {
        notbuffer[notcount++]='F';
        notbuffer[notcount++] ='0';
      }
      else if(freq>24.49 && freq <27.5)
      {
        notbuffer[notcount++]='G';
        notbuffer[notcount++] ='0';
      }
      else if(freq>27.49 && freq <30.87)
      {
        notbuffer[notcount++]='A';
        notbuffer[notcount++] ='0';
      }
      else if(freq>30.86 && freq <32.70)
      {
        notbuffer[notcount++]='B';
        notbuffer[notcount++] ='0';
      }
      //---------------------------------------------------
      else if(freq>32.69 && freq <36.71)
      {
        notbuffer[notcount++]='C';
        notbuffer[notcount++] ='1';
      }
      else if(freq>36.70 && freq <41.20)
      {
        notbuffer[notcount++]='D';
        notbuffer[notcount++] ='1';
      }
      else if(freq>41.19 && freq <43.65)
      {
        notbuffer[notcount++]='E';
        notbuffer[notcount++] ='1';
      }
      else if(freq>43.64 && freq <49.00)
      {
        notbuffer[notcount++]='F';
        notbuffer[notcount++] ='1';
      }
      else if(freq>48.99 && freq <55.00)
      {
        notbuffer[notcount++]='G';
        notbuffer[notcount++] ='1';
      }
      else if(freq>49.99 && freq <61.74)
      {
        notbuffer[notcount++]='A';
        notbuffer[notcount++] ='1';
      }
      else if(freq>61.74 && freq <65.41)
      {
        notbuffer[notcount++]='B';
        notbuffer[notcount++] ='1';
      }*/
    //----------------------------------------------
      if(freq>65.40 && freq <69.30)
      {
        notbuffer[0]="C";
        notbuffer[1] ="2";
      }
      else if(freq>69.30 && freq <73.42)
      {
        notbuffer[0]="C#2/";
        notbuffer[1] ="Db2";
      }
      else if(freq>73.42 && freq <77.78)
      {
        notbuffer[0]="D";
        notbuffer[1] ="2";
      }
       else if(freq>77.78 && freq <82.41)
      {
        notbuffer[0]="D#2/";
        notbuffer[1] ="Eb2";
      }
      else if(freq>82.40 && freq <87.31)
      {
        notbuffer[0]="E";
        notbuffer[1] ="2";
      }
      else if(freq>87.30 && freq <92.50)
      {
        notbuffer[0]="F";
        notbuffer[1] ="2";
      }
      else if(freq>92.50 && freq <98.00)
      {
        notbuffer[0]="F#2/";
        notbuffer[1] ="Gb2";
      }
      else if(freq>97.99 && freq <103.83)
      {
        notbuffer[0]="G";
        notbuffer[1] ="2";
      }
      else if(freq>103.82 && freq <110.00)
      {
        notbuffer[0]="G#2/";
        notbuffer[1] ="Ab2";
      }
      else if(freq>109.99 && freq <116.54)
      {
        notbuffer[0]="A";
        notbuffer[1] ="2";
      }
      else if(freq>116.53 && freq <123.46)
      {
        notbuffer[0]="A#2";
        notbuffer[1] ="Bb2";
      }
      else if(freq>123.46 && freq <130.81)
      {
        //Serial.println(notbuffer[notcount]);
        notbuffer[0]="B";
        notbuffer[1] ="2";
      }
    //----------------------------------------------
      else if(freq >130.80 && freq <138.59)
      {
        notbuffer[0]="C";
        notbuffer[1] ="3";
      }
      else if(freq >138.59 && freq <146.83)
      {
        notbuffer[0]="C#3/";
        notbuffer[1] ="Db3 ";
      }
      else if(freq>146.82 && freq <155.56)
      {
        notbuffer[0]="D";
        notbuffer[1] ="3";        
      }
      else if(freq>155.56 && freq <164.81)
      {
        notbuffer[0]="D#3/";
        notbuffer[1] ="Eb3";
      }
      else if(freq>164.81 && freq <174.61)
      {
        notbuffer[0]="E";
        notbuffer[1] ="3";
      }
      else if(freq>174.60 && freq <185.00)
      {
        notbuffer[0]="F";
        notbuffer[1] ="3";
      }
       else if(freq>185.00 && freq <196.00)
      {
        notbuffer[0]="F#3/";
        notbuffer[1] ="Gb3";
      }
      else if(freq>195.99 && freq <207.60)
      {
        notbuffer[0]="G";
        notbuffer[1] ="3";
      }
      else if(freq>207.60 && freq <220.00)
      {
        notbuffer[0]="G#3/";
        notbuffer[1] ="Ab3";
      }
      else if(freq>219.99 && freq <233.08)
      {
        notbuffer[0]="A";
        notbuffer[1] ="3";
      }
      else if(freq>233.08 && freq <246.94)
      {
        notbuffer[0]="A#3/";
        notbuffer[1] ="Bb3";
      }
      else if(freq>246.93 && freq <261.63)
      {
        notbuffer[0]="B";
        notbuffer[1] ="3";
      }
    //----------------------------------8------------
      else if(freq >261.62 && freq <277.)
      {
        notbuffer[0]="C";
        notbuffer[1] ="4";
      }
      else if(freq >277.18 && freq <293.66)
      {
        notbuffer[0]="C#4/";
        notbuffer[1] ="Db4";
      }
      else if(freq>293.65 && freq <311.13)
      {
        notbuffer[0]="D";
        notbuffer[1] ="4";
      }
      else if(freq>311.13 && freq <329.63)
      {
        notbuffer[0]="D#4/";
        notbuffer[1] ="Eb4";
      }
      else if(freq>329.62 && freq <349.23)
      {
        notbuffer[0]="E";
        notbuffer[1] ="4";
      }
      else if(freq>349.22 && freq <369.99)
      {
        notbuffer[0]="F";
        notbuffer[1] ="4";
      }
      else if(freq>369.99 && freq <391.99)
      {
        notbuffer[0]="F#4/";
        notbuffer[1] ="Gb4";
      }
      else if(freq>391.99 && freq <415.30)
      {
        notbuffer[0]="G";
        notbuffer[1] ="4";
      }
      else if(freq>415.30 && freq <440.00)
      {
        notbuffer[0]="G#4/";
        notbuffer[1] ="Ab4";
      }
      else if(freq>439.99 && freq <466.16)
      {
        notbuffer[0]="A";
        notbuffer[1] ="4";
      }
      else if(freq>466.16 && freq <493.88)
      {
        notbuffer[0]="A#4/";
        notbuffer[1] ="Bb4";
      }
      else if(freq>493.87 && freq <523.25)
      {
        notbuffer[0]="B";
        notbuffer[1] ="4";
      }
    //----------------------------------------------
    /*  else if(freq >523.24 && freq <587.33)
      {
        notbuffer[notcount++]="C";
        notbuffer[notcount++] ='5';
      }
      else if(freq>587.32 && freq <659.25)
      {
        notbuffer[notcount++]='D';
        notbuffer[notcount++] ='5';
      }
      else if(freq>659.24 && freq <698.46)
      {
        notbuffer[notcount++]='E';
        notbuffer[notcount++] ='5';
      }
      else if(freq>698.46 && freq <783.99)
      {
        notbuffer[notcount++]='F';
        notbuffer[notcount++] ='5';
      }
      else if(freq>783.98 && freq <880.00)
      {
        notbuffer[notcount++]='G';
        notbuffer[notcount++] ='5';
      }
      else if(freq>879.99 && freq <987.77)
      {
        notbuffer[notcount++]='A';
        notbuffer[notcount++] ='5';
      }
      else if(freq>987.77 && freq <1046.50)
      {
        notbuffer[notcount++]='B';
        notbuffer[notcount++] ='5';
      }
  */
}
