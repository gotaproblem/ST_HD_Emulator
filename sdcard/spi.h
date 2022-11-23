/* spi.h
Copyright 2021 Carl John Kugler III

Licensed under the Apache License, Version 2.0 (the License); you may not use
this file except in compliance with the License. You may obtain a copy of the
License at

   http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an AS IS BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
*/

#pragma once

#include <stdbool.h>
#include "hardware/dma.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "hardware/spi.h"
#include "pico/types.h"

/* defined in emu.h but can't be include here */
#define USEDMA 1 

#define SPI_FILL_CHAR (0xFF)


typedef struct {

    spi_inst_t *hw_inst;
    uint miso_gpio;
    uint mosi_gpio;
    uint sck_gpio;
    uint baud_rate;

#if USEDMA
    uint tx_dma;
    uint rx_dma;
    dma_channel_config tx_dma_cfg;
    dma_channel_config rx_dma_cfg;
    irq_handler_t dma_isr;
#endif
    bool initialized;

} spi_t;

#ifdef __cplusplus
extern "C" {
#endif

#if USEDMA
void __not_in_flash_func(spi_irq_handler)(spi_t *pSPI);
#endif

bool my_spi_init(spi_t *pSPI);
#if USEDMA
void set_spi_dma_irq_channel(bool useChannel1, bool shared);
#endif

#ifdef __cplusplus
}
#endif

/* [] END OF FILE */
