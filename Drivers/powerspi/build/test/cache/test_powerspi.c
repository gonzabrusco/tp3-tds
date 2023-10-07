#include "inc/powerspi_stm32_port.h"
#include "inc/powerspi.h"
#include "build/test/mocks/mock_stm32g0xx_hal.h"
#include "/var/lib/gems/3.0.0/gems/ceedling-0.31.1/vendor/unity/src/unity.h"
static SPI_HandleTypeDef spi_handle;

static powerspi_gpio_t spi_miso = {0, 0};

static powerspi_gpio_t spi_mosi = {0, 0};

static powerspi_gpio_t spi_sck = {0, 0};

static powerspi_gpio_t spi_cs = {0, 0};











HAL_StatusTypeDef HAL_SPI_Receive_IT_aux_receive_correct_address_and_no_request_data(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size)

{

    HAL_StatusTypeDef errorcode = HAL_OK;



    (void)hspi;

    (void)Size;



    pData[0] = 5;

    pData[1] = 2;

    pData[2] = 0;



    return errorcode;

}





HAL_StatusTypeDef HAL_SPI_Receive_IT_aux_receive_correct_address_and_request_data(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size)

{

    HAL_StatusTypeDef errorcode = HAL_OK;









    (void)hspi;



    if(Size == 3) {

        pData[0] = 5;

        pData[1] = 2;

        pData[2] = 20;

    }

    else if(Size == 20) {

        for(int i = 0; i < 20; i++) {

            pData[i] = i;

        }

    }

    else {

        UnityFail( (("En este test no deberias pedir solo 3 bytes o 20 bytes...")), (UNITY_UINT)(85));

    }



    return errorcode;

}





HAL_StatusTypeDef HAL_SPI_Receive_IT_aux_receive_other_address(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size)

{

    HAL_StatusTypeDef errorcode = HAL_OK;



    (void)hspi;

    (void)Size;



    pData[0] = 5 +1;

    pData[1] = 2;

    pData[2] = 0;



    return errorcode;

}





HAL_StatusTypeDef HAL_SPI_Init_aux(SPI_HandleTypeDef *hspi)

{

    HAL_StatusTypeDef errorcode = HAL_OK;





    hspi->MspInitCallback(hspi);



    return errorcode;

}





HAL_StatusTypeDef HAL_SPI_DeInit_aux(SPI_HandleTypeDef *hspi)

{

    HAL_StatusTypeDef errorcode = HAL_OK;





    hspi->MspDeInitCallback(hspi);



    return errorcode;

}





HAL_StatusTypeDef HAL_SPI_RegisterCallback_aux(SPI_HandleTypeDef *hspi, HAL_SPI_CallbackIDTypeDef CallbackID, pSPI_CallbackTypeDef pCallback)

{

    HAL_StatusTypeDef status = HAL_OK;



    switch (CallbackID)

    {

        case HAL_SPI_TX_COMPLETE_CB_ID :

        hspi->TxCpltCallback = pCallback;

        break;



        case HAL_SPI_RX_COMPLETE_CB_ID :

        hspi->RxCpltCallback = pCallback;

        break;



        case HAL_SPI_TX_RX_COMPLETE_CB_ID :

        hspi->TxRxCpltCallback = pCallback;

        break;



        case HAL_SPI_TX_HALF_COMPLETE_CB_ID :

        hspi->TxHalfCpltCallback = pCallback;

        break;



        case HAL_SPI_RX_HALF_COMPLETE_CB_ID :

        hspi->RxHalfCpltCallback = pCallback;

        break;



        case HAL_SPI_TX_RX_HALF_COMPLETE_CB_ID :

        hspi->TxRxHalfCpltCallback = pCallback;

        break;



        case HAL_SPI_ERROR_CB_ID :

        hspi->ErrorCallback = pCallback;

        break;



        case HAL_SPI_ABORT_CB_ID :

        hspi->AbortCpltCallback = pCallback;

        break;



        case HAL_SPI_MSPINIT_CB_ID :

        hspi->MspInitCallback = pCallback;

        break;



        case HAL_SPI_MSPDEINIT_CB_ID :

        hspi->MspDeInitCallback = pCallback;

        break;



        default :



        status = HAL_ERROR;

        break;

    }



    return status;

}





