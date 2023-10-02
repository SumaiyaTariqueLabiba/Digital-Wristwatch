#include <mega32.h>
#include <delay.h>
#include <alcd.h>
#include <stdlib.h>

char display_str[80];
int sec = 52, minute = 4, hour = 4; //
int day = 22, month = 8, ampm = 1;
int minutee = 0, houre = 0, ampme = 0, minutes = 0, hours = 0, secs =0;
char presscount = 0;
float time_elapsed = 1.0;

void display_int(int num) {
    itoa(num, display_str);
    lcd_puts(display_str);
}

void display_clock() {
    lcd_init(20);
    lcd_gotoxy(0, 0); //(position,line)
    if (hour < 10) {
        lcd_putsf("0");
    }
    display_int(hour);
    lcd_putsf(":");
    if (minute < 10) {
        lcd_putsf("0");
    }
    display_int(minute);
    lcd_putsf(":");
    if (sec < 10) {
        lcd_putsf("0");
    }
    display_int(sec);

    if (ampm == 1) {
        lcd_putsf(" PM");
    }
    if (ampm == 0) {
        lcd_putsf(" AM");
    }

    lcd_gotoxy(15, 0);
    if (day < 10) {
        lcd_putsf("0");
    }
    display_int(day);
    
    lcd_putsf("/");
    
    if (month < 10) {
        lcd_putsf("0");
    }
    display_int(month);
}
   
void clock(void) { //CLOCK   
     
    while(time_elapsed > 0) {
        time_elapsed = time_elapsed - 1.0;
        sec++;
        
        if (sec == 60) {
            sec = 0; //pm=1, am=0
            minute++;
            if (minute == 60) {
                minute = 0;
                hour++;
                if (hour == 12) {
                    if (ampm == 0) { //am                               
                        hour = 0;
                        ampm++;
                    }
                    if (ampm == 1) { //pm       
                        delay_ms(1000); // why delay?
                        hour = 0;
                        ampm++; 
                        
                        if (ampm == 2) {
                            ampm = 0;
                        }
                        day++;
                        
                        if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) {
                            if (day == 32) {
                                day = 1;
                                month++;
                            }
                        }
                        if (month == 4 || month == 6 || month == 9 || month == 11) {
                            if (day == 31) {
                                day = 1;
                                month++;
                            }
                        }
                        if (month == 2) {
                            if (day == 29) {
                                day = 1;
                                month++;
                            }
                        }
                        if (month == 13) {
                            month = 1;
                        }

                    }
                }
            }
        }
        if (minute == minutee && hour == houre && ampm == ampme) { /////////CHECK
            PINB.0 = 1;
            lcd_gotoxy(7,1); 
            lcd_putsf("ALARM!");
            delay_ms(3000);
            time_elapsed = time_elapsed - 3.0;          
            PINB.0 = 0;          
        }
    }   
    time_elapsed = 1.0;
    display_clock();
    delay_ms(1000);
}
                                 //kill me
 void set_clock(void){
     
     if(PINB.7==1)presscount++; 
     if(presscount==1){                //steady display
       display_clock();
      } 
       
   while(presscount==1){        
       lcd_gotoxy(6,0);  
       delay_ms(1000);
       time_elapsed = time_elapsed + 1;                                    
       sec++;
       if(sec<10)lcd_putsf("0");      // 10
       display_int(sec);
       //lcd_gotoxy(9,1); 
       //lcd_putsf(":");
       if(sec==60)sec=0;

     while(presscount==2){
        lcd_gotoxy(6,0);
        if(sec<10)lcd_putsf("0");
        sec=sec;
        display_int(sec);        
        lcd_gotoxy(3,0);  
        delay_ms(1000);
        time_elapsed = time_elapsed + 1;                                    
        minute++;
        if(minute<10)lcd_putsf("0");      // 10
        display_int(minute);
        lcd_gotoxy(5,1); 
        lcd_putsf(":");
        if(minute==60)minute=0; 

         while(presscount==3){
            lcd_gotoxy(3,0);
            if(minute<10)lcd_putsf("0");
            minute=minute;
            display_int(minute);
            lcd_gotoxy(0,0);            
            delay_ms(1000);
            time_elapsed = time_elapsed + 1; 
            hour++;
            if(hour<10)lcd_putsf("0"); 
            display_int(hour);
            lcd_gotoxy(2,0);                       
            lcd_putsf(":");        
            if(houre==13)houre=0; 
            
             while(presscount==4){
                 lcd_gotoxy(0,0);
                 if(hour<10)lcd_putsf("0"); 
                 hour=hour;
                 display_int(hour); 
                 lcd_gotoxy(9,0);
                 delay_ms(1000);
                 time_elapsed = time_elapsed + 1; 
                 ampm++;
                 if(ampm==2)ampme=0;
                 if(ampm==1)lcd_putsf("PM");
                 if(ampm==0)lcd_putsf("AM");
                   
                  while(presscount==5){
                   lcd_gotoxy(9,0);
                   ampm=ampm;
                   if(ampm==1)lcd_putsf("PM");
                   if(ampm==0)lcd_putsf("AM");
               
                   if(presscount==6){                       
                     clock();
                   }
                  } 
             }
         }
      } 
   }       
 }

