

/** @file
 ** @brief Funciones de TEST de PowerSPI
 **/

/* === Headers files inclusions
 * =============================================================== */

#include "unity.h"
#include "mock_stm32g0xx_hal.h"
#include "powerspi.h"
#include "powerspi_stm32_port.h"

/* === Macros definitions
 * ====================================================================== */

#define UNUSED(X) (void)X      /* To avoid gcc/g++ warnings */
#define MSG_ADDRESS 5
#define MSG_ADDRESS_INDEX 0
#define MSG_CMD 2
#define MSG_CMD_INDEX 1
#define MSG_SIZE_INDEX 2

/* === Private data type declarations
 * ========================================================== */

/* === Private variable declarations
 * =========================================================== */

/* === Private function declarations
 * =========================================================== */

/* === Public variable definitions
 * ============================================================= */

/* === Private variable definitions
 * ============================================================ */

static SPI_HandleTypeDef spi_handle;
static powerspi_gpio_t spi_miso = {0, 0};
static powerspi_gpio_t spi_mosi = {0, 0};
static powerspi_gpio_t spi_sck = {0, 0};
static powerspi_gpio_t spi_cs = {0, 0};

/* === Private function implementation
 * ========================================================= */

// Simula que se recibe un mensaje que es para nosotros pero sin datos de request
HAL_StatusTypeDef HAL_SPI_Receive_IT_aux_receive_correct_address_and_no_request_data(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size)
{
    HAL_StatusTypeDef errorcode = HAL_OK;

    UNUSED(hspi);
    UNUSED(Size);

    pData[MSG_ADDRESS_INDEX] = MSG_ADDRESS;
    pData[MSG_CMD_INDEX] = MSG_CMD;
    pData[MSG_SIZE_INDEX] = 0; // NO REQUEST DATA

    return errorcode;
}

// Simula que se recibe un mensaje que es para nosotros y viene 20 bytes de datos de request
HAL_StatusTypeDef HAL_SPI_Receive_IT_aux_receive_correct_address_and_request_data(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size)
{
    HAL_StatusTypeDef errorcode = HAL_OK;

    // Simulo recepcion de datos de acuerdo al size.
    // Si size es 3, entonces supongo que estamos recibiendo ADDRESS, CMD y SIZE
    // Si size es mayor a 3, entonces supongo que estamos recibiendo datos.
    UNUSED(hspi);

    if(Size == 3) {
        pData[MSG_ADDRESS_INDEX] = MSG_ADDRESS;
        pData[MSG_CMD_INDEX] = MSG_CMD;
        pData[MSG_SIZE_INDEX] = 20; // 20 BYTES OF REQUEST DATA
    }
    else if(Size == 20) {
        for(int i = 0; i < 20; i++) {
            pData[i] = i;
        }
    }
    else {
        TEST_FAIL_MESSAGE("En este test no deberias pedir solo 3 bytes o 20 bytes...");
    }

    return errorcode;
}

// Simula que se recibe un mensaje pero son para otro dispositivo (otro address)
HAL_StatusTypeDef HAL_SPI_Receive_IT_aux_receive_other_address(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size)
{
    HAL_StatusTypeDef errorcode = HAL_OK;

    UNUSED(hspi);
    UNUSED(Size);

    pData[MSG_ADDRESS_INDEX] = MSG_ADDRESS+1; // OTHER ADDRESS
    pData[MSG_CMD_INDEX] = MSG_CMD;
    pData[MSG_SIZE_INDEX] = 0; // NO REQUEST DATA

    return errorcode;
}

// Fake HAL_SPI_Init que utiliza el MSP Init registrado por callback
HAL_StatusTypeDef HAL_SPI_Init_aux(SPI_HandleTypeDef *hspi)
{
    HAL_StatusTypeDef errorcode = HAL_OK;

    /* Init the low level hardware : GPIO, CLOCK, NVIC... */
    hspi->MspInitCallback(hspi);

    return errorcode;
}