void callback(uint8_t cmd, uint8_t * buffer, uint8_t * size) {

    if(cmd == 2) {



        buffer[0] = 'H';

        buffer[1] = 'O';

        buffer[2] = 'L';

        buffer[3] = 'A';

        *size = 4;

    }

    else {



        *size = 0;

    }

}









void suiteSetUp() {



    PowerSPI_Init(5, callback, &spi_handle, spi_miso, spi_mosi, spi_sck, spi_cs);

}



void setUp() {

    fff.call_history_idx = 0; memset(fff.call_history, 0, sizeof(fff.call_history));;





    { HAL_SPI_Receive_IT_reset(); };





    HAL_SPI_Init_fake.custom_fake = HAL_SPI_Init_aux;

    HAL_SPI_DeInit_fake.custom_fake = HAL_SPI_DeInit_aux;

    HAL_SPI_RegisterCallback_fake.custom_fake = HAL_SPI_RegisterCallback_aux;

}



void tearDown() {



    PowerSPI_CsRisingEdgeISRHandler();

}





void test_se_detecta_flanco_descendente_de_cs_y_comienza_a_escuchar(void) {



    PowerSPI_CsFallingEdgeISRHandler();











    UnityAssertEqualNumber((UNITY_INT)((1)), (UNITY_INT)((HAL_SPI_Receive_IT_fake.call_count)), (

   ((void *)0)

   ), (UNITY_UINT)(234), UNITY_DISPLAY_STYLE_INT);



    UnityAssertEqualNumber((UNITY_INT)((3)), (UNITY_INT)((HAL_SPI_Receive_IT_fake.arg2_history[0])), (

   ((void *)0)

   ), (UNITY_UINT)(236), UNITY_DISPLAY_STYLE_INT);



    do {if ((

   ((void *)0) 

   != HAL_SPI_Receive_IT_fake.arg1_history[0])) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(238)));}} while(0);



    do {if ((&spi_handle == HAL_SPI_Receive_IT_fake.arg0_history[0])) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(240)));}} while(0);

}





