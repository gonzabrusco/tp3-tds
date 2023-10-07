
#include "powerspi.h"
#include "powerspi_stm32_port.h"
#include <stddef.h>



// Ubicacion en el buffers del addres, comando y tamaño.
#define ADDRESS_INDEX   0
#define COMMAND_INDEX   1
#define SIZE_INDEX      2

static struct {
    uint8_t address;
    powerspi_callback_t callback;
} config = {0, NULL};

static enum {WAITING_FOR_CS, RECEIVING_ADDRESS_COMMAND_AND_SIZE, RECEIVING_REQUEST, SENDING_ADDRESS_COMMAND_SIZE_AND_RESPONSE} _state = WAITING_FOR_CS;
static uint8_t _tx_rx_buffer[POWERSPI_TX_RX_BUFFER_SIZE];

/**
  * @brief Inicializa PowerSPI
  *
  * @param address direccion de PowerSPI
  * @param callback funcion que es llamada cuando se recibe un mensaje
  * @param spi_handle handle del SPI
  * @param spi_miso Pin MISO del SPI
  * @param spi_mosi Pin MOSI del SPI
  * @param spi_sck Pin SCK del SPI
  * @param spi_cs Pin CS del SPI
  * 
  * @retval None
  */
void PowerSPI_Init(uint8_t address, powerspi_callback_t callback, void * spi_handle, powerspi_gpio_t spi_miso, powerspi_gpio_t spi_mosi, powerspi_gpio_t spi_sck, powerspi_gpio_t spi_cs) {
    if(callback == NULL) return;

    PowerSPI_Port_Init(spi_handle, spi_miso, spi_mosi, spi_sck, spi_cs); // Inicializamos el port del driver

    PowerSPI_Port_DisableInterrupts(); // Desactivo interrupciones

    config.address = address;
    config.callback = callback;

    // Inicializo la MEF
    PowerSPI_Port_Abort();
    PowerSPI_Port_DisableTX();
    _state = WAITING_FOR_CS;

    PowerSPI_Port_EnableInterrupts(); // Activo interrupciones
}

/**
  * @brief  Funcion que setea el callback que es llamado cuando se reciben datos por PowerSPI
  *
  * @retval None
  */
void PowerSPI_SetCallback(powerspi_callback_t cb) {
    config.callback = cb;
}

/**
  * @brief  Funcion que debe ser llamada desde el handler de interrupcion de recepcion completa por SPI
  *
  * @retval None
  */
void PowerSPI_RxCompleteISRHandler() {
    if(config.callback == NULL) return;

    switch(_state) {
    case RECEIVING_ADDRESS_COMMAND_AND_SIZE:
        // Recibimos 3 bytes de address, command y size.
        if(config.address == _tx_rx_buffer[ADDRESS_INDEX]) {
            // El mensaje es para nosotros.
            if(0 == _tx_rx_buffer[SIZE_INDEX]) {
                // No nos envian datos, unicamente nos piden.
                // Contestamos con los datos solicitados.
                config.callback(_tx_rx_buffer[COMMAND_INDEX], _tx_rx_buffer + SIZE_INDEX + 1, _tx_rx_buffer + SIZE_INDEX);
                PowerSPI_Port_EnableTX();
                PowerSPI_Port_Transmit(_tx_rx_buffer, _tx_rx_buffer[SIZE_INDEX] + SIZE_INDEX + 1);
                _state = SENDING_ADDRESS_COMMAND_SIZE_AND_RESPONSE;
            }
            else {
                // Nos envian datos, los tenemos que recibir antes de poder contestar.
                PowerSPI_Port_Receive(_tx_rx_buffer + SIZE_INDEX + 1, _tx_rx_buffer[SIZE_INDEX]);
                _state = RECEIVING_REQUEST;
            }
        }
        else {
            // El mensaje no es para nosotros.
            _state = WAITING_FOR_CS;
        }
        break;
    case RECEIVING_REQUEST:
        // Terminamos de recibir el request. Ahora procesamos los datos enviados y contestamos.
        config.callback(_tx_rx_buffer[COMMAND_INDEX], _tx_rx_buffer + SIZE_INDEX + 1, _tx_rx_buffer + SIZE_INDEX);
        PowerSPI_Port_EnableTX();
        PowerSPI_Port_Transmit(_tx_rx_buffer, _tx_rx_buffer[SIZE_INDEX] + SIZE_INDEX + 1);
        _state = SENDING_ADDRESS_COMMAND_SIZE_AND_RESPONSE;
        break;
    default:
        // No deberia haber llegado nunca aca.
        // Por las dudas reinicializo la MEF
        PowerSPI_Port_Abort();
        PowerSPI_Port_DisableTX();
        _state = WAITING_FOR_CS;
        break;
    }
}

/**
  * @brief  Funcion que debe ser llamada desde el handler de interrupcion de transmision completa por SPI
  *
  * @retval None
  */
void PowerSPI_TxCompleteISRHandler() {
    if(config.callback == NULL) return;

    if(_state == SENDING_ADDRESS_COMMAND_SIZE_AND_RESPONSE) {
        // Se termino el envio de PowerSPI. Reiniciamos la MEF.
        PowerSPI_Port_DisableTX();
        _state = WAITING_FOR_CS;
    }
    else {
        // No deberia haber llegado nunca aca.
        // Por las dudas abortamos cualquier comunicacion SPI y reinicializo la MEF
        PowerSPI_Port_Abort();
        PowerSPI_Port_DisableTX();
        _state = WAITING_FOR_CS;
    }
}

/**
  * @brief  Funcion que debe ser llamada desde el handler de interrupcion de flanco negativo en el pin CS
  *
  * @retval None
  */
void PowerSPI_CsFallingEdgeISRHandler() {
    if(config.callback != NULL && _state == WAITING_FOR_CS) {
        // PowerSPI esta listo para recibir el address, comando y length (3 bytes)
        PowerSPI_Port_Receive(_tx_rx_buffer, 3);
        _state = RECEIVING_ADDRESS_COMMAND_AND_SIZE;
    }
}

/**
  * @brief  Funcion que debe ser llamada desde el handler de interrupcion de flanco positivo en el pin CS
  *
  * @retval None
  */
void PowerSPI_CsRisingEdgeISRHandler() {
    if(_state != WAITING_FOR_CS) {
        // PowerSPI estaba transmitiendo o recibiendo, aborto cualquier transmision.
        // Reinicializo la MEF
        PowerSPI_Port_Abort();
        PowerSPI_Port_DisableTX();
        _state = WAITING_FOR_CS;
    }
}


