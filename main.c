// filename ******** Main.C ************** 

//NAME: Akiv Jhirad

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"  /* derivative information */
#include "SCI.h"

char string[20];
unsigned short val1;
unsigned short count=0, temp=0;

void ISR_Vtimch0(void);
void delayby1ms(int);

//---------------------OutCRLF---------------------
// Output a CR,LF to SCI to move cursor to a new line
// Input: none
// Output: none
// Toggle LED each time through the s = serial('COM4');     %choose your COM port

void OutCRLF(void){
  SCI_OutChar(CR);
  SCI_OutChar(LF);
  PTJ ^= 0x20;          // toggle LED D2
}

void main(void) {
// Setup and enable ADC channel 0 (AN0)
// Refer to Chapter 14 in S12G Reference Manual for ADC subsystem details
	
	//ADC for all of them
	//ATDCTL0 = 0x01;   // continuous conversion
	ATDCTL1 = 0x4B;		// set for 12-bit resolution, external trigger source is channel 11 
	ATDCTL3 = 0x88;		// right justified, one sample per sequence
	ATDCTL4 = 0x02;		// prescaler = 2; ATD clock = 6.25MHz / (2 * (2 + 1)) == 1.04MHz
	ATDCTL5 = 0x2B;		// continuous conversion on channel 11
  //ATDCTL5 = 0x20;		// continuous conversion on channel 0
  
	//setting the clock 
 CPMUCLKS = 0x80;  //PLLSEL = 1
 CPMUOSC  = 0x00;  //OSCE = 0
 CPMUSYNR = 0x0F;  //VCOFRQ = 0, SYNDIV = 15
 CPMUPOSTDIV = 0x00;  // postdiv=0  
 
 CPMUFLG  = 0x00;
 
 TSCR1=0x90;   //Timer System Control Register 1
 TSCR2=0x00;   //Timer System Control Register 2
 TIOS=0xFE;
 
 PERT=0x01;    //enable pull-up resistor on TIC[0]
  
 TCTL3=0x00;   //TCTL3 & TCTL4 configure which edges to capture
 TCTL4=0x02;   //configured for falling edge on TIC[0]
 
 TIE=0x01;
	
	// bus clock = 16Mhz
	
// Setup LED and SCI
  DDRJ |= 0x01;     // PortJ bit 0 is output to LED D2 on DIG13
  SCI_Init(9600);
  
  EnableInterrupts;
  
  //to make a sampling frequency of 300Hz, add a time delay of 3ms
  for(;;) {
    /*PTJ ^= 0x01;          // toggle LED     
    val1 = ATDDR0;       // read analog input from channel 11
    SCI_OutUDec(val1);    // output analog reading via serial
    OutCRLF();
    delayby1ms(3); 
    */
  }           
}

interrupt VectorNumber_Vtimch0 void ISR_Vtimch0(void){
  //int i;
  if (!(PTIT & (0x01))){            
    for(;;) {
        PTJ ^= 0x01;          // toggle LED     
        val1 = ATDDR0;       // read analog input from channel 11
        SCI_OutUDec(val1);    // output analog reading via serial
        OutCRLF();
        delayby1ms(3); 
    }    
    temp=TC0;   //refer back to TFFCA, we enabled FastFlagClear, thus by reading Timer Capture 
                //input we automatically clear the flag, allowing another TIC interrupt
  }
}

void delayby1ms(int k)
{
      int ix;
      TSCR1 = 0x90;  /* enable timer and fast timer flag clear */
      TSCR2 = 0x00;  /* disable timer interrupt, set prescaler to 1*/
      TIOS |= 0x01; /* enable OC0 */        //(not necessary)
      TC0 = TCNT + 16000;
      for(ix = 0; ix < k; ix++) {
             while(!(TFLG1_C0F));
             TC0 += 16000;
      }
      TIOS  &= ~0x01;   /* disable OC0 */  //(not necessary)
}
