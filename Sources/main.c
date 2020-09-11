/* Autor: Sara Skutova
 * Login: xskuto00
 * Projekt: Simulace v CW: Svetelne noviny
 * Datum: 16.12. 2014
 * Do projektu jsem pouzila a modifikovala funkci delay ze 3. civiceni
 */

#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
void led_init(void);
void led_horiz(void);
void led_vertik(void);
void led_delay(void);

byte LED[64] @0x0200;  //pole s loginem
unsigned char LED_help[64]; //pomocne pole pro vertikalni posun

byte pomoc_carry @0xB0; //pomoc promenna pro vertikalni posun

/* Tlacitka a posuvnik */
byte init @0x250;
byte horiz @0x251;
byte vertik @0x252;
byte rychlost @0x253;

int pom_init;
int pom_horiz;
int pom_vertik;



void main(void) {
int i;
/* pocatescni Stavy programmu/simulace*/
 pom_init = 0;
 pom_horiz = 0;
 pom_vertik = 0;

/*Indikace stisknutych klaves*/
horiz = 0;
init = 0;
vertik = 0;

rychlost = 50; //default hodnota posuvniku

  EnableInterrupts; /* enable interrupts */
  /* include your code here */
 
 /*vynulovani pole pro login a pomoceho pole*/ 
  for (i = 0; i < 64; i++) {
    
      LED[i] = 0;
      LED_help[i] = 0;
      
  }
  
  led_init();


  /*hlavni programova smycka*/
  for(;;) {
  
      /*Testovani stisku jednotlivych tlacitek a volani danych funkci k nim*/
    if (init != 0 || pom_init == 1) {
        for (i = 0; i < 64; i++)
            LED_help[i] = 0;
        pom_horiz = 0;
        pom_vertik = 0;
        pom_init = 0;
        led_init();

       
    }
     
    if (horiz != 0 || pom_horiz == 1) {
        pom_horiz = 1;
        pom_vertik = 0;
        
        led_horiz();
        
        led_delay();
    
    }
    
    if (vertik != 0 || pom_vertik == 1) {
        pom_vertik = 1;
        pom_horiz = 0;
        
        led_vertik();
        
        led_delay();
    }
      
    __RESET_WATCHDOG(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}

/*funkce init, inicializuje pole s loginem, vola se i po stisku tlacitak init*/
void led_init(void) {
        /*0b76543210 */
  /* X */      
  LED[0] = 0;
  LED[1] = 0b01000100;
  LED[2] = 0b01101100;
  LED[3] = 0b00111000;
  LED[4] = 0b00111000;
  LED[5] = 0b01101100;
  LED[6] = 0b01000100;
  LED[7] = 0;
  
  /* S */
  LED[8] = 0;
  LED[9] = 0b01011000;
  LED[10] = 0b01011100;
  LED[11] = 0b01010100;
  LED[12] = 0b01010100;
  LED[13] = 0b01110100;
  LED[14] = 0b00110100;
  LED[15] = 0;
  
  /* K */
  LED[16] = 0;
  LED[17] = 0b01111110;
  LED[18] = 0b01111110;
  LED[19] = 0b00011000;
  LED[20] = 0b00111100;
  LED[21] = 0b01000110;
  LED[22] = 0b01000000;
  LED[23] = 0;
  
  /* U */
  LED[24] = 0;
  LED[25] = 0b00111000;
  LED[26] = 0b01111100;
  LED[27] = 0b01000000;
  LED[28] = 0b01000000;
  LED[29] = 0b01111100;
  LED[30] = 0b00111000;
  LED[31] = 0;
  
  /* T */
  LED[32] = 0;
  LED[33] = 0b00001000;
  LED[34] = 0b01111110;
  LED[35] = 0b01111110;
  LED[36] = 0b01000100;
  LED[37] = 0b01001100;
  LED[38] = 0b00100000;
  LED[39] = 0;
  
  /* O */
  LED[40] = 0;
  LED[41] = 0b00111000;
  LED[42] = 0b01111100;
  LED[43] = 0b01000100;
  LED[44] = 0b01000100;
  LED[45] = 0b01111100;
  LED[46] = 0b00111000;
  LED[47] = 0;
  
  /* 0 */
  LED[48] = 0;
  LED[49] = 0b00111100;
  LED[50] = 0b00111110;
  LED[51] = 0b01010010;
  LED[52] = 0b01001010;
  LED[53] = 0b01111110;
  LED[54] = 0b01111100;
  LED[55] = 0;
  
  /* 0 */
  LED[56] = 0;
  LED[57] = 0b00111100;
  LED[58] = 0b00111110;
  LED[59] = 0b01010010;
  LED[60] = 0b01001010;
  LED[61] = 0b01111110;
  LED[62] = 0b01111100;
  LED[63] = 0;
  }
 
 /*Horizontalni posun*/ 
  void led_horiz(void) {
  
  byte pomoc;
  byte pomoc_carry2;
  int i;

  pomoc = LED[0];
  pomoc_carry2 = LED_help[0];
   
  //pole o 1 mensi, do posledniko LED[63] na hraju pomoc, do dalsich nahraju jejich nasledniky
  for (i = 0; i < 63; i++) {
     LED[i] = LED[i + 1];
     LED_help[i] = LED_help[i + 1];
  }
  LED[63] = pomoc;
  LED_help[63] = pomoc_carry2;
  

  }
  
  /*Vertikalni posun a assemblerovske neco*/
  void led_vertik(void) {
  int i;
  byte pomoc;
  pomoc_carry = 0;
  

 /*Každý sloupec binarnì rotuji, jelikož rotace pøechazí pøes carry musím pøenastavovat carry*/
 /*Velice doufam, ze se nic nezkazi*/ 
  for (i = 0; i < 64; i++) {
      pomoc = LED[i];
      
      if (LED_help[i] == 0)  
          __asm CLC;         
      else
          __asm SEC;         
      
      asm {
          ROL pomoc        
          
          BCC nula        
          
          MOV #1, $B0     
          JMP konec_carry
          
          nula: 
            MOV #0, $B0 
            
          konec_carry:
      }
      
      
     LED_help[i] = pomoc_carry;  
     LED[i] = pomoc;             
     
   }
  }
  
  /*funkce reakce na hodnoty posuvniku, zrychleni a zpomaleni*/
  void led_delay(void) {
      int i;
      
      for(i = 0; i < (6000-(rychlost*50)); i++) {
      
              asm nop;
      }
  }