// Fake HAL_SPI_DeInit que utiliza el MSP De Init registrado por callback
HAL_StatusTypeDef HAL_SPI_DeInit_aux(SPI_HandleTypeDef *hspi)
{
    HAL_StatusTypeDef errorcode = HAL_OK;

    /* DeInit the low level hardware : GPIO, CLOCK, NVIC... */
    hspi->MspDeInitCallback(hspi);

    return errorcode;
}

// Registro de callback de MSP_DeInit y MSP_Init
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
        /* Return error status */
        status =  HAL_ERROR;
        break;
    }

    return status;
}

// Callback para procesar el mensaje recibido
void callback(uint8_t cmd, uint8_t * buffer, uint8_t * size) {
    if(cmd == MSG_CMD) {
        // Si tengo implementado un procesamiento de este comando, contesto "HOLA" y envio el size correspondiente (4)
        buffer[0] = 'H';
        buffer[1] = 'O';
        buffer[2] = 'L';
        buffer[3] = 'A';
        *size = 4;
    }
    else {
        // Si no tengo implementado un procesamiento de este comando, no envio nada y devuelvo size 0
        *size = 0;
    }
}

/* === Public function implementation
 * ========================================================== */

void suiteSetUp() {
    // Inicializo PowerSPI
    PowerSPI_Init(MSG_ADDRESS, callback, &spi_handle, spi_miso, spi_mosi, spi_sck, spi_cs);
}

void setUp() {
    FFF_RESET_HISTORY();

    // Reseteo fake de recepcion (es el unico que cambia de test a test)
    RESET_FAKE(HAL_SPI_Receive_IT);

    // Fakes que no cambian nunca para todos los tests
    HAL_SPI_Init_fake.custom_fake = HAL_SPI_Init_aux;
    HAL_SPI_DeInit_fake.custom_fake = HAL_SPI_DeInit_aux;
    HAL_SPI_RegisterCallback_fake.custom_fake = HAL_SPI_RegisterCallback_aux;
}

void tearDown() {
    // Para reiniciar la MEF de PowerSPI es necesario levantar el CS sino permanecerá escuchando datos eternamente y fallaran todos los tests.
    PowerSPI_CsRisingEdgeISRHandler();
}

// Test para probar que al ocurrir un flanco descendente del CS, PowerSPI se pone a la espera del address, command y size (3 bytes)
void test_se_detecta_flanco_descendente_de_cs_y_comienza_a_escuchar(void) {

    PowerSPI_CsFallingEdgeISRHandler();

    // // Verifico que se haya llamado a HAL_SPI_Receive_IT()
    // TEST_ASSERT_SEQUENCE_START(HAL_SPI_Receive_IT);
    // // Verifico que llame a HAL_SPI_Receive_IT()  
    // Verifico que se haya llamado a HAL_SPI_Receive_IT() una sola vez
    TEST_ASSERT_EQUAL(1, HAL_SPI_Receive_IT_fake.call_count);
    // Verifico que se hayan pedido 3 bytes
    TEST_ASSERT_EQUAL(3, HAL_SPI_Receive_IT_fake.arg2_history[0]);
    // Verifico que el puntero al buffer no sea NULL
    TEST_ASSERT(NULL != HAL_SPI_Receive_IT_fake.arg1_history[0]);
    // Verifico que el handle sea correcto
    TEST_ASSERT(&spi_handle == HAL_SPI_Receive_IT_fake.arg0_history[0]);
}