void test_se_recibe_un_mensaje_sin_payload_y_contestamos_directamente(void) {





    HAL_SPI_Receive_IT_fake.custom_fake = HAL_SPI_Receive_IT_aux_receive_correct_address_and_no_request_data;





    PowerSPI_CsFallingEdgeISRHandler();





    PowerSPI_RxCompleteISRHandler();





    PowerSPI_TxCompleteISRHandler();







    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_SPI_Receive_IT)), (UNITY_INT64)((fff.call_history[0])), (("Function " "HAL_SPI_Receive_IT" " not called in order " "0")), (UNITY_UINT)(260), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT)((3)), (UNITY_INT)((HAL_SPI_Receive_IT_fake.arg2_history[0])), (

   ((void *)0)

   ), (UNITY_UINT)(261), UNITY_DISPLAY_STYLE_INT);

    do {if ((

   ((void *)0) 

   != HAL_SPI_Receive_IT_fake.arg1_history[0])) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(262)));}} while(0);

    do {if ((&spi_handle == HAL_SPI_Receive_IT_fake.arg0_history[0])) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(263)));}} while(0);



    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_SPI_RegisterCallback)), (UNITY_INT64)((fff.call_history[1])), (("Function " "HAL_SPI_RegisterCallback" " not called in order " "1")), (UNITY_UINT)(265), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_SPI_DeInit)), (UNITY_INT64)((fff.call_history[2])), (("Function " "HAL_SPI_DeInit" " not called in order " "2")), (UNITY_UINT)(266), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_GPIO_DeInit)), (UNITY_INT64)((fff.call_history[3])), (("Function " "HAL_GPIO_DeInit" " not called in order " "3")), (UNITY_UINT)(267), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_SPI_RegisterCallback)), (UNITY_INT64)((fff.call_history[4])), (("Function " "HAL_SPI_RegisterCallback" " not called in order " "4")), (UNITY_UINT)(268), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_SPI_Init)), (UNITY_INT64)((fff.call_history[5])), (("Function " "HAL_SPI_Init" " not called in order " "5")), (UNITY_UINT)(269), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_GPIO_Init)), (UNITY_INT64)((fff.call_history[6])), (("Function " "HAL_GPIO_Init" " not called in order " "6")), (UNITY_UINT)(270), UNITY_DISPLAY_STYLE_HEX64);



    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_SPI_Transmit_IT)), (UNITY_INT64)((fff.call_history[7])), (("Function " "HAL_SPI_Transmit_IT" " not called in order " "7")), (UNITY_UINT)(272), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT)((3+4)), (UNITY_INT)((HAL_SPI_Transmit_IT_fake.arg2_history[0])), (

   ((void *)0)

   ), (UNITY_UINT)(273), UNITY_DISPLAY_STYLE_INT);

    do {if ((

   ((void *)0) 

   != HAL_SPI_Transmit_IT_fake.arg1_history[0])) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(274)));}} while(0);

    do {if ((&spi_handle == HAL_SPI_Transmit_IT_fake.arg0_history[0])) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(275)));}} while(0);



    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_SPI_RegisterCallback)), (UNITY_INT64)((fff.call_history[8])), (("Function " "HAL_SPI_RegisterCallback" " not called in order " "8")), (UNITY_UINT)(277), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_SPI_DeInit)), (UNITY_INT64)((fff.call_history[9])), (("Function " "HAL_SPI_DeInit" " not called in order " "9")), (UNITY_UINT)(278), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_GPIO_DeInit)), (UNITY_INT64)((fff.call_history[10])), (("Function " "HAL_GPIO_DeInit" " not called in order " "10")), (UNITY_UINT)(279), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_SPI_RegisterCallback)), (UNITY_INT64)((fff.call_history[11])), (("Function " "HAL_SPI_RegisterCallback" " not called in order " "11")), (UNITY_UINT)(280), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_SPI_Init)), (UNITY_INT64)((fff.call_history[12])), (("Function " "HAL_SPI_Init" " not called in order " "12")), (UNITY_UINT)(281), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_GPIO_Init)), (UNITY_INT64)((fff.call_history[13])), (("Function " "HAL_GPIO_Init" " not called in order " "13")), (UNITY_UINT)(282), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_GPIO_Init)), (UNITY_INT64)((fff.call_history[14])), (("Function " "HAL_GPIO_Init" " not called in order " "14")), (UNITY_UINT)(283), UNITY_DISPLAY_STYLE_HEX64);

}





