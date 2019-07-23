/*******************************************************************************
* File Name: main.cpp
*
* Version: 1.0
*
* Description:
*   This example demonstrates the usage of CSD analog to digital converter
*   (ADC). In this project an external voltage is measured by using CSDADC
*   and displays the equivalent digital value in UART terminal.
*
* Related Document: README.md
*
* Supported Kits (Target Names):
*   CY8CKIT-062-BLE PSoC 6 BLE Pioneer Kit (CY8CKIT_062_BLE)
*   CY8CKIT-062-WiFi-BT PSoC 6 WiFi-BT Pioneer Kit (CY8CKIT_062_WIFI_BT)
*   CY8CPROTO-062-4343W PSoC 6 Wi-Fi BT Prototyping Kit (CY8CPROTO_062_4343W)
*
*******************************************************************************
* Copyright (2019), Cypress Semiconductor Corporation. All rights reserved.
*******************************************************************************
* This software, including source code, documentation and related materials
* (“Software”), is owned by Cypress Semiconductor Corporation or one of its
* subsidiaries (“Cypress”) and is protected by and subject to worldwide patent
* protection (United States and foreign), United States copyright laws and
* international treaty provisions. Therefore, you may use this Software only
* as provided in the license agreement accompanying the software package from
* which you obtained this Software (“EULA”).
*
* If no EULA applies, Cypress hereby grants you a personal, nonexclusive,
* non-transferable license to copy, modify, and compile the Software source
* code solely for use in connection with Cypress’s integrated circuit products.
* Any reproduction, modification, translation, compilation, or representation
* of this Software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death (“High Risk Product”). By
* including Cypress’s product in a High Risk Product, the manufacturer of such
* system or application assumes all risk of such use and in doing so agrees to
* indemnify Cypress against all liability.
*******************************************************************************/


/*******************************************************************************
* Header files including
********************************************************************************/
#include "mbed.h"
#include "cy_pdl.h"
#include "cy_device_headers.h"
#include "cy_csdadc.h"


/*******************************************************************************
* Global constants
********************************************************************************/
#define CSD_HW                  (CSD0)
#define CHANNEL_IDX0_BIT_POS    (0u)                        /* Channel 0 */
#define CHANNEL_IDX0_BIT_MASK   (1 << CHANNEL_IDX0_BIT_POS)
#define CSDADC_SCAN_PERIOD      (1000)                      /* in millisecond */
#define CSDADC_SCAN_WAIT_DELAY  (1)                         /* in millisecond */


/*******************************************************************************
* Function Prototypes
********************************************************************************/
static void csdadc_interrupt_handler(void);
static void config_routing(void);
static void check_status(const char *message, uint32_t status);
static void csdadc_scan(void);
static void config_clock(void);

/*******************************************************************************
* Interrupt configuration
*******************************************************************************/
const cy_stc_sysint_t csdadc_interrupt_config =
{
        .intrSrc = csd_interrupt_IRQn,
        .intrPriority = 7u,
};


/*******************************************************************************
* Global variables
*******************************************************************************/
EventQueue queue;
Semaphore csdadc_sem(0,1);
cy_stc_csd_context_t cy_csd_context;
cy_stc_csdadc_context_t cy_csdadc_context;

static const cy_stc_csdadc_ch_pin_t csdadc_channel_list[] =
{
    [0] = {
            .ioPcPtr = GPIO_PRT10,
            .pin = 0u,
          },
};

const cy_stc_csdadc_config_t csdadc_config =
{
    .ptrPinList = &csdadc_channel_list[0u],
    .base = CSD_HW,
    .csdCxtPtr = &cy_csd_context,
    .cpuClk = 144000000u,
    .periClk = 50000000u,
    .vref = -1,
    .vdda = 3300u,
    .calibrInterval = 30u,
    .range = CY_CSDADC_RANGE_VDDA,
    .resolution = CY_CSDADC_RESOLUTION_10BIT,
    .periDivTyp = CY_SYSCLK_DIV_8_BIT,
    .numChannels = 1u,
    .idac = 31u,
    .operClkDivider = 1u,
    .azTime = 5u,
    .acqTime = 10u,
    .csdInitTime = 25u,
    .idacCalibrationEn = 0u,
    .periDivInd = 0u,
};

