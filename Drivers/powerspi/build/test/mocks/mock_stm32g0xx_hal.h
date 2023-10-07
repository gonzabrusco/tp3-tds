#ifndef mock_stm32g0xx_hal_H
#define mock_stm32g0xx_hal_H

#include "fff.h"
#include "fff_unity_helper.h"
#include "stm32g0xx_hal.h"


DECLARE_FAKE_VALUE_FUNC1(HAL_StatusTypeDef, HAL_SPI_Abort, SPI_HandleTypeDef*);
DECLARE_FAKE_VALUE_FUNC3(HAL_StatusTypeDef, HAL_SPI_Receive_IT, SPI_HandleTypeDef*, uint8_t*, uint16_t);
DECLARE_FAKE_VALUE_FUNC3(HAL_StatusTypeDef, HAL_SPI_Transmit_IT, SPI_HandleTypeDef*, uint8_t*, uint16_t);
DECLARE_FAKE_VALUE_FUNC1(HAL_StatusTypeDef, HAL_SPI_Init, SPI_HandleTypeDef*);
DECLARE_FAKE_VALUE_FUNC1(HAL_StatusTypeDef, HAL_SPI_DeInit, SPI_HandleTypeDef*);
DECLARE_FAKE_VALUE_FUNC3(HAL_StatusTypeDef, HAL_SPI_RegisterCallback, SPI_HandleTypeDef*, HAL_SPI_CallbackIDTypeDef, pSPI_CallbackTypeDef);
DECLARE_FAKE_VOID_FUNC0(__enable_irq);
DECLARE_FAKE_VOID_FUNC0(__disable_irq);
DECLARE_FAKE_VOID_FUNC2(HAL_GPIO_Init, GPIO_TypeDef*, GPIO_InitTypeDef*);
DECLARE_FAKE_VOID_FUNC2(HAL_GPIO_DeInit, GPIO_TypeDef*, uint32_t);

void mock_stm32g0xx_hal_Init(void);
void mock_stm32g0xx_hal_Verify(void);
void mock_stm32g0xx_hal_Destroy(void);

#endif // mock_stm32g0xx_hal_H