void display_stopwatch(void){

      lcd_init(20);
      lcd_gotoxy(6,2);            //(position,line)
      if(hours<10)lcd_putsf("0"); 
      display_int(hours);
      lcd_putsf(":");
      if(minutes<10)lcd_putsf("0");
      display_int(minutes);
      lcd_putsf(":");
      if(secs<10)lcd_putsf("0");
      display_int(secs);  
 } 

interrupt [EXT_INT0] void ext_int0_isr(void)                // External Interrupt 0
 {
     //void alarm(void)
     #asm("sei") 
     presscount++; 
     if(presscount==1){         //steady display
       display_clock();
       lcd_gotoxy(0,1);
       lcd_putsf("Alarm: ");
       lcd_gotoxy(7,1);            //(position,line)           
       if(houre<10)lcd_putsf("0"); 
       display_int(houre);
       lcd_putsf(":");
       if(minutee<10)lcd_putsf("0");
       display_int(minutee);
       lcd_gotoxy(13,1);
       if(ampme==1)lcd_putsf("PM");
       if(ampme==0)lcd_putsf("AM");
      } 
      
     while(presscount==1){        
       lcd_gotoxy(10,1);  
       delay_ms(1000);
       time_elapsed = time_elapsed + 1;                                    
       minutee++;
       if(minutee<10)lcd_putsf("0");      // 10
       display_int(minutee);
       lcd_gotoxy(9,1); 
       lcd_putsf(":");
       if(minutee==60)minutee=0; 

         while(presscount==2){ 
            lcd_gotoxy(0,1);
            lcd_putsf("Alarm: ");
            lcd_gotoxy(10,1);
            if(minutee<10)lcd_putsf("0");
            minutee=minutee;
            display_int(minutee);
            lcd_gotoxy(7,1);            
            delay_ms(1000);
            time_elapsed = time_elapsed + 1; 
            houre++;
            if(houre<10)lcd_putsf("0"); 
            display_int(houre);
            lcd_putsf(":");        
            if(houre==13)houre=0; 
            
             while(presscount==3){
                 lcd_gotoxy(0,1);
                 lcd_putsf("Alarm: ");
                 lcd_gotoxy(7,1);
                 if(houre<10)lcd_putsf("0"); 
                 houre=houre;
                 display_int(houre); 
                 lcd_gotoxy(13,1);
                 delay_ms(1000);
                 time_elapsed = time_elapsed + 1; 
                 ampme++;
                 if(ampme==2)ampme=0;
                 if(ampme==1)lcd_putsf("PM");
                 if(ampme==0)lcd_putsf("AM");
                   
                  while(presscount==4){
                   lcd_gotoxy(0,1);
                   lcd_putsf("Alarm: "); 
                   lcd_gotoxy(13,1);
                   ampme=ampme;
                   if(ampme==1)lcd_putsf("PM");
                   if(ampme==0)lcd_putsf("AM");
               
                   if(presscount==5){                      
                     clock();
                   }
                  } 
             }
         }
      }    
 }

