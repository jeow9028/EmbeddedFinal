/******************************************************************************
 * @Title: uart.c
 *
 * @author Sam Zimmer
 * @date October 24, 2017
 * @version 1.0
 *
 * Compiled using CCSv7
 *
 * Description:
 * This file contains functions to configure the UART peripheral on the MSP432,
 * and send data over UART to a serial terminal
 * ***************************************************************************/

#include "uart.h"
#include <string.h>

buffer * tx;
buffer * rx;
uint8_t ready;

/*
 * Function: uart_config
 * ----------------------------
 *   Initializes and configures the UART
 *
 *   buad: baud rate for the UART module to operate at
 *
 *   returns: void
 */
void uart_config(uint16_t buad)
{
    uint16_t N = 12000000/buad;
    float p1 = (float)(N/16.0);
    float p2 = (float)((uint16_t)(N/16));
    uint16_t os = (uint16_t)((p1-p2)*16);

    P1->SEL0 |= BIT2 | BIT3;
    P1->SEL1  = 0;

    P3->SEL0|=BIT3|BIT2;

    EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SWRST;           // reset

    EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SSEL__SMCLK;     // BRCLK = SMCLK
    EUSCI_A2->BRW    = (uint16_t)((12000000/buad)/16); // 19200 Buad
    EUSCI_A2->MCTLW |= EUSCI_A_MCTLW_OS16 | (os<<4); //0x44 // oversampling

    EUSCI_A2->CTLW0 &=~EUSCI_A_CTLW0_SWRST;           // release for operation
    EUSCI_A2->IE    |= EUSCI_A_IE_RXIE;

    tx = (buffer *)buffer_initialize(256);
    rx = (buffer *)buffer_initialize(512);

    ready=0;

    NVIC_EnableIRQ(EUSCIA0_IRQn);
    NVIC_EnableIRQ(EUSCIA2_IRQn);
}

/*
 * Function: uart_write_byte
 * ----------------------------
 *   Forces one byte to be written to the TXBUF
 *
 *   data: byte to be written
 *
 *   returns: void
 */
void uart_write_byte(uint8_t data) {
    while (!EUSCI_A2->IFG & EUSCI_A_IFG_TXCPTIFG);
    EUSCI_A2->TXBUF=data;
}

/*
 * Function: uart_write_n
 * ----------------------------
 *   Forces multiple bytes to be written to the TXBUF
 *
 *   data: byte array to be written
 *
 *   returns: void
 */
void uart_write_n(uint8_t * data, uint8_t size) {
    if(size<=0||!data) return;
    uint32_t i;
    for(i=0; i<size; i++) {
        uart_write_byte(*data);
        data++;
    }
}

/*
 * Function: uart_send
 * ----------------------------
 *   Sends one byte to the UART TX buffer to be written when ready
 *
 *   byte: byte to be written
 *
 *   returns: void
 */
void uart_send(uint8_t byte) {
    buffer_add(tx,byte);
}

/*
 * Function: uart_print
 * ----------------------------
 *   Prints a string of bytes to the UART by adding each to the TX
 *   buffer and sending them via the tx ready interrupt
 *
 *   bytes: byte array to be written
 *
 *   returns: void
 */
void uart_print(uint8_t * bytes) {
    uint32_t i;
    int size = strlen(bytes);
    for(i=0;i<size;i++) {
        uart_send(bytes[i]);
    }
    EUSCI_A2->IE |= EUSCI_A_IE_TXIE;
}

/*
 * Function: uart_prinln
 * ----------------------------
 *   Does the same thing as uart_print but adds a new line character
 *   to the end of the buffer
 *
 *   bytes: byte array to be written
 *
 *   returns: void
 */
void uart_println(uint8_t * bytes) {
    uint32_t i;
    int size = strlen(bytes);
    for(i=0;i<size;i++) {
        uart_send(bytes[i]);
    }
    buffer_add(tx,13);
    EUSCI_A2->IE |= EUSCI_A_IE_TXIE;
}

/*
 * Function: EUSCIA0_IRQHandler
 * ----------------------------
 *   Handles the interrupts for the EUSCIA0 peripheral
 *
 */
