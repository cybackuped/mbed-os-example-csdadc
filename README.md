# Introduction
This example demonstrates the usage of CSD analog to digital converter (ADC) for PSoC&reg; 6 MCU with Mbed OS using the [CSDADC Middleware Library](https://github.com/cypresssemiconductorco/csdadc). In this project an external voltage is measured by using CSDADC and displays the equivalent digital value in UART terminal. Additional PSoC 6-related code examples are available in other repos. See all examples at [Code Examples for Modus Toolbox](https://github.com/cypresssemiconductorco/Code-Examples-for-ModusToolbox-Software).

# Instructions to run the CSDADC code example

1. Import the code example
	
        mbed import https://github.com/cypresssemiconductorco/mbed-os-example-csdadc

2. Change working directory to the example folder

        cd mbed-os-example-csdadc
   
3. Plug in the CY8CPROTO-062-4343W kit. Ensure that the kit has the latest version of KitProg3 firmware and is in DAPLink mode to allow programming from Mbed CLI. See [Firmware-loader](https://github.com/cypresssemiconductorco/Firmware-loader) for more information on how to update the firmware and switch to DAPLink mode.

4. Place a potentiometer on P10[0] to change the input voltage.

5. Compile the example and Program

        mbed compile --target CY8CPROTO_062_4343W --toolchain GCC_ARM --flash --sterm
        
        For other targets:
        mbed compile --target CY8CKIT_062_WIFI_BT --toolchain GCC_ARM --flash --sterm
        mbed compile --target CY8CKIT_062_BLE --toolchain GCC_ARM --flash --sterm

    **Note:** The *--sterm* option opens the serial terminal with 9600-8N1 setting on the command prompt itself after programming completes. Do not use this option if you wish to connect using another serial terminal application.

6. Following message is displayed on the serial terminal when the application starts running.

        CSDADC Code Example

7. Turn the knob of the potentiometer and observe result printed on the serial terminal.

    CSDADC Code Example

    	ADC Count: 289
    	ADC Count: 289
    	ADC Count: 291
    	ADC Count: 216
    	ADC Count: 75

# How to modify the CSDADC input pin for this project

In order to modify the input pin of the CSDADC follow the steps below:

1. Update the pin information in the 'csdadc_channel_list' variable.

2. Update the function 'config_routing' with new AMUX bus connection. 

* For CY8CKIT-062-BLE PSoC 6 BLE Pioneer Kit 

| Port                                    | AMUX configuration                                           |
| --------------------------------------- | ------------------------------------------------------------ |
| Port 0<br>Port 11<br>Port 12<br>Port 13 | `HSIOM->AMUX_SPLIT_CTL[5] = HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SL_Msk \| HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SR_Msk;` <br>  `HSIOM->AMUX_SPLIT_CTL[6] = HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SL_Msk \| HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SR_Msk;` |
| Port 1                                  | `HSIOM->AMUX_SPLIT_CTL[2] = HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SL_Msk \| HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SR_Msk;` <br>  `HSIOM->AMUX_SPLIT_CTL[4] = HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SL_Msk \| HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SR_Msk;` |
| Port 5<br>Port 6<br>Port 7<br>Port 8    | `HSIOM->AMUX_SPLIT_CTL[4] = HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SL_Msk \| HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SR_Msk;` |
| Port 9<br>Port 10                       | `HSIOM->AMUX_SPLIT_CTL[5] = HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SL_Msk \| HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SR_Msk;` |

* For CY8CKIT-062-WiFi-BT PSoC 6 WiFi-BT Pioneer Kit

| Port         | AMUX configuration |
| ------------- | ----------------------- |
| Port 0<br>Port 11<br>Port 12<br>Port 13  | `HSIOM->AMUX_SPLIT_CTL[5] = HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SL_Msk \| HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SR_Msk;` <br>  `HSIOM->AMUX_SPLIT_CTL[6] = HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SL_Msk \| HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SR_Msk;` |
| Port 1<br>Port 2<br>Port 3<br>Port 4 | `HSIOM->AMUX_SPLIT_CTL[2] = HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SL_Msk \| HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SR_Msk;` <br>  `HSIOM->AMUX_SPLIT_CTL[4] = HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SL_Msk \| HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SR_Msk;`  |
| Port 5<br>Port 6<br>Port 7<br>Port 8 | `HSIOM->AMUX_SPLIT_CTL[4] = HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SL_Msk \| HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SR_Msk;` |
| Port 9<br>Port 10 | `HSIOM->AMUX_SPLIT_CTL[5] = HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SL_Msk \| HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SR_Msk;` |

* For CY8CPROTO-062-4343W PSoC 6 Wi-Fi BT Prototyping Kit

| Port         | AMUX configuration |
| ------------- | ----------------------- |
| Port 0<br>Port 11<br>Port 12<br>Port 13  | `HSIOM->AMUX_SPLIT_CTL[5] = HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SL_Msk \| HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SR_Msk;` <br>  `HSIOM->AMUX_SPLIT_CTL[6] = HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SL_Msk \| HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SR_Msk;` |
| Port 1<br>Port 2<br>Port 3<br>Port 4 | `HSIOM->AMUX_SPLIT_CTL[2] = HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SL_Msk \| HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SR_Msk;` <br>  `HSIOM->AMUX_SPLIT_CTL[4] = HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SL_Msk \| HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SR_Msk;`  |
| Port 5<br>Port 6<br>Port 7<br>Port 8 | `HSIOM->AMUX_SPLIT_CTL[4] = HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SL_Msk \| HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SR_Msk;` |
| Port 9<br>Port 10 | `HSIOM->AMUX_SPLIT_CTL[5] = HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SL_Msk \| HSIOM_AMUX_SPLIT_CTL_SWITCH_BB_SR_Msk;` |

# Reference

- [CSDADC Middleware API Reference Guide](https://cypresssemiconductorco.github.io/csdadc/csdadc_api_reference_manual/html/index.html)
- [ModusToolbox IDE User Guide](https://www.cypress.com/ModusToolboxUserGuide)
- [CY8CPROTO-062-4343W PSoC 6 Wi-Fi BT Prototyping kit](http://www.cypress.com/documentation/development-kitsboards/cy8cproto-063-4343w)
- [CY8CKIT-062-WiFi-BT PSoC 6 WiFi-BT Pioneer Kit](http://www.cypress.com/documentation/development-kitsboards/psoc-6-wifi-bt-pioneer-kit)
- [CY8CKIT-062-BLE PSoC 6 BLE Pioneer Kit](http://www.cypress.com/documentation/development-kitsboards/psoc-6-ble-pioneer-kit)

# Release Notes

| Version | Description                                                  |
| ------- | ------------------------------------------------------------ |
| 1.0     | Initial release. <br>Tested with mbed-os v5.13.1 and CSDADC middleware v2.0. |

