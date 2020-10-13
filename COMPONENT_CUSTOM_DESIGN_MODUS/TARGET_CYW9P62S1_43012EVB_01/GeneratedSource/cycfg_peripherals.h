/*******************************************************************************
* File Name: cycfg_peripherals.h
*
* Description:
* Peripheral Hardware Block configuration
* This file was automatically generated and should not be modified.
* Tools Package 2.2.0.2407
* latest-v2.X 2.0.0.5642
* personalities 3.0.0.0
* udd 3.0.0.539
*
********************************************************************************
* Copyright 2020 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
********************************************************************************/

#if !defined(CYCFG_PERIPHERALS_H)
#define CYCFG_PERIPHERALS_H

#include "cycfg_notices.h"
#include "cy_sysclk.h"
#include "cy_csd.h"
#include "cy_csdadc.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define CSDADC_ENABLED 1U
#define CSDADC_HW CSD0
#define CSDADC_IRQ csd_interrupt_IRQn
#define CSDADC_CSDADC_CHANN_NUM 1u
#define CSDADC_CSDADC_ALL_CHAN_MASK ((1u << CSDADC_CSDADC_CHANN_NUM) - 1u)

extern cy_stc_csd_context_t cy_csd_0_context;
extern const cy_stc_csdadc_config_t CSDADC_csdadc_config;

void init_cycfg_peripherals(void);

#if defined(__cplusplus)
}
#endif


#endif /* CYCFG_PERIPHERALS_H */
