
// FIXME: Copied from ME11, removed RAM shutdown functions, needs review
// FIXME: Needs copyright header

#include <stdio.h>
#include "lp.h"
#include "pwrseq_regs.h"
#include "mxc_errors.h"
#include "gcr_regs.h"
#include "mxc_config.h"
#include "mxc_sys.h"
#include "tmr_utils.h"
#include "mcr_regs.h"

extern void Reset_Handler(void);
extern void Backup_Handler(void);

void LP_ClearWakeStatus(void)
{
    /* Write 1 to clear */
    MXC_PWRSEQ->lpwkst0 = 0xFFFFFFFF;
    MXC_PWRSEQ->lpwkst1 = 0xFFFFFFFF;
    MXC_PWRSEQ->lppwst  = 0xFFFFFFFF;
}

void LP_EnableRTCAlarmWakeup(void)
{
    MXC_GCR->pm |= MXC_F_GCR_PM_RTCWKEN;
}

void LP_DisableRTCAlarmWakeup(void)
{
    MXC_GCR->pm &= ~MXC_F_GCR_PM_RTCWKEN;
}

void LP_EnableGPIOWakeup(gpio_cfg_t *wu_pins)
{
    MXC_GCR->pm |= MXC_F_GCR_PM_GPIOWKEN;
    switch (wu_pins->port) {
        case PORT_0:
            MXC_PWRSEQ->lpwken0 |= wu_pins->mask;
            break;
        case PORT_1:
            MXC_PWRSEQ->lpwken1 |= wu_pins->mask;
    }
}

void LP_DisableGPIOWakeup(gpio_cfg_t *wu_pins)
{
    switch (wu_pins->port) {
        case PORT_0:
            MXC_PWRSEQ->lpwken0 &= ~wu_pins->mask;
            break;
        case PORT_1:
            MXC_PWRSEQ->lpwken1 &= ~wu_pins->mask;
    }
    
    if (MXC_PWRSEQ->lpwken1 == 0 && MXC_PWRSEQ->lpwken0 == 0) {
        MXC_GCR->pm &= ~MXC_F_GCR_PM_GPIOWKEN;
    }
}

void LP_SysRam0LightSleep(void)
{
    MXC_GCR->memckcn |= MXC_F_GCR_MEMCKCN_SYSRAM0LS;   
}

void LP_SysRam1LightSleep(void)
{
    MXC_GCR->memckcn |= MXC_F_GCR_MEMCKCN_SYSRAM1LS;   
}

void LP_SysRam2LightSleep(void)
{
    MXC_GCR->memckcn |= MXC_F_GCR_MEMCKCN_SYSRAM2LS;   
}

void LP_SysRam3LightSleep(void)
{
    MXC_GCR->memckcn |= MXC_F_GCR_MEMCKCN_SYSRAM3LS;   
}

void LP_SysRam4LightSleep(void)
{
    MXC_GCR->memckcn |= MXC_F_GCR_MEMCKCN_SYSRAM4LS;   
}

void LP_SysRam5LightSleep(void)
{
    MXC_GCR->memckcn |= MXC_F_GCR_MEMCKCN_SYSRAM5LS;   
}

void LP_SysRam0Shutdown(void)
{
    MXC_PWRSEQ->lpmemsd |= MXC_F_PWRSEQ_LPMEMSD_SRAM0SD;
}

void LP_SysRam0Wakeup(void)
{
    MXC_GCR->memckcn &= ~MXC_F_GCR_MEMCKCN_SYSRAM0LS;
    MXC_PWRSEQ->lpmemsd &= ~MXC_F_PWRSEQ_LPMEMSD_SRAM0SD;
}

void LP_SysRam1Shutdown(void)
{
    MXC_PWRSEQ->lpmemsd |= MXC_F_PWRSEQ_LPMEMSD_SRAM1SD;
}

void LP_SysRam1Wakeup(void)
{
    MXC_GCR->memckcn &= ~MXC_F_GCR_MEMCKCN_SYSRAM1LS;
    MXC_PWRSEQ->lpmemsd &= ~MXC_F_PWRSEQ_LPMEMSD_SRAM1SD;
}

void LP_SysRam2Shutdown(void)
{
    MXC_PWRSEQ->lpmemsd |= MXC_F_PWRSEQ_LPMEMSD_SRAM2SD;
}

void LP_SysRam2Wakeup(void)
{
    MXC_GCR->memckcn &= ~MXC_F_GCR_MEMCKCN_SYSRAM2LS;
    MXC_PWRSEQ->lpmemsd &= ~MXC_F_PWRSEQ_LPMEMSD_SRAM2SD;
}

void LP_SysRam3Shutdown(void)
{
    MXC_PWRSEQ->lpmemsd |= MXC_F_PWRSEQ_LPMEMSD_SRAM3SD;
}

