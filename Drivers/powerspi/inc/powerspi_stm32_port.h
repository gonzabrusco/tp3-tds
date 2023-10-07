
#ifndef POWERSPI_INC_POWERSPI_STM32_PORT_H_
#define POWERSPI_INC_POWERSPI_STM32_PORT_H_

#include <stdint.h>
#include "powerspi.h"

void PowerSPI_Port_Init(void * spi_handle, powerspi_gpio_t spi_miso, powerspi_gpio_t spi_mosi, powerspi_gpio_t spi_sck, powerspi_gpio_t spi_cs);
void PowerSPI_Port_EnableTX();
void PowerSPI_Port_DisableTX();
void PowerSPI_Port_Abort();
void PowerSPI_Port_EnableInterrupts();
void PowerSPI_Port_DisableInterrupts();
void PowerSPI_Port_Receive(uint8_t * buffer, uint16_t size);
void PowerSPI_Port_Transmit(uint8_t * buffer, uint16_t size);

#endif /* POWERSPI_INC_POWERSPI_STM32_PORT_H_ */
