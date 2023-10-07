
#ifndef POWERSPI_INC_POWERSPI_H_
#define POWERSPI_INC_POWERSPI_H_

#include <stdint.h>

#define POWERSPI_HEADER_SIZE   3 // Tamaño del header de PowerSPI
#define POWERSPI_TX_RX_BUFFER_SIZE   256 // Tamaño del buffer de transmision y recepcion de datos (incluye los 3 bytes de header)

typedef void(* powerspi_callback_t)(uint8_t cmd, uint8_t * buffer, uint8_t * size);

typedef struct {
    void * port;
    void * pin;
} powerspi_gpio_t;

void PowerSPI_Init(uint8_t address, powerspi_callback_t callback, void * spi_handle, powerspi_gpio_t spi_miso, powerspi_gpio_t spi_mosi, powerspi_gpio_t spi_sck, powerspi_gpio_t spi_cs);
void PowerSPI_SetCallback(powerspi_callback_t cb);
void PowerSPI_RxCompleteISRHandler();
void PowerSPI_TxCompleteISRHandler();
void PowerSPI_CsFallingEdgeISRHandler();
void PowerSPI_CsRisingEdgeISRHandler();

#endif /* POWERSPI_INC_POWERSPI_H_ */