// Test para probar que al recibir el address correspondiente al dispositivo y size cero, el dispositivo contesta directamente.
void test_se_recibe_un_mensaje_sin_payload_y_contestamos_directamente(void) {

    // Inicializo los fakes necesarios
    HAL_SPI_Receive_IT_fake.custom_fake = HAL_SPI_Receive_IT_aux_receive_correct_address_and_no_request_data;

    // Simulo ISR de flanco negativo CS
    PowerSPI_CsFallingEdgeISRHandler();

    // Simulo ISR recepcion completa de 3 bytes por SPI
    PowerSPI_RxCompleteISRHandler();

    // Simulo ISR fin de transmision de respuesta
    PowerSPI_TxCompleteISRHandler();

    // Verifico que se hayan llamado a las funciones en el orden correcto
    // Inicia recepcion
    TEST_ASSERT_CALLED_IN_ORDER(0, HAL_SPI_Receive_IT);
    TEST_ASSERT_EQUAL(3, HAL_SPI_Receive_IT_fake.arg2_history[0]); // Verifico que se hayan pedido 3 bytes
    TEST_ASSERT(NULL != HAL_SPI_Receive_IT_fake.arg1_history[0]); // Verifico que el puntero al buffer no sea NULL
    TEST_ASSERT(&spi_handle == HAL_SPI_Receive_IT_fake.arg0_history[0]); // Verifico que el handle sea correcto
    // Activa TX para responder
    TEST_ASSERT_CALLED_IN_ORDER(1, HAL_SPI_RegisterCallback);
    TEST_ASSERT_CALLED_IN_ORDER(2, HAL_SPI_DeInit);
    TEST_ASSERT_CALLED_IN_ORDER(3, HAL_GPIO_DeInit);
    TEST_ASSERT_CALLED_IN_ORDER(4, HAL_SPI_RegisterCallback);
    TEST_ASSERT_CALLED_IN_ORDER(5, HAL_SPI_Init);
    TEST_ASSERT_CALLED_IN_ORDER(6, HAL_GPIO_Init);
    // Responde
    TEST_ASSERT_CALLED_IN_ORDER(7, HAL_SPI_Transmit_IT);
    TEST_ASSERT_EQUAL(3+4, HAL_SPI_Transmit_IT_fake.arg2_history[0]); // Verifico que se hayan enviado 3 bytes de header + 4 de payload (definido en callback())
    TEST_ASSERT(NULL != HAL_SPI_Transmit_IT_fake.arg1_history[0]); // Verifico que el puntero al buffer no sea NULL
    TEST_ASSERT(&spi_handle == HAL_SPI_Transmit_IT_fake.arg0_history[0]); // Verifico que el handle sea correcto
    // Desactiva TX luego de responder
    TEST_ASSERT_CALLED_IN_ORDER(8, HAL_SPI_RegisterCallback);
    TEST_ASSERT_CALLED_IN_ORDER(9, HAL_SPI_DeInit);
    TEST_ASSERT_CALLED_IN_ORDER(10, HAL_GPIO_DeInit);
    TEST_ASSERT_CALLED_IN_ORDER(11, HAL_SPI_RegisterCallback);
    TEST_ASSERT_CALLED_IN_ORDER(12, HAL_SPI_Init);
    TEST_ASSERT_CALLED_IN_ORDER(13, HAL_GPIO_Init);
    TEST_ASSERT_CALLED_IN_ORDER(14, HAL_GPIO_Init);
}