/*****************************************************************************
* Function Name: main()
******************************************************************************
* Summary:
*  Main function that starts a thread for CSDADC conversion and enters a
*  forever wait state.
*
*****************************************************************************/
int main(void)
{
    /* Variable used for storing CSDADC API return result */
    cy_status status;

    config_clock();
    config_routing();

    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
    printf("\x1b[2J\x1b[;H");
    printf("CSDADC Code Example\r\n\n");

    /* Initialize CSDADC */
    status = Cy_CSDADC_Init(&csdadc_config, &cy_csdadc_context);
    check_status("CSDADC initialization failed.", status);

    /* Initialize CSDADC interrupt */
    Cy_SysInt_Init(&csdadc_interrupt_config, csdadc_interrupt_handler);
    NVIC_ClearPendingIRQ(csdadc_interrupt_config.intrSrc);
    NVIC_EnableIRQ(csdadc_interrupt_config.intrSrc);

    /* Initialize CSDADC firmware modules */
    status = Cy_CSDADC_Enable(&cy_csdadc_context);
    check_status("CSDADC firmware module initialization failed.", status);

    /* Reseting the semaphore before starting CSDADC Thread */
    csdadc_sem.acquire();

    /* Create a thread to start ADC conversion periodically using an event
     * queue dispatcher.
     */
    Thread thread(osPriorityNormal, OS_STACK_SIZE, NULL, "CSDADC Thread");
    thread.start(callback(&queue, &EventQueue::dispatch_forever));
    queue.call_every(CSDADC_SCAN_PERIOD, csdadc_scan);

    wait(osWaitForever);
}


/*****************************************************************************
* Function Name: csdadc_scan()
******************************************************************************
* Summary:
*   This function starts the CSDADC conversion, and prints the result on
*   terminal application.
*
*****************************************************************************/
static void csdadc_scan(void)
{
    uint32_t adcResult;
    cy_en_csdadc_status_t  status;

    status = Cy_CSDADC_IsEndConversion(&cy_csdadc_context);
    if(CY_CSDADC_SUCCESS == status)
    {
        status = Cy_CSDADC_StartConvert(CY_CSDADC_SINGLE_SHOT,
            CHANNEL_IDX0_BIT_MASK, &cy_csdadc_context);
        check_status("Unable to start ADC conversion.", status);

        /* Wait until the conversion completes */
        csdadc_sem.acquire();

        status = Cy_CSDADC_IsEndConversion(&cy_csdadc_context);
        if(CY_CSDADC_SUCCESS == status)
        {
            adcResult = Cy_CSDADC_GetResult(CHANNEL_IDX0_BIT_POS,
                         &cy_csdadc_context);
            printf("ADC Count: %lu\n\r", adcResult);
        }
    }
    if (CY_CSDADC_OVERFLOW == status)
    {
        printf("Last conversion caused overflow. Re-calibrating CSDADC\n\r");
        status = Cy_CSDADC_Calibrate(&cy_csdadc_context);
        check_status("CSDADC calibration failed", status);
    }
    else
    {
        check_status("CSDADC hardware error", status);
    }
}


/*****************************************************************************
* Function Name: csdadc_interrupt_handler()
******************************************************************************
* Summary:
*  Wrapper function for handling interrupts from CSD block.
*
*****************************************************************************/
static void csdadc_interrupt_handler(void)
{
    Cy_CSDADC_InterruptHandler(CSD_HW, &cy_csdadc_context);
    csdadc_sem.release();
}


/*****************************************************************************
* Function Name: config_routing()
******************************************************************************
* Summary:
*   Configures the AMUX bus segment.
*
*****************************************************************************/
static void config_routing(void)
{
    HSIOM->AMUX_SPLIT_CTL[5] = HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SL_Msk |
        HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SR_Msk;
}


/*****************************************************************************
* Function Name: config_routing()
******************************************************************************
* Summary:
*   Configures the peripheral clock divider for CSDADC.
*
*****************************************************************************/
static void config_clock(void)
{
    Cy_SysClk_PeriphDisableDivider(CY_SYSCLK_DIV_8_BIT, 0U);
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_8_BIT, 0U, 0U);
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_8_BIT, 0U);
    Cy_SysClk_PeriphAssignDivider(PCLK_CSD_CLOCK, CY_SYSCLK_DIV_8_BIT, 0U);
}


/*******************************************************************************
* Function Name: check_status()
****************************************************************************//**
* Summary:
*   Asserts the non-zero status and prints the message.
*
* Parameters:
*   char *message : message to print if status is non-zero.
*   uint32_t status : status for evaluation.
*
*******************************************************************************/
static inline void check_status(const char *message, uint32_t status)
{
    if(0u != status)
    {
        printf("[Error] : %s Error Code: 0x%08lX\r\n", message, status);
        while(1);
    }
}

/* [] END OF FILE */
