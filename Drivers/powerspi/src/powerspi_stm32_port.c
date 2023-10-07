
#include "powerspi_stm32_port.h"
#include "stm32g0xx_hal.h"

// Tipos de datos internos
typedef struct {
    GPIO_TypeDef * port;
    uint16_t pin;
} powerspi_port_gpio_t;

// Variables internas
static struct {
    SPI_HandleTypeDef * spi_handle;
    powerspi_port_gpio_t spi_miso;
    powerspi_port_gpio_t spi_mosi;
    powerspi_port_gpio_t spi_sck;
    powerspi_port_gpio_t spi_cs;
} config;

// Funciones internas
// XXX: Este port necesita poder registrar callbacks especiales para inicializar y desinicializar.
// Por eso el define USE_HAL_SPI_REGISTER_CALLBACKS debe estar activado (desde el CubeMX en Project Manager->Advanced Settings->Register Callbacks->SPI (Enable))
void PowerSPI_Port_MspInit_MISO_Active(SPI_HandleTypeDef* spiHandle);
void PowerSPI_Port_MspInit_MISO_Inactive(SPI_HandleTypeDef* spiHandle);
void PowerSPI_Port_MspDeInit(SPI_HandleTypeDef* spiHandle);

/**
 * @brief Inicializa PowerSPI.
 *
 * @param spi_handle handle del SPI
 * @param spi_cs_port puerto del CS de SPI
 * @param spi_cs_pin pin del CS de SPI
 *
 * @retval None
 */

void PowerSPI_Port_Init(void * spi_handle, powerspi_gpio_t spi_miso, powerspi_gpio_t spi_mosi, powerspi_gpio_t spi_sck, powerspi_gpio_t spi_cs) {
    // Se asume que SPI y el GPIO ya fueron inicializados por el CubeMX.
    // En otras plataformas, aqui deberia ir la inicializacion de los perifericos

    // Guardamos la informacion de configuracion en una estructura interna.
    config.spi_handle = (SPI_HandleTypeDef *)spi_handle;
    config.spi_miso.port = (GPIO_TypeDef *)spi_miso.port;
    config.spi_miso.pin = (uint16_t)(uint32_t)spi_miso.pin;
    config.spi_mosi.port = (GPIO_TypeDef *)spi_mosi.port;
    config.spi_mosi.pin = (uint16_t)(uint32_t)spi_mosi.pin;
    config.spi_sck.port = (GPIO_TypeDef *)spi_sck.port;
    config.spi_sck.pin = (uint16_t)(uint32_t)spi_sck.pin;
    config.spi_cs.port = (GPIO_TypeDef *)spi_cs.port;
    config.spi_cs.pin = (uint16_t)(uint32_t)spi_cs.pin;
}

/**
 * @brief Aborta cualquier operacion que este haciendo SPI
 *
 * @retval None
 */
void PowerSPI_Port_Abort() {
    HAL_SPI_Abort(config.spi_handle);
}

/**
 * @brief Activa interrupciones de SPI y GPIO
 *
 * @retval None
 */
void PowerSPI_Port_EnableInterrupts() {
    __enable_irq(); // CMSIS: Activa todas las interrupciones de manera global.
}

/**
 * @brief Inicia la recepcion de datos por SPI
 *
 * @retval None
 */
void PowerSPI_Port_DisableInterrupts() {
    __disable_irq(); // CMSIS: Desactiva todas las interrupciones de manera global.
}

/**
 * @brief Funcion que inicializa la recepcion de datos por SPI
 *
 * @retval None
 */
void PowerSPI_Port_Receive(uint8_t * buffer, uint16_t size) {
    HAL_SPI_Receive_IT(config.spi_handle, buffer, size);
}

/**
 * @brief Funcion que inicializa la transmision de datos por SPI
 *
 * @retval None
 */
void PowerSPI_Port_Transmit(uint8_t * buffer, uint16_t size) {
    HAL_SPI_Transmit_IT(config.spi_handle, buffer, size);
}

/**
 * @brief Activa el pin de TX en SPI
 *
 * @retval None
 */
void PowerSPI_Port_EnableTX() {
    HAL_SPI_RegisterCallback(config.spi_handle, HAL_SPI_MSPDEINIT_CB_ID, PowerSPI_Port_MspDeInit);
    HAL_SPI_DeInit(config.spi_handle);
    config.spi_handle->Init.Direction = SPI_DIRECTION_2LINES;
    HAL_SPI_RegisterCallback(config.spi_handle, HAL_SPI_MSPINIT_CB_ID, PowerSPI_Port_MspInit_MISO_Active);
    HAL_SPI_Init(config.spi_handle);
}