void test_se_recibe_un_mensaje_con_payload_y_contestamos(void) {





    HAL_SPI_Receive_IT_fake.custom_fake = HAL_SPI_Receive_IT_aux_receive_correct_address_and_request_data;





    PowerSPI_CsFallingEdgeISRHandler();





    PowerSPI_RxCompleteISRHandler();





    PowerSPI_RxCompleteISRHandler();





    PowerSPI_TxCompleteISRHandler();







    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_SPI_Receive_IT)), (UNITY_INT64)((fff.call_history[0])), (("Function " "HAL_SPI_Receive_IT" " not called in order " "0")), (UNITY_UINT)(306), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT)((3)), (UNITY_INT)((HAL_SPI_Receive_IT_fake.arg2_history[0])), (

   ((void *)0)

   ), (UNITY_UINT)(307), UNITY_DISPLAY_STYLE_INT);

    do {if ((

   ((void *)0) 

   != HAL_SPI_Receive_IT_fake.arg1_history[0])) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(308)));}} while(0);

    do {if ((&spi_handle == HAL_SPI_Receive_IT_fake.arg0_history[0])) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(309)));}} while(0);



    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_SPI_Receive_IT)), (UNITY_INT64)((fff.call_history[1])), (("Function " "HAL_SPI_Receive_IT" " not called in order " "1")), (UNITY_UINT)(311), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT)((20)), (UNITY_INT)((HAL_SPI_Receive_IT_fake.arg2_history[1])), (

   ((void *)0)

   ), (UNITY_UINT)(312), UNITY_DISPLAY_STYLE_INT);

    do {if ((

   ((void *)0) 

   != HAL_SPI_Receive_IT_fake.arg1_history[1])) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(313)));}} while(0);

    do {if ((&spi_handle == HAL_SPI_Receive_IT_fake.arg0_history[1])) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(314)));}} while(0);



    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_SPI_RegisterCallback)), (UNITY_INT64)((fff.call_history[2])), (("Function " "HAL_SPI_RegisterCallback" " not called in order " "2")), (UNITY_UINT)(316), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_SPI_DeInit)), (UNITY_INT64)((fff.call_history[3])), (("Function " "HAL_SPI_DeInit" " not called in order " "3")), (UNITY_UINT)(317), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_GPIO_DeInit)), (UNITY_INT64)((fff.call_history[4])), (("Function " "HAL_GPIO_DeInit" " not called in order " "4")), (UNITY_UINT)(318), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_SPI_RegisterCallback)), (UNITY_INT64)((fff.call_history[5])), (("Function " "HAL_SPI_RegisterCallback" " not called in order " "5")), (UNITY_UINT)(319), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_SPI_Init)), (UNITY_INT64)((fff.call_history[6])), (("Function " "HAL_SPI_Init" " not called in order " "6")), (UNITY_UINT)(320), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_GPIO_Init)), (UNITY_INT64)((fff.call_history[7])), (("Function " "HAL_GPIO_Init" " not called in order " "7")), (UNITY_UINT)(321), UNITY_DISPLAY_STYLE_HEX64);



    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_SPI_Transmit_IT)), (UNITY_INT64)((fff.call_history[8])), (("Function " "HAL_SPI_Transmit_IT" " not called in order " "8")), (UNITY_UINT)(323), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT)((3+4)), (UNITY_INT)((HAL_SPI_Transmit_IT_fake.arg2_history[0])), (

   ((void *)0)

   ), (UNITY_UINT)(324), UNITY_DISPLAY_STYLE_INT);

    do {if ((

   ((void *)0) 

   != HAL_SPI_Transmit_IT_fake.arg1_history[0])) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(325)));}} while(0);

    do {if ((&spi_handle == HAL_SPI_Transmit_IT_fake.arg0_history[0])) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(326)));}} while(0);



    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_SPI_RegisterCallback)), (UNITY_INT64)((fff.call_history[9])), (("Function " "HAL_SPI_RegisterCallback" " not called in order " "9")), (UNITY_UINT)(328), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_SPI_DeInit)), (UNITY_INT64)((fff.call_history[10])), (("Function " "HAL_SPI_DeInit" " not called in order " "10")), (UNITY_UINT)(329), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_GPIO_DeInit)), (UNITY_INT64)((fff.call_history[11])), (("Function " "HAL_GPIO_DeInit" " not called in order " "11")), (UNITY_UINT)(330), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_SPI_RegisterCallback)), (UNITY_INT64)((fff.call_history[12])), (("Function " "HAL_SPI_RegisterCallback" " not called in order " "12")), (UNITY_UINT)(331), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_SPI_Init)), (UNITY_INT64)((fff.call_history[13])), (("Function " "HAL_SPI_Init" " not called in order " "13")), (UNITY_UINT)(332), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_GPIO_Init)), (UNITY_INT64)((fff.call_history[14])), (("Function " "HAL_GPIO_Init" " not called in order " "14")), (UNITY_UINT)(333), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_GPIO_Init)), (UNITY_INT64)((fff.call_history[15])), (("Function " "HAL_GPIO_Init" " not called in order " "15")), (UNITY_UINT)(334), UNITY_DISPLAY_STYLE_HEX64);

}





void test_se_recibe_un_mensaje_dirigido_a_otro_dispositivo(void) {





    HAL_SPI_Receive_IT_fake.custom_fake = HAL_SPI_Receive_IT_aux_receive_other_address;





    PowerSPI_CsFallingEdgeISRHandler();





    PowerSPI_RxCompleteISRHandler();







    UnityAssertEqualNumber((UNITY_INT)((1)), (UNITY_INT)((HAL_SPI_Receive_IT_fake.call_count)), (

   ((void *)0)

   ), (UNITY_UINT)(351), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((HAL_SPI_Transmit_IT_fake.call_count)), (

   ((void *)0)

   ), (UNITY_UINT)(352), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((HAL_SPI_DeInit_fake.call_count)), (

   ((void *)0)

   ), (UNITY_UINT)(353), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((HAL_SPI_Init_fake.call_count)), (

   ((void *)0)

   ), (UNITY_UINT)(354), UNITY_DISPLAY_STYLE_INT);



}