// Test para probar que al recibir el address correspondiente al dispositivo y size mayor a cero, el dispositivo recibe completamente el request y luego contesta.
void test_se_recibe_un_mensaje_con_payload_y_contestamos(void) {

    // Inicializo los fakes necesarios
    HAL_SPI_Receive_IT_fake.custom_fake = HAL_SPI_Receive_IT_aux_receive_correct_address_and_request_data;

    // Simulo ISR de flanco negativo CS
    PowerSPI_CsFallingEdgeISRHandler();

    // Simulo ISR recepcion completa de 3 bytes por SPI
    PowerSPI_RxCompleteISRHandler();

    // Simulo ISR recepcion completa de 20 bytes por SPI (definido por el size establecido en el fake)
    PowerSPI_RxCompleteISRHandler();

    // Simulo ISR fin de transmision de respuesta
    PowerSPI_TxCompleteISRHandler();

    // Verifico que se hayan llamado a las funciones en el orden correcto
    // Inicia recepcion 3 bytes
    TEST_ASSERT_CALLED_IN_ORDER(0, HAL_SPI_Receive_IT);
    TEST_ASSERT_EQUAL(3, HAL_SPI_Receive_IT_fake.arg2_history[0]); // Verifico que se hayan pedido 3 bytes
    TEST_ASSERT(NULL != HAL_SPI_Receive_IT_fake.arg1_history[0]); // Verifico que el puntero al buffer no sea NULL
    TEST_ASSERT(&spi_handle == HAL_SPI_Receive_IT_fake.arg0_history[0]); // Verifico que el handle sea correcto
    // Inicia recepcion 20 bytes 
    TEST_ASSERT_CALLED_IN_ORDER(1, HAL_SPI_Receive_IT);
    TEST_ASSERT_EQUAL(20, HAL_SPI_Receive_IT_fake.arg2_history[1]); // Verifico que se hayan pedido 20 bytes
    TEST_ASSERT(NULL != HAL_SPI_Receive_IT_fake.arg1_history[1]); // Verifico que el puntero al buffer no sea NULL
    TEST_ASSERT(&spi_handle == HAL_SPI_Receive_IT_fake.arg0_history[1]); // Verifico que el handle sea correcto
    // Activa TX para responder
    TEST_ASSERT_CALLED_IN_ORDER(2, HAL_SPI_RegisterCallback);
    TEST_ASSERT_CALLED_IN_ORDER(3, HAL_SPI_DeInit);
    TEST_ASSERT_CALLED_IN_ORDER(4, HAL_GPIO_DeInit);
    TEST_ASSERT_CALLED_IN_ORDER(5, HAL_SPI_RegisterCallback);
    TEST_ASSERT_CALLED_IN_ORDER(6, HAL_SPI_Init);
    TEST_ASSERT_CALLED_IN_ORDER(7, HAL_GPIO_Init);
    // Responde
    TEST_ASSERT_CALLED_IN_ORDER(8, HAL_SPI_Transmit_IT);
    TEST_ASSERT_EQUAL(3+4, HAL_SPI_Transmit_IT_fake.arg2_history[0]); // Verifico que se hayan enviado 3 bytes de header + 4 de payload (definido en callback())
    TEST_ASSERT(NULL != HAL_SPI_Transmit_IT_fake.arg1_history[0]); // Verifico que el puntero al buffer no sea NULL
    TEST_ASSERT(&spi_handle == HAL_SPI_Transmit_IT_fake.arg0_history[0]); // Verifico que el handle sea correcto
    // Desactiva TX luego de responder
    TEST_ASSERT_CALLED_IN_ORDER(9, HAL_SPI_RegisterCallback);
    TEST_ASSERT_CALLED_IN_ORDER(10, HAL_SPI_DeInit);
    TEST_ASSERT_CALLED_IN_ORDER(11, HAL_GPIO_DeInit);
    TEST_ASSERT_CALLED_IN_ORDER(12, HAL_SPI_RegisterCallback);
    TEST_ASSERT_CALLED_IN_ORDER(13, HAL_SPI_Init);
    TEST_ASSERT_CALLED_IN_ORDER(14, HAL_GPIO_Init);
    TEST_ASSERT_CALLED_IN_ORDER(15, HAL_GPIO_Init);
}