/**
 * @brief Desactiva el pin de TX en SPI y lo deja en alta impedancia
 *
 * @retval None
 */
void PowerSPI_Port_DisableTX() {
    HAL_SPI_RegisterCallback(config.spi_handle, HAL_SPI_MSPDEINIT_CB_ID, PowerSPI_Port_MspDeInit);
    HAL_SPI_DeInit(config.spi_handle);
    config.spi_handle->Init.Direction = SPI_DIRECTION_2LINES_RXONLY;
    HAL_SPI_RegisterCallback(config.spi_handle, HAL_SPI_MSPINIT_CB_ID, PowerSPI_Port_MspInit_MISO_Inactive);
    HAL_SPI_Init(config.spi_handle);
}

/**
 * @brief Reimplementacion del HAL_SPI_MspInit() de la HAL de STM32 para que active el MISO
 *
 * @warning Debido a que este funcion esta muy fuertemente ligada a como se configuro el SPI, se debe adaptar si se modifica las configuraciones de pines.
 * Se podría adaptar dinamicamente pero seria mas costoso computacionalmente y se desea priorizar la velocidad de ejecucion.
 *
 * @retval None
 */
void PowerSPI_Port_MspInit_MISO_Active(SPI_HandleTypeDef* spiHandle) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(spiHandle == config.spi_handle)
  {
    /* SPI1 clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE(); // Adaptar este llamado si cambia el SPI

    __HAL_RCC_GPIOB_CLK_ENABLE(); // Adaptar este llamado si cambia el PORT

    // Configuramos para SPI el MISO, MOSI y SCK
    GPIO_InitStruct = (GPIO_InitTypeDef){0};
    GPIO_InitStruct.Pin = config.spi_sck.pin | config.spi_mosi.pin | config.spi_sck.pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
    HAL_GPIO_Init(config.spi_sck.port, &GPIO_InitStruct); // Esto funciona porque mosi y sck estan el mismo port.
  }
}

/**
 * @brief Reimplementacion del HAL_SPI_MspInit() de la HAL de STM32 para que deje MISO en alta impedancia
 *
 * @warning Debido a que este funcion esta muy fuertemente ligada a como se configuro el SPI, se debe adaptar si se modifica las configuraciones de pines.
 * Se podría adaptar dinamicamente pero seria mas costoso computacionalmente y se desea priorizar la velocidad de ejecucion.
 *
 * @retval None
 */
void PowerSPI_Port_MspInit_MISO_Inactive(SPI_HandleTypeDef* spiHandle) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(spiHandle == config.spi_handle) {
        /* SPI clock enable */
        __HAL_RCC_SPI1_CLK_ENABLE(); // Adaptar este llamado si cambia el SPI

        __HAL_RCC_GPIOB_CLK_ENABLE(); // Adaptar este llamado si cambia el PORT

        // Configuramos el pin correspondiente al MISO en alta impedancia (como input)
        GPIO_InitStruct = (GPIO_InitTypeDef){0};
        GPIO_InitStruct.Pin = config.spi_miso.pin;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(config.spi_miso.port, &GPIO_InitStruct);

        // Configuramos para SPI el MOSI
        GPIO_InitStruct = (GPIO_InitTypeDef){0};
        GPIO_InitStruct.Pin = config.spi_mosi.pin | config.spi_sck.pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
        HAL_GPIO_Init(config.spi_sck.port, &GPIO_InitStruct); // Esto funciona porque mosi y sck estan el mismo port.
    }
}

/**
 * @brief Reimplementacion del HAL_SPI_MspDeInit() de la HAL de STM32. Se utiliza la misma para ambos casos (TX activo o inactivo)
 *
 * @warning Debido a que este funcion esta muy fuertemente ligada a como se configuro el SPI, se debe adaptar si se modifica las configuraciones de pines.
 * Se podría adaptar dinamicamente pero seria mas costoso computacionalmente y se desea priorizar la velocidad de ejecucion.
 *
 * @retval None
 */
void PowerSPI_Port_MspDeInit(SPI_HandleTypeDef* spiHandle) {
  if(spiHandle == config.spi_handle) {
    __HAL_RCC_SPI1_CLK_DISABLE(); // Adaptar este llamado si cambia el SPI

    HAL_GPIO_DeInit(config.spi_sck.port, config.spi_sck.pin | config.spi_mosi.pin | config.spi_sck.pin);
  }
}


