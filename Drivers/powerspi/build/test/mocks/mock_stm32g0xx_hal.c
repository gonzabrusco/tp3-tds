#include <string.h>
#include "fff.h"
#include "mock_stm32g0xx_hal.h"

DEFINE_FAKE_VALUE_FUNC1(HAL_StatusTypeDef, HAL_SPI_Abort, SPI_HandleTypeDef*);
DEFINE_FAKE_VALUE_FUNC3(HAL_StatusTypeDef, HAL_SPI_Receive_IT, SPI_HandleTypeDef*, uint8_t*, uint16_t);
DEFINE_FAKE_VALUE_FUNC3(HAL_StatusTypeDef, HAL_SPI_Transmit_IT, SPI_HandleTypeDef*, uint8_t*, uint16_t);
DEFINE_FAKE_VALUE_FUNC1(HAL_StatusTypeDef, HAL_SPI_Init, SPI_HandleTypeDef*);
DEFINE_FAKE_VALUE_FUNC1(HAL_StatusTypeDef, HAL_SPI_DeInit, SPI_HandleTypeDef*);
DEFINE_FAKE_VALUE_FUNC3(HAL_StatusTypeDef, HAL_SPI_RegisterCallback, SPI_HandleTypeDef*, HAL_SPI_CallbackIDTypeDef, pSPI_CallbackTypeDef);
DEFINE_FAKE_VOID_FUNC0(__enable_irq);
DEFINE_FAKE_VOID_FUNC0(__disable_irq);
DEFINE_FAKE_VOID_FUNC2(HAL_GPIO_Init, GPIO_TypeDef*, GPIO_InitTypeDef*);
DEFINE_FAKE_VOID_FUNC2(HAL_GPIO_DeInit, GPIO_TypeDef*, uint32_t);

void mock_stm32g0xx_hal_Init(void)
{
    FFF_RESET_HISTORY();
    RESET_FAKE(HAL_SPI_Abort)
    RESET_FAKE(HAL_SPI_Receive_IT)
    RESET_FAKE(HAL_SPI_Transmit_IT)
    RESET_FAKE(HAL_SPI_Init)
    RESET_FAKE(HAL_SPI_DeInit)
    RESET_FAKE(HAL_SPI_RegisterCallback)
    RESET_FAKE(__enable_irq)
    RESET_FAKE(__disable_irq)
    RESET_FAKE(HAL_GPIO_Init)
    RESET_FAKE(HAL_GPIO_DeInit)
}
void mock_stm32g0xx_hal_Verify(void)
{
}
void mock_stm32g0xx_hal_Destroy(void)
{
}