void test_se_recibe_un_mensaje_con_payload_y_se_levanta_el_cs_en_el_medio(void) {



    HAL_SPI_Receive_IT_fake.custom_fake = HAL_SPI_Receive_IT_aux_receive_correct_address_and_request_data;





    PowerSPI_CsFallingEdgeISRHandler();





    PowerSPI_RxCompleteISRHandler();





    PowerSPI_CsRisingEdgeISRHandler();







    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_SPI_Receive_IT)), (UNITY_INT64)((fff.call_history[0])), (("Function " "HAL_SPI_Receive_IT" " not called in order " "0")), (UNITY_UINT)(374), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT)((3)), (UNITY_INT)((HAL_SPI_Receive_IT_fake.arg2_history[0])), (

   ((void *)0)

   ), (UNITY_UINT)(375), UNITY_DISPLAY_STYLE_INT);

    do {if ((

   ((void *)0) 

   != HAL_SPI_Receive_IT_fake.arg1_history[0])) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(376)));}} while(0);

    do {if ((&spi_handle == HAL_SPI_Receive_IT_fake.arg0_history[0])) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(377)));}} while(0);





    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_SPI_Receive_IT)), (UNITY_INT64)((fff.call_history[1])), (("Function " "HAL_SPI_Receive_IT" " not called in order " "1")), (UNITY_UINT)(380), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT)((20)), (UNITY_INT)((HAL_SPI_Receive_IT_fake.arg2_history[1])), (

   ((void *)0)

   ), (UNITY_UINT)(381), UNITY_DISPLAY_STYLE_INT);

    do {if ((

   ((void *)0) 

   != HAL_SPI_Receive_IT_fake.arg1_history[1])) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(382)));}} while(0);

    do {if ((&spi_handle == HAL_SPI_Receive_IT_fake.arg0_history[1])) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(383)));}} while(0);





    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_SPI_Abort)), (UNITY_INT64)((fff.call_history[2])), (("Function " "HAL_SPI_Abort" " not called in order " "2")), (UNITY_UINT)(386), UNITY_DISPLAY_STYLE_HEX64);



    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_SPI_RegisterCallback)), (UNITY_INT64)((fff.call_history[3])), (("Function " "HAL_SPI_RegisterCallback" " not called in order " "3")), (UNITY_UINT)(388), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_SPI_DeInit)), (UNITY_INT64)((fff.call_history[4])), (("Function " "HAL_SPI_DeInit" " not called in order " "4")), (UNITY_UINT)(389), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_GPIO_DeInit)), (UNITY_INT64)((fff.call_history[5])), (("Function " "HAL_GPIO_DeInit" " not called in order " "5")), (UNITY_UINT)(390), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_SPI_RegisterCallback)), (UNITY_INT64)((fff.call_history[6])), (("Function " "HAL_SPI_RegisterCallback" " not called in order " "6")), (UNITY_UINT)(391), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_SPI_Init)), (UNITY_INT64)((fff.call_history[7])), (("Function " "HAL_SPI_Init" " not called in order " "7")), (UNITY_UINT)(392), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_GPIO_Init)), (UNITY_INT64)((fff.call_history[8])), (("Function " "HAL_GPIO_Init" " not called in order " "8")), (UNITY_UINT)(393), UNITY_DISPLAY_STYLE_HEX64);

    UnityAssertEqualNumber((UNITY_INT64)(((void *) HAL_GPIO_Init)), (UNITY_INT64)((fff.call_history[9])), (("Function " "HAL_GPIO_Init" " not called in order " "9")), (UNITY_UINT)(394), UNITY_DISPLAY_STYLE_HEX64);

}
