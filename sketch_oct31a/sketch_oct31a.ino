
/*
 Arduino inputs-outputs test
 Analog and digital pins are tested as digital i/o ports. Exceptions:
 a) pins D0 and D1 are regarded as tested if USB-channel works well.
 b) analog inputs A6 and A7 are tested in analoge mode
 
 The circuit:
 All Arduino pins D2...D12 and A0...A7 are connected to a common
 point via small resistors (200 Ohm).
 Ports' numbers are regarded as Arduino pins: D2...D12 are Ports 2...12
 D13 is tested visually with BLINK), then A0...A5 are Ports 14...19
 
 Idea: Each port pin (in sequence) is configured as output while
 all other pins are inputs (without pullup resistors). The TESTED pin
 sets HIGH and LOW levels and ATmega168 measures the voltage via A6
 and A7 ADC channels. Then the pin is read via the next digital input.
 
 Operator can observe the results of complete test (a table is printed,
 2 seconds are waited an the allports test is repeated) or he can give
 commands:
 s - toggles STEP mode. In the mode onle ome Port is tested and only in
 one level (initially HIGH). The result is printed and the test is repeated.
 When operator gives a SPACE command, the level is set to LOW and next
 SPACE increases the port number (level HIGH). So operator can test all
 ports at both levels.
 The "b" command skips to the first Potr (number 2).
 The "q" command stops the program.
*/

#define VERSION 1

#define SHORT_TICK 50
#define LONG_TICK 2000
#define smillis() ((long)millis())

#define TESTED port[OutIndx]
#define READ port[InpIndx]

typedef enum TEST_PHASE { RUN, DIGITAL_HIGH, DIGITAL_LOW} phases;

const int port[] =     // All ports that are to be tested
// 0 1 2 3 4 5 6 7 8  9  10        11 12 13 14 15 16     it is "Indx"
{ 2,3,4,5,6,7,8,9,10,11,12,/*13,*/14,15,16,17,18,19};
//D2D3D4D5D6D7D8D9D10D11D12   D13  A0 A1 A2 A3 A4 A5
const byte portsNmb = sizeof(port)/2;

 byte Return[ portsNmb][2];    // for accumulating the result of test
 int InpIndx, OutIndx, i, j;
 int inpHiLevel[ portsNmb][2], inpLoLevel[ portsNmb][2];
 char s[80], Char;
 long StopTime, Millis, TickDuration;
 phases Phase = RUN;