void LP_SysRam3Wakeup(void)
{
    MXC_GCR->memckcn &= ~MXC_F_GCR_MEMCKCN_SYSRAM3LS;
    MXC_PWRSEQ->lpmemsd &= ~MXC_F_PWRSEQ_LPMEMSD_SRAM3SD;
}

void LP_SysRam4Shutdown(void)
{
    MXC_PWRSEQ->lpmemsd |= MXC_F_PWRSEQ_LPMEMSD_SRAM4SD;
}

void LP_SysRam4Wakeup(void)
{
    MXC_GCR->memckcn &= ~MXC_F_GCR_MEMCKCN_SYSRAM4LS;
    MXC_PWRSEQ->lpmemsd &= ~MXC_F_PWRSEQ_LPMEMSD_SRAM4SD;
}

void LP_SysRam5Shutdown(void)
{
    MXC_PWRSEQ->lpmemsd |= MXC_F_PWRSEQ_LPMEMSD_SRAM5SD;
}

void LP_SysRam5Wakeup(void)
{
    MXC_GCR->memckcn &= ~MXC_F_GCR_MEMCKCN_SYSRAM5LS;
    MXC_PWRSEQ->lpmemsd &= ~MXC_F_PWRSEQ_LPMEMSD_SRAM5SD;
}

void LP_ICacheShutdown(void)
{
    MXC_PWRSEQ->lpmemsd |= MXC_F_PWRSEQ_LPMEMSD_ICACHESD;
}

void LP_ICacheWakeup(void)
{
    MXC_PWRSEQ->lpmemsd &= ~MXC_F_PWRSEQ_LPMEMSD_ICACHESD;
}

void LP_ICacheXIPShutdown(void)
{
    MXC_PWRSEQ->lpmemsd |= MXC_F_PWRSEQ_LPMEMSD_ICACHEXIPSD;
}

void LP_ICacheXIPWakeup(void)
{
    MXC_PWRSEQ->lpmemsd &= ~MXC_F_PWRSEQ_LPMEMSD_ICACHEXIPSD;
}

void LP_CryptoShutdown(void)
{
    MXC_PWRSEQ->lpmemsd |= MXC_F_PWRSEQ_LPMEMSD_CRYPTOSD;
}

void LP_CryptoWakeup(void)
{
    MXC_PWRSEQ->lpmemsd &= ~MXC_F_PWRSEQ_LPMEMSD_CRYPTOSD;
}

void LP_USBFIFOShutdown(void)
{
    MXC_PWRSEQ->lpmemsd |= MXC_F_PWRSEQ_LPMEMSD_USBFIFOSD;
}

void LP_USBFIFOWakeup(void)
{
    MXC_PWRSEQ->lpmemsd &= ~MXC_F_PWRSEQ_LPMEMSD_USBFIFOSD;
}

void LP_ROMShutdown(void)
{
    MXC_PWRSEQ->lpmemsd |= MXC_F_PWRSEQ_LPMEMSD_ROMSD;
}

void LP_ROMWakeup(void)
{
    MXC_PWRSEQ->lpmemsd &= ~MXC_F_PWRSEQ_LPMEMSD_ROMSD;
}

void LP_ROM1Shutdown(void)
{
    MXC_PWRSEQ->lpmemsd |= MXC_F_PWRSEQ_LPMEMSD_ROM1SD;
}

void LP_ROM1Wakeup(void)
{
    MXC_PWRSEQ->lpmemsd &= ~MXC_F_PWRSEQ_LPMEMSD_ROM1SD;
}

void LP_ICache1Shutdown(void)
{
    MXC_PWRSEQ->lpmemsd |= MXC_F_PWRSEQ_LPMEMSD_IC1SD;
}

void LP_ICache1Wakeup(void)
{
    MXC_PWRSEQ->lpmemsd &= ~MXC_F_PWRSEQ_LPMEMSD_IC1SD;
}

void LP_SysCacheShutdown(void)
{
    MXC_PWRSEQ->lpmemsd |= MXC_F_PWRSEQ_LPMEMSD_SRCCSD;
}

void LP_SysCacheWakeup(void)
{
    MXC_PWRSEQ->lpmemsd &= ~MXC_F_PWRSEQ_LPMEMSD_SRCCSD;
}

void LP_USBSWLPDisable(void)
{
    MXC_MCR->ctrl |= MXC_F_MCR_CTRL_USBSWEN_N;
}

void LP_USBSWLPEnable(void)
{
    MXC_MCR->ctrl &= ~MXC_F_MCR_CTRL_USBSWEN_N;
}

void LP_VDD2PowerDown(void)
{
    MXC_PWRSEQ->lpvddpd |= MXC_F_PWRSEQ_LPVDDPD_VDD2PD;
}

