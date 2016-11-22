#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// Constructors ////////////////////////////////////////////////////////////////

UARTClass::UARTClass( Uart* pUart, IRQn_Type dwIrq, uint32_t dwId, RingBuffer* pRx_buffer )
{
  _rx_buffer = pRx_buffer ;
  _pUart=pUart ;
  _dwIrq=dwIrq ;
  _dwId=dwId ;
}

// Public Methods //////////////////////////////////////////////////////////////
void UARTClass::begin( const uint32_t dwBaudRate )
{

  // Configure PMC
  pmc_enable_periph_clk( _dwId ) ;
  // Disable PDC channel
  _pUart->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS ;
  // Reset and disable receiver and transmitter
  _pUart->UART_CR = UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS ;
  // Configure mode
  _pUart->UART_MR = UART_MR_PAR_NO | UART_MR_CHMODE_NORMAL ;
  // Configure baudrate (asynchronous, no oversampling)
  _pUart->UART_BRGR = (SystemCoreClock / dwBaudRate) >> 4 ;
  // Configure interrupts
  _pUart->UART_IDR = 0xFFFFFFFF;
  _pUart->UART_IER = UART_IER_RXRDY | UART_IER_OVRE | UART_IER_FRAME;
  // Enable UART interrupt in NVIC
  NVIC_EnableIRQ(_dwIrq);
  // Enable receiver and transmitter
  _pUart->UART_CR = UART_CR_RXEN | UART_CR_TXEN ;

}
