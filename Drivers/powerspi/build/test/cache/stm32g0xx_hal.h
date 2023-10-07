







typedef enum

{

  HAL_OK = 0x00U,

  HAL_ERROR = 0x01U,

  HAL_BUSY = 0x02U,

  HAL_TIMEOUT = 0x03U

} HAL_StatusTypeDef;









typedef enum

{

  HAL_SPI_TX_COMPLETE_CB_ID = 0x00U,

  HAL_SPI_RX_COMPLETE_CB_ID = 0x01U,

  HAL_SPI_TX_RX_COMPLETE_CB_ID = 0x02U,

  HAL_SPI_TX_HALF_COMPLETE_CB_ID = 0x03U,

  HAL_SPI_RX_HALF_COMPLETE_CB_ID = 0x04U,

  HAL_SPI_TX_RX_HALF_COMPLETE_CB_ID = 0x05U,

  HAL_SPI_ERROR_CB_ID = 0x06U,

  HAL_SPI_ABORT_CB_ID = 0x07U,

  HAL_SPI_MSPINIT_CB_ID = 0x08U,

  HAL_SPI_MSPDEINIT_CB_ID = 0x09U



} HAL_SPI_CallbackIDTypeDef;









typedef struct

{

  volatile uint32_t MODER;

  volatile uint32_t OTYPER;

  volatile uint32_t OSPEEDR;

  volatile uint32_t PUPDR;

  volatile uint32_t IDR;

  volatile uint32_t ODR;

  volatile uint32_t BSRR;

  volatile uint32_t LCKR;

  volatile uint32_t AFR[2];

  volatile uint32_t BRR;

} GPIO_TypeDef;















typedef struct

{

  uint32_t Pin;





  uint32_t Mode;





  uint32_t Pull;





  uint32_t Speed;





  uint32_t Alternate;



} GPIO_InitTypeDef;









typedef struct

{

  volatile uint32_t CR1;

  volatile uint32_t CR2;

  volatile uint32_t SR;

  volatile uint32_t DR;

  volatile uint32_t CRCPR;

  volatile uint32_t RXCRCR;

  volatile uint32_t TXCRCR;

  volatile uint32_t I2SCFGR;

  volatile uint32_t I2SPR;

} SPI_TypeDef;









typedef struct

{

  uint32_t Mode;





  uint32_t Direction;





  uint32_t DataSize;





  uint32_t CLKPolarity;





  uint32_t CLKPhase;





  uint32_t NSS;







  uint32_t BaudRatePrescaler;











  uint32_t FirstBit;





  uint32_t TIMode;





  uint32_t CRCCalculation;





  uint32_t CRCPolynomial;





  uint32_t CRCLength;







  uint32_t NSSPMode;











} SPI_InitTypeDef;









typedef struct __SPI_HandleTypeDef

{

  SPI_TypeDef *Instance;



  SPI_InitTypeDef Init;



  uint8_t *pTxBuffPtr;



  uint16_t TxXferSize;



  volatile uint16_t TxXferCount;



  uint8_t *pRxBuffPtr;



  uint16_t RxXferSize;



  volatile uint16_t RxXferCount;



  uint32_t CRCSize;



  void (*RxISR)(struct __SPI_HandleTypeDef *hspi);



  void (*TxISR)(struct __SPI_HandleTypeDef *hspi);



  volatile uint32_t ErrorCode;





  void (* TxCpltCallback)(struct __SPI_HandleTypeDef *hspi);

  void (* RxCpltCallback)(struct __SPI_HandleTypeDef *hspi);

  void (* TxRxCpltCallback)(struct __SPI_HandleTypeDef *hspi);

  void (* TxHalfCpltCallback)(struct __SPI_HandleTypeDef *hspi);

  void (* RxHalfCpltCallback)(struct __SPI_HandleTypeDef *hspi);

  void (* TxRxHalfCpltCallback)(struct __SPI_HandleTypeDef *hspi);

  void (* ErrorCallback)(struct __SPI_HandleTypeDef *hspi);

  void (* AbortCpltCallback)(struct __SPI_HandleTypeDef *hspi);

  void (* MspInitCallback)(struct __SPI_HandleTypeDef *hspi);

  void (* MspDeInitCallback)(struct __SPI_HandleTypeDef *hspi);





} SPI_HandleTypeDef;









typedef void (*pSPI_CallbackTypeDef)(SPI_HandleTypeDef *hspi);

HAL_StatusTypeDef HAL_SPI_Abort(SPI_HandleTypeDef *hspi);

HAL_StatusTypeDef HAL_SPI_Receive_IT(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size);

HAL_StatusTypeDef HAL_SPI_Transmit_IT(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size);

HAL_StatusTypeDef HAL_SPI_Init(SPI_HandleTypeDef *hspi);















HAL_StatusTypeDef HAL_SPI_DeInit(SPI_HandleTypeDef *hspi);

HAL_StatusTypeDef HAL_SPI_RegisterCallback(SPI_HandleTypeDef *hspi, HAL_SPI_CallbackIDTypeDef CallbackID, pSPI_CallbackTypeDef pCallback);













void __enable_irq(void);













void __disable_irq(void);

void HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init);

void HAL_GPIO_DeInit(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin);
