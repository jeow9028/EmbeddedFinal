/******************************************************************************
 * @Title: uart.c
 *
 * @authors Adam Smrekar
 * @date November 30th, 2017
 * @version - ---
 *
 * Compiled using CCSv7
 *
 * ***************************************************************************/

#include "uart.h"
#include "msp.h"

void UART_Configure(){
    UCA2CTLW0 |= UCSWRST;                  // Put eUSCI in reset
/* Set Pins */
    P3SEL0 |= (BIT2 | BIT3);               // TX & RX Primary mode
    P3SEL1 &= ~(BIT2 | BIT3);
    P1SEL0 |= (BIT2 | BIT3);               // TX & RX Primary mode
    P1SEL1 &= ~(BIT2 | BIT3);
/* Select Frame parameters and clock source */
    UCA2CTLW0 |= EUSCI_A_CTLW0_SSEL__SMCLK;
    UCA2CTLW0 &= ~EUSCI_A_CTLW0_PEN        // Parity disabled
              &  ~EUSCI_A_CTLW0_MODE0      // Set to uart mode
              &  ~EUSCI_A_CTLW0_MODE1
              &  ~EUSCI_A_CTLW0_MSB        // LSB first
              &  ~EUSCI_A_CTLW0_SEVENBIT   // 8 bit character length
              &  ~EUSCI_A_CTLW0_SPB;       // One stop bit one start bit is default
/* Baud Rate == 115200 */
    UCA2MCTLW |= 0x0080 | EUSCI_A_MCTLW_OS16;
    UCA2MCTLW &= 0xB5A1;
    UCA2BR0 = 0x01;                        // Set Baud Rate
    UCA2BR1 = 0x00;
    UCA2CTLW0 &= ~UCSWRST;                 // Initialize eUSCI
    //UCA2IE |= EUSCI_A_IE_RXIE;             // Enable interrupt for RX receive
    NVIC_EnableIRQ(EUSCIA2_IRQn);          // Enable IRQ for UART
}

/* Send a byte of data */
void UART_sent_byte(uint8_t tx_data){
    while(EUSCI_A_IFG_TXIFG & ~UCA2IFG);   // While there is a Transmit flag
    EUSCI_A2->TXBUF = tx_data;             // TX is the data that you want to transmit
}

/* Send multiple bytes of data */
void UART_sent_n(uint8_t * data, uint32_t length){
    volatile int i = 0;                    // Initialize counter
    for(i; i < length; i++){
        char test = data[i];
        UART_sent_byte(data[i]);           // Loop through data and send
    }
}

