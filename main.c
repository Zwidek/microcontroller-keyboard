#include<msp430x14x.h>
#include "uart.h"
#include "lcd.h"
#include "portyUart.h" 
#include "portyLcd.h" 
#define BUTTON1 (P4IN & BIT4)
#define BUTTON2 (P4IN & BIT5)
#define BUTTON3 (P4IN & BIT6)
#define BUTTON4 (P4IN & BIT7)

char slownik[3][9] = {
  {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i'},
  {'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r'},
  {'s', 't', 'u', 'v', 'w', 'x', 'y', 'z', ' '}
};
int licznik_btn_1 = -1;
int licznik_btn_2 = -1;
int licznik_btn_3 = -1;

char Bufor[30];                         // bufor odczytywanych danych
int low=0;                              // znacznik pocz¹teku danych w buforze  
int high=0;                             // zmacznik koñca danych w buforze

void main(void)
{

 WDTCTL=WDTPW + WDTHOLD;                // wy³¹czenie WDT
  
 InitPortsLcd();                        // inicjalizacja portów LCD
 InitLCD();                             // inicjalizacja LCD
 clearDisplay();                        // czyszczenie wyœwietlacza
 initPortyUart();                       // inicjalizacja portow UART  
 initUart(1200);                        // inicjalizacja UARTa prêdkoœæ transmisji 2400 Budoów  

 _EINT();                               // w³¹czenie przerwañ 
int is_btn1_clicked = 0;
int is_btn2_clicked = 0;
int is_btn3_clicked = 0;
int is_btn4_clicked = 0;
while(1)                                // nieskoñczona pêtla
 {
    if (!BUTTON1 && !is_btn1_clicked) {
      licznik_btn_1 = (licznik_btn_1 + 1)%9;
      licznik_btn_2 = -1;
      licznik_btn_3 = -1;
      is_btn1_clicked = 1;
    }
    if (BUTTON1 && is_btn1_clicked){
      is_btn1_clicked = 0;
    }
    if (!BUTTON2 && !is_btn2_clicked) {
      licznik_btn_2 = (licznik_btn_2 + 1)%9;
      licznik_btn_1 = -1;
      licznik_btn_3 = -1;
      is_btn2_clicked = 1;
    }
    if (BUTTON2 && is_btn2_clicked){
      is_btn2_clicked = 0;
    }
    if (!BUTTON3 && !is_btn3_clicked) {
      licznik_btn_3 = (licznik_btn_3 + 1)%9;
      licznik_btn_2 = -1;
      licznik_btn_1 = -1;
      is_btn3_clicked = 1;
    }
    if (BUTTON3 && is_btn3_clicked){
      is_btn3_clicked = 0;
    }
    if (!BUTTON4 && !is_btn4_clicked) {
      is_btn4_clicked = 1;
      if (licznik_btn_1 != -1){
        UartCharTransmit(slownik[0][licznik_btn_1]);
      }
      else if (licznik_btn_2 != -1){
        UartCharTransmit(slownik[1][licznik_btn_2]);
      }
      else if (licznik_btn_3 != -1){
        UartCharTransmit(slownik[2][licznik_btn_3]);
      }
    }
    if (BUTTON4 && is_btn4_clicked){
      is_btn4_clicked = 0;
    }

 }
}


#pragma vector=UART0RX_VECTOR           // procedura obs³ugi przerwania UART
__interrupt void usart0_rx (void)
{ 
Bufor[high]=RXBUF0;                     // wpisanie odebranych danych do bufora
high=(++high)%30;                       // inkrementowanie znacznika koñca danych
}