// // Test para probar que al recibir un address de otro dispositivo, el dispositivo no contesta.
void test_se_recibe_un_mensaje_dirigido_a_otro_dispositivo(void) {

    // Inicializo los fakes necesarios
    HAL_SPI_Receive_IT_fake.custom_fake = HAL_SPI_Receive_IT_aux_receive_other_address;

    // Simulo ISR de flanco negativo CS
    PowerSPI_CsFallingEdgeISRHandler();

    // Simulo ISR recepcion completa de 3 bytes por SPI
    PowerSPI_RxCompleteISRHandler();

    // Como no deberia hacer nada porque el address recibido es el de otro dispositivo, probamos esto por la negativa.
    // Es decir, miranos que NO haya llamado a ninguna funcion de recepcion ni de transmision.
    TEST_ASSERT_EQUAL(1, HAL_SPI_Receive_IT_fake.call_count); // Solo llama una vez a la funcion de recepcion (primeros 3 bytes)
    TEST_ASSERT_EQUAL(0, HAL_SPI_Transmit_IT_fake.call_count); // Nunca deberia llamar a la funcion de transmitir7
    TEST_ASSERT_EQUAL(0, HAL_SPI_DeInit_fake.call_count); // Nunca deberia cambiar la configuracion de SPI
    TEST_ASSERT_EQUAL(0, HAL_SPI_Init_fake.call_count); // Nunca deberia cambiar la configuracion de SPI

}

// Test para probar que al levantarse el CS en medio de una comunicacion, se aborta toda la comunicacion.
void test_se_recibe_un_mensaje_con_payload_y_se_levanta_el_cs_en_el_medio(void) {
    // Inicializo los fakes necesarios
    HAL_SPI_Receive_IT_fake.custom_fake = HAL_SPI_Receive_IT_aux_receive_correct_address_and_request_data;

    // Simulo ISR de flanco negativo CS
    PowerSPI_CsFallingEdgeISRHandler();

    // Simulo ISR recepcion completa de 3 bytes por SPI
    PowerSPI_RxCompleteISRHandler();

    // Simulo que se levanto el chip select (a pesar de que el maestro nos iba a enviar 20 bytes mas)
    PowerSPI_CsRisingEdgeISRHandler();

    // Verifico que se hayan llamado a las funciones en el orden correcto
    // Inicia recepcion 3 bytes
    TEST_ASSERT_CALLED_IN_ORDER(0, HAL_SPI_Receive_IT);
    TEST_ASSERT_EQUAL(3, HAL_SPI_Receive_IT_fake.arg2_history[0]); // Verifico que se hayan pedido 3 bytes
    TEST_ASSERT(NULL != HAL_SPI_Receive_IT_fake.arg1_history[0]); // Verifico que el puntero al buffer no sea NULL
    TEST_ASSERT(&spi_handle == HAL_SPI_Receive_IT_fake.arg0_history[0]); // Verifico que el handle sea correcto

    // Inicia recepcion de 20 bytes
    TEST_ASSERT_CALLED_IN_ORDER(1, HAL_SPI_Receive_IT);
    TEST_ASSERT_EQUAL(20, HAL_SPI_Receive_IT_fake.arg2_history[1]); // Verifico que se hayan pedido 20 bytes
    TEST_ASSERT(NULL != HAL_SPI_Receive_IT_fake.arg1_history[1]); // Verifico que el puntero al buffer no sea NULL
    TEST_ASSERT(&spi_handle == HAL_SPI_Receive_IT_fake.arg0_history[1]); // Verifico que el handle sea correcto

    // Detencion de comunicacion
    TEST_ASSERT_CALLED_IN_ORDER(2, HAL_SPI_Abort);
    // Desactiva TX (sin importar como estaba antes)
    TEST_ASSERT_CALLED_IN_ORDER(3, HAL_SPI_RegisterCallback);
    TEST_ASSERT_CALLED_IN_ORDER(4, HAL_SPI_DeInit);
    TEST_ASSERT_CALLED_IN_ORDER(5, HAL_GPIO_DeInit);
    TEST_ASSERT_CALLED_IN_ORDER(6, HAL_SPI_RegisterCallback);
    TEST_ASSERT_CALLED_IN_ORDER(7, HAL_SPI_Init);
    TEST_ASSERT_CALLED_IN_ORDER(8, HAL_GPIO_Init);
    TEST_ASSERT_CALLED_IN_ORDER(9, HAL_GPIO_Init);
}