void setup()
{
 for( InpIndx = 0; InpIndx < portsNmb-1; InpIndx++)
 {
   pinMode( READ, INPUT);
   digitalWrite( READ, LOW);
 }
 Serial.begin(9600);
 Serial.println( " ");
 Serial.print( "Arduino Nano ports test.       version: ");
 Serial.println(VERSION);  
 Serial.println( "DrVlas, 2010");
 Serial.println( " ");
 OutIndx = 0;  
 TickDuration = SHORT_TICK;
}
/********************************************* L O O P *************************/
void loop()
{
/************************************** Timeout 1 ******************************/  
 Millis = smillis();
 if( Millis - StopTime > 0)
 {
//    Serial.print(StopTime); Serial.print( " "); Serial.println( Millis);
   StopTime = (Millis / TickDuration + 1) * TickDuration;
   pinMode( 13, OUTPUT);            // BLINK output is not tested as others
   digitalWrite( 13, HIGH);
   
/******************************* Get command and parse it ************/    
   if( Serial.available() > 0)
   {
     Char = Serial.read();
     switch( Char)
     {
       case 'b':          // BEGIN command
       case 'B':
         OutIndx = 0;
         break;
       case 's':          // STEP toggle command
       case 'S':
         if( Phase == RUN)
         {
           Phase = DIGITAL_HIGH;      // Set STEP mode
           TickDuration = LONG_TICK;
         }
         else
         {
           Phase = RUN;
           TickDuration = SHORT_TICK;
         }
         break;
       case ' ':          // Next port command
         switch( Phase)
         {
           case DIGITAL_HIGH: Phase = DIGITAL_LOW;  break;
           case DIGITAL_LOW:
             pinMode( TESTED, INPUT);
             digitalWrite( TESTED, LOW);        
             if( ++OutIndx >= portsNmb) { OutIndx = 0; }
             Phase = DIGITAL_HIGH;  
         }
         break;
       case 'q':
       case 'Q':
         Serial.println( " ");
         Serial.println( "Thanks for using the program!");
         Serial.println( " ");
         while(1) ;
     }
   }    
   pinMode( 13, INPUT);
   digitalWrite( 13, LOW);        // High impedance state of the pin
   
/******************************* Output and Input ********************/    
   pinMode( TESTED, OUTPUT);
   if( (Phase == DIGITAL_HIGH) || (Phase == RUN))
   {
     digitalWrite( TESTED, HIGH);
     InpIndx = OutIndx + 1;
     if( InpIndx >= portsNmb) InpIndx = 0;      
     inpHiLevel[OutIndx][0] = map( analogRead(6), 0, 1024, 0, 5000);
     inpHiLevel[OutIndx][1] = map( analogRead(7), 0, 1024, 0, 5000);
     Return[InpIndx][1] = digitalRead( READ);
   }

   if( (Phase == DIGITAL_LOW) || (Phase == RUN))
   {
     digitalWrite( TESTED, LOW);
     inpLoLevel[OutIndx][0] = map( analogRead(6), 0, 1024, 0, 5000);
     inpLoLevel[OutIndx][1] = map( analogRead(7), 0, 1024, 0, 5000);
     Return[InpIndx][0] = digitalRead( READ);
   }

   if( Phase == RUN)
   {
     pinMode( TESTED, INPUT);
     digitalWrite( TESTED, LOW);        
     if( ++OutIndx >= portsNmb) { OutIndx = 0; }
   }
   pinMode( 13, OUTPUT);  
   digitalWrite( 13, LOW);
   
/******************************* Print results of test ******/    
   switch( Phase)
   {
     case RUN:
       if( OutIndx == 0)
       {
         Serial.println( "Port    U6/U7 @ LOW    U6/U7 @ HIGH  inpPin  Pin(LOW/HIGH)");
         for( i = 0; i <= portsNmb-1; i++)
         {
           j = (i+1==portsNmb? 0:i+1);
           sprintf( s, " %02d     %04d / %04d     %04d / %04d    %02d       %d / %d",
           port[i], inpLoLevel[i][0], inpLoLevel[i][1], inpHiLevel[i][0], inpHiLevel[i][1],
           port[j], Return[j][0], Return[j][1]);
           Serial.print( s);  
           if( Return[j][1] && (!Return[j][0])) Serial.println( "    Port test OK!");
           else                                 Serial.println( "    Port test failed");
         }
         StopTime += (2*LONG_TICK);
       }
       break;
     case DIGITAL_HIGH:
       i = OutIndx;
       j = (i+1==portsNmb? 0:i+1);
       sprintf( s, "Port %02d HIGH    %04d / %04d    %02d       %d",
       port[i], inpHiLevel[i][0], inpHiLevel[i][1], port[j], Return[j][1]);
       Serial.print( s);  
       if( Return[j][1]) Serial.println( "    Port test OK!");
       else              Serial.println( "    Port test failed");
       break;
     case DIGITAL_LOW:
       i = OutIndx;
       j = (i+1==portsNmb? 0:i+1);
       sprintf( s, "Port %02d LOW     %04d / %04d    %02d       %d",
       port[i], inpLoLevel[i][0], inpLoLevel[i][1], port[j], Return[j][0]);
       Serial.print( s);  
       if( !Return[j][0]) Serial.println( "    Port test OK!");
       else               Serial.println( "    Port test failed");
       break;
     default: Serial.println( "Error in CASE");  
   }    // End of PRINT section
 }             // End of Timer1 handling
}                        // End of loop()