void EUSCIA2_IRQHandler() {
    if(EUSCI_A2->IFG & EUSCI_A_IFG_TXIFG) {
        if(tx->num_items>0) {
            EUSCI_A2->TXBUF=buffer_get(tx);
        }
        else {
            EUSCI_A2->IE &=~EUSCI_A_IE_TXIE;
        }
    }
    if(EUSCI_A2->IFG & EUSCI_A_IFG_RXIFG) {
        uint8_t _r = EUSCI_A2->RXBUF;
        if(_r==13||_r==10||buffer_isfull(rx)) ready=1;
        buffer_add(rx,_r);
        //echo
        //EUSCI_A2->TXBUF = EUSCI_A2->RXBUF;
    }
}


/***CIRCULAR BUFFER CODE***/

/*
 * Function: buffer_initialize
 * ----------------------------
 *   Initializes a circular buffer of size len
 *
 *   len: length of the circular buffer
 *
 *   returns: pointer to initialized buffer
 */
buffer * buffer_initialize(uint32_t len) {
    if(len<=0) return NULL;
    buffer * buf;
    buf=(buffer *)malloc(sizeof(buffer));
    if(buf==NULL) {
        return NULL;
    }
    buf->buffer = (uint8_t *)malloc(len*sizeof(uint8_t));
    if(buf->buffer==NULL) {
        return NULL;
    }
    buf->head=0;
    buf->tail=1;
    buf->capacity=len;
    buf->num_items=0;
    buffer_clear(buf);
    return buf;
}

/*
 * Function: buffer_clear
 * ----------------------------
 *   Clears a circular buffer by setting the data to all zeros
 *
 *   buf: pointer to the buffer
 *
 *   returns: success or failure
 */
int8_t buffer_clear(buffer * buf) {
    if(!buf) return -1;
    uint32_t i;
    for(i=0;i<buf->capacity;i++)
        buf->buffer[i]=0;
    buf->num_items=0;
    return 0;
}

/*
 * Function: buffer_delete
 * ----------------------------
 *   Deletes the buffer from memory
 *
 *   buf: buffer to be deleted
 *
 *   returns: success or failure
 */
int8_t buffer_delete(buffer * buf) {
    if(!buf) return -1;
    free(buf);
    return 0;
}

/*
 * Function: buffer_isfull
 * ----------------------------
 *   Returns if the circular buffer is full or not
 *
 *   buf: pointer to the buffer
 *
 *   returns: -1 if null pointer, 0 if not full, 1 if full
 */
int8_t buffer_isfull(buffer * buf) {
    if(!buf) return -1;
    return(buf->num_items==buf->capacity);
}

/*
 * Function: buffer_isempty
 * ----------------------------
 *   Checks if the circular buffer is empty
 *
 *   buf: buffer to check
 *
 *   returns: -1 if null pointer, 0 is not empty, 1 if empty
 */
int8_t buffer_isempty(buffer * buf) {
    if(!buf) return -1;
    return (buf->head==buf->tail);
}

/*
 * Function: buffer_length
 * ----------------------------
 *   Returns the legnth of the buffer (how many items)
 *
 *   buf: pointer to buffer
 *
 *   returns: length of buffer
 */
int8_t buffer_length(buffer *buf) {
    if(!buf) return -1;
    return (buf->num_items);
}

/*
 * Function: buffer_add
 * ----------------------------
 *   Adds an item to the end of the buffer
 *
 *   buf: pointer to the buffer
 *   item: item to add to the buffer
 *
 *   returns: success or failure
 */
int8_t buffer_add(buffer * buf, uint8_t item) {
    if(!buf) return -1;

    if(buffer_isfull(buf)) {
        buf->tail=inc(buf->tail,buf->capacity);
        buf->head=inc(buf->head,buf->capacity);
        buf->buffer[buf->head]=item;
        return 1;
    }
    buf->head=inc(buf->head,buf->capacity);
    buf->buffer[buf->head]=item;
    buf->num_items++;
    if(buf->num_items>buf->capacity) buf->num_items=buf->capacity;
    return 0;
}

/*
 * Function: buffer_get
 * ----------------------------
 *   Retrieves the first item in the FIFO circular buffer
 *
 *   buf: pointer to the buffer
 *
 *   returns: item add the tail of the buffer
 */
uint8_t buffer_get(buffer * buf) {
    if(!buf) return NULL;
    uint8_t item;
    if(buf->num_items) {
        item = buf->buffer[buf->tail];
        //buf->buffer[buf->tail]=0;
        buf->tail=inc(buf->tail,buf->capacity);
        if(buf->num_items>0) buf->num_items--;
    }
    return item;
}

uint32_t inc(uint32_t var, uint32_t cap) {
    if(var==cap-1)
        return 0;
    return var+1;
}