interrupt[EXT_INT1] void ext_int1_isr(void) // External Interrupt 1     // STOPWATCH
{
  #asm("sei")
  presscount++;
      while(presscount==1){
       delay_ms(1000);
       time_elapsed = time_elapsed + 1; 
      display_stopwatch();
      }
      
      while(presscount==2){ 
      delay_ms(1000);                          //
       time_elapsed = time_elapsed + 1; 
      secs++;
         if (secs==60){
         secs=0;
         minutes++;
           if (minutes==60){
           minutes=0;
           hours++;
             if (hours==100){
             hours=0;
             }
           }
         } 
         display_stopwatch();
      } 
                  
     while (presscount==3){
      delay_ms(100);
       time_elapsed = time_elapsed + 1;           //stopwatch stopped
        hours=hours; 
        minutes=minutes;  
        secs=secs;
     }
                          
     while(presscount==4){
      delay_ms(1000);
       time_elapsed = time_elapsed + 1;                  //stopwatch reset
        hours=0;
        minutes=0;
        secs=0;
        display_stopwatch();
     } 
     if(presscount==5)presscount=2; 
         
}        
            
interrupt[EXT_INT2] void ext_int2_isr(void) // External Interrupt 2
{
  #asm("sei")               
  while(1){
  clock(); 
  }
}

interrupt[TIM1_COMPA] void timer1_compa_isr(void) // Timer1 output compare A
{
    //#asm("sei")
    //presscount++;
   /* TCNT1H=0x00;
    TCNT1L=0x00;
    //while(presscount==1){
    //display_stopwatch();
      if(presscount==2){
      secs++;
         if (secs==60){
         secs=0;
         minutes++;
           if (minutes==60){
           minutes=0;
           hours++;
             if (hours==100){
             hours=0;
             }
           }
         }
      }         
      display_stopwatch(); 
      //}   
   */   
}

void main(void) {

    DDRB.0 = 1;
    DDRB.7 = 0;

    TCCR1A = (0 << COM1A1) | (0 << COM1A0) | (0 << COM1B1) | (0 << COM1B0) | (0 << WGM11) | (0 << WGM10); // Timer(s)/Counter(s) 
    TCCR1B = (0 << ICNC1) | (0 << ICES1) | (0 << WGM13) | (0 << WGM12) | (1 << CS12) | (0 << CS11) | (1 << CS10);
    TCNT1H = 0x00;
    TCNT1L = 0x00;
    ICR1H = 0x00;
    ICR1L = 0x00;
    OCR1AH = 0x3D;
    OCR1AL = 0x09;
    OCR1BH = 0x00;
    OCR1BL = 0x00;
    TIMSK = (0 << OCIE2) | (0 << TOIE2) | (0 << TICIE1) | (1 << OCIE1A) | (0 << OCIE1B) | (0 << TOIE1) | (0 << OCIE0) | (0 << TOIE0);
    GICR |= (1 << INT1) | (1 << INT0) | (1 << INT2); //interrupts
    MCUCR = (1 << ISC11) | (0 << ISC10) | (1 << ISC01) | (0 << ISC00);
    MCUCSR = (0 << ISC2);
    GIFR = (1 << INTF1) | (1 << INTF0) | (1 << INTF2);

    #asm("sei")
    while (1) { 
      if(PINB.7==1)set_clock();
      clock();     //alarm(); 
    }
}