/*****************************************************************************
 *
 *  C++ interface/implementation created by Martin Kojtal (0xc0170). Thanks to
 *  Jim Carver and Frank Vannieuwkerke for their inital cc3000 mbed port and
 *  provided help.
 *
 *  This version of "host driver" uses CC3000 Host Driver Implementation. Thus
 *  read the following copyright:
 *
 *  Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/
#include "cc3000.h"
#include "cc3000_spi.h"
#include "ch.hpp"
#include "hal.h"

namespace mbed_cc3000 {

  cc3000_spi::cc3000_spi(SPIDriver *spidrv, ioportid_t ssport, uint16_t sspad,
                         uint16_t cr1, EXTDriver *extdrv,
                         ioportid_t irqport, uint16_t irqpad,
                         ioportid_t enport, uint16_t enpad,
                         cc3000_event &event, cc3000_simple_link &simple_link)
  : _spidrv(spidrv), _irqport(irqport), _irqpad(irqpad), _enport(enport) ,
    _enpad(enpad), _extdrv(extdrv), _event(event),  _simple_link(simple_link) {

    _spicfg = {
               NULL,
               ssport,
               sspad,
               cr1};
  }

  cc3000_spi::~cc3000_spi() {

  }

  void cc3000_spi::assertCS(void){
    spiAcquireBus(_spidrv);             /* Acquire ownership of the bus.    */
    spiStart(_spidrv, &_spicfg);         /* Setup transfer parameters.       */
    spiSelect(_spidrv);                 /* Slave Select assertion.          */
  }


  void cc3000_spi::deassertCS(void){
    spiUnselect(_spidrv);                /* Slave Select de-assertion.       */
    spiReleaseBus(_spidrv);              /* Ownership release.               */
  }

  void cc3000_spi::wlan_irq_enable()
  {
    extChannelEnable(_extdrv, _irqpad);

    if(wlan_irq_read() == 0) {
      WLAN_IRQHandler();
    }
  }

  void cc3000_spi::wlan_irq_disable() {
    extChannelDisable(_extdrv, _irqpad);
  }

  uint32_t cc3000_spi::wlan_irq_read() {
    return palReadPad(_irqport, _irqpad);
  }

  void cc3000_spi::close() {
    wlan_irq_disable();
  }

  void cc3000_spi::open() {
    _spi_info.spi_state = eSPI_STATE_POWERUP;
    _spi_info.tx_packet_length = 0;
    _spi_info.rx_packet_length = 0;
    wlan_irq_enable();
  }

  uint32_t cc3000_spi::first_write(uint8_t *buffer, uint16_t length) {
    assertCS();
    chibios_rt::BaseThread::sleep(MS2ST(50));

    /* first 4 bytes of the data */
    write_synchronous(buffer, 4);
    chibios_rt::BaseThread::sleep(MS2ST(50));
    write_synchronous(buffer + 4, length - 4);
    _spi_info.spi_state = eSPI_STATE_IDLE;

    deassertCS();

    return 0;
  }


  uint32_t cc3000_spi::write(uint8_t *buffer, uint16_t length) {
    uint8_t pad = 0;
    // check the total length of the packet in order to figure out if padding is necessary
    if(!(length & 0x0001))
    {
      pad++;
    }
    buffer[0] = WRITE;
    buffer[1] = HI(length + pad);
    buffer[2] = LO(length + pad);
    buffer[3] = 0;
    buffer[4] = 0;

    length += (SPI_HEADER_SIZE + pad);

    // The magic number resides at the end of the TX/RX buffer (1 byte after the allocated size)
    // If the magic number is overwitten - buffer overrun occurred - we will be stuck here forever!
    uint8_t * transmit_buffer = _simple_link.get_transmit_buffer();
    if (transmit_buffer[CC3000_TX_BUFFER_SIZE - 1] != CC3000_BUFFER_MAGIC_NUMBER)
    {
      while (1);
    }

    if (_spi_info.spi_state == eSPI_STATE_POWERUP)
    {
      while (_spi_info.spi_state != eSPI_STATE_INITIALIZED);
    }

    if (_spi_info.spi_state == eSPI_STATE_INITIALIZED)
    {
      // TX/RX transaction over SPI after powerup: IRQ is low - send read buffer size command
      first_write(buffer, length);
    }
    else
    {
      // Prevent occurence of a race condition when 2 back to back packets are sent to the
      // device, so the state will move to IDLE and once again to not IDLE due to IRQ
      wlan_irq_disable();

      while (_spi_info.spi_state != eSPI_STATE_IDLE);

      _spi_info.spi_state = eSPI_STATE_WRITE_IRQ;
      //_spi_info.pTxPacket = buffer;
      _spi_info.tx_packet_length = length;

      // Assert the CS line and wait until the IRQ line is active, then initialize the write operation
      assertCS();

      wlan_irq_enable();
    }

    // Wait until the transaction ends
    while (_spi_info.spi_state != eSPI_STATE_IDLE);

    return 0;
  }

  void cc3000_spi::write_synchronous(uint8_t *data, uint16_t size) {
    spiSend(_spidrv,size,data);
  }

  void cc3000_spi::read_synchronous(uint8_t *data, uint16_t size) {
    spiReceive(_spidrv,size,data);
  }

  uint32_t cc3000_spi::read_data_cont() {
    long data_to_recv;
    unsigned char *evnt_buff, type;

    //determine the packet type
    evnt_buff = _simple_link.get_received_buffer();
    data_to_recv = 0;
    STREAM_TO_UINT8((uint8_t *)(evnt_buff + SPI_HEADER_SIZE), HCI_PACKET_TYPE_OFFSET, type);

    switch(type)
    {
    case HCI_TYPE_DATA:
    {
      // Read the remaining data..
      STREAM_TO_UINT16((uint8_t *)(evnt_buff + SPI_HEADER_SIZE), HCI_DATA_LENGTH_OFFSET, data_to_recv);
      if (!((HEADERS_SIZE_EVNT + data_to_recv) & 1))
      {
        data_to_recv++;
      }

      if (data_to_recv)
      {
        read_synchronous(evnt_buff + 10, data_to_recv);
      }
      break;
    }
    case HCI_TYPE_EVNT:
    {
      // Calculate the rest length of the data
      STREAM_TO_UINT8((char *)(evnt_buff + SPI_HEADER_SIZE), HCI_EVENT_LENGTH_OFFSET, data_to_recv);
      data_to_recv -= 1;
      // Add padding byte if needed
      if ((HEADERS_SIZE_EVNT + data_to_recv) & 1)
      {
        data_to_recv++;
      }

      if (data_to_recv)
      {
        read_synchronous(evnt_buff + 10, data_to_recv);
      }

      _spi_info.spi_state = eSPI_STATE_READ_EOT;
      break;
    }
    }
    return (0);
  }

  void cc3000_spi::set_wlan_en(uint8_t value) {
    if (value) {
      palSetPad(_enport, _enpad);
    } else {
      palClearPad(_enport, _enpad);
    }
  }


  void cc3000_spi::WLAN_IRQHandler() {
    if (_spi_info.spi_state == eSPI_STATE_POWERUP)
    {
      // Inform HCI Layer that IRQ occured after powerup
      _spi_info.spi_state = eSPI_STATE_INITIALIZED;
    }
    else if (_spi_info.spi_state == eSPI_STATE_IDLE)
    {
      _spi_info.spi_state = eSPI_STATE_READ_IRQ;
      /* IRQ line goes low - acknowledge it */
      assertCS();
      read_synchronous(_simple_link.get_received_buffer(), 10);
      _spi_info.spi_state = eSPI_STATE_READ_EOT;


      // The header was read - continue with the payload read
      if (!read_data_cont())
      {
        // All the data was read - finalize handling by switching to the task
        // Trigger Rx processing
        wlan_irq_disable();
        deassertCS();
        // The magic number resides at the end of the TX/RX buffer (1 byte after the allocated size)
        // If the magic number is overwitten - buffer overrun occurred - we will be stuck here forever!
        uint8_t *received_buffer = _simple_link.get_received_buffer();
        if (received_buffer[CC3000_RX_BUFFER_SIZE - 1] != CC3000_BUFFER_MAGIC_NUMBER)
        {
          while (1);
        }
        _spi_info.spi_state = eSPI_STATE_IDLE;
        _event.received_handler(received_buffer + SPI_HEADER_SIZE);
      }
    }
    else if (_spi_info.spi_state == eSPI_STATE_WRITE_IRQ)
    {
      write_synchronous(_simple_link.get_transmit_buffer(), _spi_info.tx_packet_length);
      _spi_info.spi_state = eSPI_STATE_IDLE;
      deassertCS();
    }
  }

}
