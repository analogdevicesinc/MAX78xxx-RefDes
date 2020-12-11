/* **** Includes **** */
#include <stddef.h>
#include "mxc_assert.h"
#include "tmr.h"
#include "gpio.h"
#include "mxc_pins.h"
#include "mxc_lock.h"

int MXC_TMR_ME15_Init (mxc_tmr_regs_t *tmr, mxc_tmr_cfg_t* cfg);
void MXC_TMR_ME15_Shutdown (mxc_tmr_regs_t *tmr);