void LP_VDD2PowerUp(void)
{
    MXC_PWRSEQ->lpvddpd &= ~MXC_F_PWRSEQ_LPVDDPD_VDD2PD;
}

void LP_VDD3PowerDown(void)
{
    MXC_PWRSEQ->lpvddpd |= MXC_F_PWRSEQ_LPVDDPD_VDD3PD;
}

void LP_VDD3PowerUp(void)
{
    MXC_PWRSEQ->lpvddpd &= ~MXC_F_PWRSEQ_LPVDDPD_VDD3PD;
}

void LP_VDD4PowerDown(void)
{
    MXC_PWRSEQ->lpvddpd |= MXC_F_PWRSEQ_LPVDDPD_VDD4PD;
}

void LP_VDD4PowerUp(void)
{
    MXC_PWRSEQ->lpvddpd &= ~MXC_F_PWRSEQ_LPVDDPD_VDD4PD;
}

void LP_VDD5PowerDown(void)
{
    MXC_PWRSEQ->lpvddpd |= MXC_F_PWRSEQ_LPVDDPD_VDD5PD;
}

void LP_VDD5PowerUp(void)
{
    MXC_PWRSEQ->lpvddpd &= ~MXC_F_PWRSEQ_LPVDDPD_VDD5PD;
}

void LP_SIMOVregBPowerDown(void)
{
    MXC_PWRSEQ->lpvddpd |= MXC_F_PWRSEQ_LPVDDPD_VREGOBPD;
}

void LP_SIMOVregBPowerUp(void)
{
    MXC_PWRSEQ->lpvddpd &= ~MXC_F_PWRSEQ_LPVDDPD_VREGOBPD;
}

void LP_SIMOVregDPowerDown(void)
{
    MXC_PWRSEQ->lpvddpd |= MXC_F_PWRSEQ_LPVDDPD_VREGODPD;
}

void LP_SIMOVregDPowerUp(void)
{
    MXC_PWRSEQ->lpvddpd &= ~MXC_F_PWRSEQ_LPVDDPD_VREGODPD;
}

void LP_FastWakeupEnable(void) 
{
    MXC_PWRSEQ->lpcn |= MXC_F_PWRSEQ_LPCN_FWKM;
}

void LP_FastWakeupDisable(void) 
{
    MXC_PWRSEQ->lpcn &= ~MXC_F_PWRSEQ_LPCN_FWKM;
}

void LP_SetRAMRetention(ram_retained_t ramRetained) 
{
    MXC_SETFIELD(MXC_PWRSEQ->lpcn, MXC_F_PWRSEQ_LPCN_RAMRET, ramRetained);
}

void LP_EnterSleepMode(void)
{
    LP_ClearWakeStatus();
    
    /* Clear SLEEPDEEP bit */
    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
    
    /* Go into Sleep mode and wait for an interrupt to wake the processor */
    __WFI();
}

void LP_EnterDeepSleepMode(void)
{
    LP_ClearWakeStatus();

    /* Set SLEEPDEEP bit */
    MXC_PWRSEQ->lpcn &= ~MXC_F_PWRSEQ_LPCN_BCKGRND;
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    /* Go into Deepsleep mode and wait for an interrupt to wake the processor */
    __WFI();
}

void LP_EnterBackgroundMode(void)
{
    LP_ClearWakeStatus();

    /* Set BACKGROUND bit and SLEEPDEEP bit */
    MXC_PWRSEQ->lpcn |= MXC_F_PWRSEQ_LPCN_BCKGRND;
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    /* Go into Background mode and wait for an interrupt to wake the processor */
    __WFI();
}

void LP_EnterBackupMode(void* func(void))
{
    LP_ClearWakeStatus();

    MXC_PWRSEQ->buretvec = (uint32_t)(&Backup_Handler) | 1;
    if(func == NULL) {
        MXC_PWRSEQ->buaod = (uint32_t)(&Reset_Handler) | 1;
    } else {
        MXC_PWRSEQ->buaod = (uint32_t)(&func) | 1;
    }

    // Enable the VDDCSW to ensure we have enough power to start
    MXC_MCR->ctrl |= MXC_F_MCR_CTRL_VDDCSWEN;

    // Enable backup mode
    MXC_GCR->pm &= ~MXC_F_GCR_PM_MODE;
    MXC_GCR->pm |= MXC_S_GCR_PM_MODE_BACKUP;
    while(1); // Should never reach this line - device will jump to backup vector on exit from background mode.
}

void LP_EnterShutdownMode(void)
{
    MXC_GCR->pm &= ~MXC_F_GCR_PM_MODE;
    MXC_GCR->pm |= MXC_S_GCR_PM_MODE_SHUTDOWN;
    while (1); // Should never reach this line - device will reset on exit from shutdown mode.
}
