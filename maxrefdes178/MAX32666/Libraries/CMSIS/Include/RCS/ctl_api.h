//$(LICENSE_COMMENT)

#ifndef CrossWorks_Target_Library
#define CrossWorks_Target_Library

#ifdef CTL_TASKING
#include "ctl.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*CTL_ISR_FN_t)(void);

#if defined(_lint)

// Don't get lint confused.
int ctl_global_interrupts_disable(void);
void ctl_global_interrupts_enable(void);
void ctl_enter_isr(void);
void ctl_exit_isr(void);

#elif defined(__CROSSWORKS_ARM)

#if defined(__ARM_ARCH_7EM__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_6M__)

#ifdef CTL_TASKING

#define SCB_ICSR (*(volatile unsigned *)0xE000ED04)
#define SCB_SHPR(N) (*(volatile unsigned char*)(0xE000ED18+N))
#define NVIC_IPR(N) (*(volatile unsigned char*)(0xE000E400+N))

// Must be 'called' at the start of an interrupt handler that uses CTL services
static inline void __attribute__((always_inline))
ctl_enter_isr(void)
{
  ctl_global_interrupts_disable();
#if (defined(__ARM_ARCH_7EM__) || defined(__ARM_ARCH_7M__)) && !defined(CTL_TASKING_NO_ISR_PRIORITY_CHECK)
  {
    unsigned vectactive = (SCB_ICSR & 0xff);
    unsigned char priority;
    if (vectactive > 15)
      priority = NVIC_IPR(vectactive-16);
    else
      priority = SCB_SHPR(vectactive-4);
    if ((priority & 0x80)==0)
      ctl_handle_error(CTL_UNSPECIFIED_ERROR);
  }
#endif
  ctl_interrupt_count++;
  ctl_global_interrupts_enable();
}

// Must be 'called' at the end of an interrupt handler that uses CTL services
static inline void __attribute__((always_inline))
ctl_exit_isr(void)
{
  ctl_global_interrupts_disable();
  ctl_interrupt_count--;
  if (ctl_interrupt_count == 0 && ctl_reschedule_on_last_isr_exit)
    {
      ctl_reschedule_on_last_isr_exit = 0;
      SCB_ICSR = (1<<28);
    }
  ctl_global_interrupts_enable();
}

#undef SCB_ICSR
#undef SCB_SHPR
#undef NVIC_IPR

#else

#define ctl_global_interrupts_disable() __asm volatile("cpsid i")
#define ctl_global_interrupts_enable() __asm volatile("cpsie i") 
#define ctl_enter_isr() 
#define ctl_exit_isr()

#endif

// Install a new interrupt handler
int ctl_set_isr(unsigned int irq,
                unsigned int priority,
                CTL_ISR_FN_t isr, 
                CTL_ISR_FN_t *oldisr);

#else 

#ifndef CTL_TASKING

#include "libarm.h"
#define ctl_global_interrupts_enable() libarm_set_irq(1)
#define ctl_global_interrupts_disable() libarm_set_irq(0)

#endif

// Deprecated Enumeration of interrupt trigger types
typedef enum
{
  CTL_ISR_TRIGGER_FIXED,
  CTL_ISR_TRIGGER_LOW_LEVEL,
  CTL_ISR_TRIGGER_HIGH_LEVEL,
  CTL_ISR_TRIGGER_NEGATIVE_EDGE,
  CTL_ISR_TRIGGER_POSITIVE_EDGE,
  CTL_ISR_TRIGGER_DUAL_EDGE
} 
CTL_ISR_TRIGGER_t;

// Install a new interrupt handler.
int ctl_set_isr(unsigned int vector, 
                unsigned int priority, 
                CTL_ISR_TRIGGER_t trigger, 
                CTL_ISR_FN_t isr, 
                CTL_ISR_FN_t *oldisr);

#endif

// Set the priority of an interrupt handler
void ctl_set_priority(int irq, int priority);

// Unmask an interrupt handler
int ctl_unmask_isr(unsigned int irq);

// Mask an interrupt handler
int ctl_mask_isr(unsigned int irq);

// Return the lowest available interrupt handler priority
int ctl_lowest_isr_priority(void);

// Return the highest available interrupt handler priority
int ctl_highest_isr_priority(void);

// Adjust the interrupt handler priority by n
int ctl_adjust_isr_priority(int priority, int n);

#endif

// Start the timer ticking - which will call timerFn on timeout
// For example ctl_start_timer(ctl_increment_tick_from_isr);
void ctl_start_timer(CTL_ISR_FN_t timerFn);

// return the number of timer ticks per second usually 1000
unsigned long ctl_get_ticks_per_second(void);

// Initialise the board
void ctl_board_init(void);

// Set/Clear the leds on the board
void ctl_board_set_leds(unsigned set);

// Set a function to be called when a button is pressed
void ctl_board_on_button_pressed(CTL_ISR_FN_t buttonFn);

// Sleep for delay ticks
void ctl_sleep(unsigned delay);

#ifdef __cplusplus
}
#endif

#endif
