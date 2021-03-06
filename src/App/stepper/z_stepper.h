/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * stepper.h - stepper motor driver: executes motion plans of zPlanner.c using the stepper motors
 * Derived from Grbl
 *
 * Copyright (c) 2009-2011 Simen Svale Skogsrud
 *
 * Grbl is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Grbl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Grbl.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "main.h"
#include "tim.h"
#include "z_planner.h"


#define ISR_PULSE_CONTROL	1
#define ISR_MULTI_STEPS		1

#define DISABLE_MULTI_STEPPING 1

#define MINIMUM_STEPPER_PULSE			1
#define MAXIMUM_STEPPER_RATE			100000
#define MINIMUM_STEPPER_POST_DIR_DELAY	200
#define MINIMUM_STEPPER_PRE_DIR_DELAY	MINIMUM_STEPPER_POST_DIR_DELAY
#define STEPPER_TIMER_TICKS_PER_US		((STEPPER_TIMER_RATE) / 1000000) // stepper timer ticks per µs
#if MINIMUM_STEPPER_PULSE && MAXIMUM_STEPPER_RATE
  constexpr uint32_t _MIN_STEP_PERIOD_NS = 1000000000UL / MAXIMUM_STEPPER_RATE;
  constexpr uint32_t _MIN_PULSE_HIGH_NS = 1000UL * MINIMUM_STEPPER_PULSE;
  constexpr uint32_t _MIN_PULSE_LOW_NS = _MAX((_MIN_STEP_PERIOD_NS - _MIN(_MIN_STEP_PERIOD_NS, _MIN_PULSE_HIGH_NS)), _MIN_PULSE_HIGH_NS);
#elif MINIMUM_STEPPER_PULSE
  // Assume 50% duty cycle
  constexpr uint32_t _MIN_PULSE_HIGH_NS = 1000UL * MINIMUM_STEPPER_PULSE;
  constexpr uint32_t _MIN_PULSE_LOW_NS = _MIN_PULSE_HIGH_NS;
#elif MAXIMUM_STEPPER_RATE
  // Assume 50% duty cycle
  constexpr uint32_t _MIN_PULSE_HIGH_NS = 500000000UL / MAXIMUM_STEPPER_RATE;
  constexpr uint32_t _MIN_PULSE_LOW_NS = _MIN_PULSE_HIGH_NS;
#else
  #error "Expected at least one of MINIMUM_STEPPER_PULSE or MAXIMUM_STEPPER_RATE to be defined"
#endif



#define PENDING(NOW,SOON) ((int32_t)(NOW-(SOON))<0)
/*
__INLINE static void enableCycleCounter()
{
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

  #if __CORTEX_M == 7
	DWT->LAR = 0xC5ACCE55; // Unlock DWT on the M7
  #endif

  DWT->CYCCNT = 0;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

__INLINE static uint32_t getCycleCount()
{
	return DWT->CYCCNT;
}

__INLINE static void DELAY_CYCLES(const uint32_t x)
{
  const uint32_t endCycles = getCycleCount() + x;
  while (PENDING(getCycleCount(), endCycles)) {}
}
*/
	 
    #define nop() asm volatile("nop;\n\t":::)

    __INLINE static void __delay_4cycles(uint32_t cy)
	{ // +1 cycle
      asm volatile(
		"beg:\n"
        "subs %0,#1\n"
        "nop\n"
        "bne beg"
        : "+r"(cy)   // output: +r means input+output
        :                 // input:
        : "cc"            // clobbers:
      );
    }

    // Delay in cycles
    __INLINE static void DELAY_CYCLES(uint32_t x)
	{

        #define MAXNOPS 4

        if (x <= (MAXNOPS)) {
          switch (x) { case 4: nop(); case 3: nop(); case 2: nop(); case 1: nop(); }
        }
        else { // because of +1 cycle inside delay_4cycles
          const uint32_t rem = (x - 1) % (MAXNOPS);
          switch (rem) { case 3: nop(); case 2: nop(); case 1: nop(); }
          if ((x = (x - 1) / (MAXNOPS)))
            __delay_4cycles(x); // if need more then 4 nop loop is more optimal
        }
        #undef MAXNOPS
    }
    #undef nop

__INLINE static uint32_t MultiU32X24toH32(uint32_t longIn1, uint32_t longIn2)
{
  return ((uint64_t)longIn1 * longIn2 + 0x00800000) >> 24;
}


#define ENABLE_STEPPER_DRIVER_INTERRUPT()   HAL_TIM_Base_Start_IT(&hStepperTim)
#define DISABLE_STEPPER_DRIVER_INTERRUPT() 	HAL_TIM_Base_Stop_IT(&hStepperTim)
#define STEPPER_ISR_ENABLED()				(NVIC->ISER[(uint32_t)((int32_t)TIM2_IRQn) >> 5] & (uint32_t)(1 << ((uint32_t)((int32_t)TIM2_IRQn) & (uint32_t)0x1F)))
#define DELAY_NS(x)							DELAY_CYCLES( (x) * (F_CPU / 1000000UL) / 1000UL )




#ifdef __cplusplus
extern "C"
{
#endif
void IsrStepperHandler();
#ifdef __cplusplus
}
#endif

class Stepper;
extern Stepper		zStepper;



// Disable multiple steps per ISR
//#define DISABLE_MULTI_STEPPING

//
// Estimate the amount of time the Stepper ISR will take to execute
//

/**
 * The method of calculating these cycle-constants is unclear.
 * Most of them are no longer used directly for pulse timing, and exist
 * only to estimate a maximum step rate based on the user's configuration.
 * As 32-bit processors continue to diverge, maintaining cycle counts
 * will become increasingly difficult and error-prone.
 */

  /**
   * Duration of START_TIMED_PULSE
   *
   * ...as measured on an LPC1768 with a scope and converted to cycles.
   * Not applicable to other 32-bit processors, but as long as others
   * take longer, pulses will be longer. For example the SKR Pro
   * (stm32f407zgt6) requires ~60 cyles.
   */
  #define TIMER_READ_ADD_AND_STORE_CYCLES 60UL

  // The base ISR takes 792 cycles
  #define ISR_BASE_CYCLES  792UL

  // Linear advance base time is 64 cycles
  // S curve interpolation adds 40 cycles
  #ifdef S_CURVE_ACCELERATION
    #define ISR_S_CURVE_CYCLES 40UL
  #else
    #define ISR_S_CURVE_CYCLES 0UL
  #endif

  // Stepper Loop base cycles
  #define ISR_LOOP_BASE_CYCLES 4UL

  // To start the step pulse, in the worst case takes
  #define ISR_START_STEPPER_CYCLES 13UL

  // And each stepper (start + stop pulse) takes in worst case
  #define ISR_STEPPER_CYCLES 16UL

// Add time for each stepper
#define ISR_Z_STEPPER_CYCLES       ISR_STEPPER_CYCLES

// If linear advance is disabled, the loop also handles them
#define ISR_MIXING_STEPPER_CYCLES  0UL

// And the total minimum loop time, not including the base
#define MIN_ISR_LOOP_CYCLES (ISR_Z_STEPPER_CYCLES + ISR_MIXING_STEPPER_CYCLES)

// Calculate the minimum MPU cycles needed per pulse to enforce, limited to the max stepper rate
#define _MIN_STEPPER_PULSE_CYCLES(N) _MAX(uint32_t((F_CPU) / (MAXIMUM_STEPPER_RATE)), ((F_CPU) / 500000UL) * (N))
#define MIN_STEPPER_PULSE_CYCLES _MIN_STEPPER_PULSE_CYCLES(1UL)

// But the user could be enforcing a minimum time, so the loop time is
#define ISR_LOOP_CYCLES (ISR_LOOP_BASE_CYCLES + _MAX(MIN_STEPPER_PULSE_CYCLES, MIN_ISR_LOOP_CYCLES))

// Now estimate the total ISR execution time in cycles given a step per ISR multiplier
#define ISR_EXECUTION_CYCLES(R) (((ISR_BASE_CYCLES + ISR_S_CURVE_CYCLES + (ISR_LOOP_CYCLES) * (R))) / (R))

// The maximum allowable stepping frequency when doing x128-x1 stepping (in Hz)
#define MAX_STEP_ISR_FREQUENCY_128X ((F_CPU) / ISR_EXECUTION_CYCLES(128))
#define MAX_STEP_ISR_FREQUENCY_64X  ((F_CPU) / ISR_EXECUTION_CYCLES(64))
#define MAX_STEP_ISR_FREQUENCY_32X  ((F_CPU) / ISR_EXECUTION_CYCLES(32))
#define MAX_STEP_ISR_FREQUENCY_16X  ((F_CPU) / ISR_EXECUTION_CYCLES(16))
#define MAX_STEP_ISR_FREQUENCY_8X   ((F_CPU) / ISR_EXECUTION_CYCLES(8))
#define MAX_STEP_ISR_FREQUENCY_4X   ((F_CPU) / ISR_EXECUTION_CYCLES(4))
#define MAX_STEP_ISR_FREQUENCY_2X   ((F_CPU) / ISR_EXECUTION_CYCLES(2))
#define MAX_STEP_ISR_FREQUENCY_1X   ((F_CPU) / ISR_EXECUTION_CYCLES(1))

//
// Stepper class definition
//
class Stepper {

  public:


  private:

    static block_t* current_block;          // A pointer to the block currently being traced

    static uint8_t last_direction_bits,     // The next stepping-bits to be output
                   axis_did_move;           // Last Movement in the given direction is not null, as computed when the last movement was fetched from planner

    static bool abort_current_block;        // Signals to the stepper that current block should be aborted

    static uint32_t acceleration_time, deceleration_time; // time measured in Stepper Timer ticks
    static uint8_t steps_per_isr;         // Count of steps to perform per Stepper ISR call


    // Delta error variables for the Bresenham line tracer
    static int32_t delta_error;
    static uint32_t advance_dividend;
    static uint32_t advance_divisor,
                    step_events_completed,  // The number of step events executed in the current block
                    accelerate_until,       // The point from where we need to stop acceleration
                    decelerate_after,       // The point from where we need to start decelerating
                    step_event_count;       // The total event count for the current block

    #ifdef S_CURVE_ACCELERATION
      static int32_t bezier_A,     // A coefficient in Bézier speed curve
                     bezier_B,     // B coefficient in Bézier speed curve
                     bezier_C;     // C coefficient in Bézier speed curve
      static uint32_t bezier_F,    // F coefficient in Bézier speed curve
                      bezier_AV;   // AV coefficient in Bézier speed curve
      static bool bezier_2nd_half; // If Bézier curve has been initialized or not
    #endif

    static int32_t ticks_nominal;
    #ifndef S_CURVE_ACCELERATION
      static uint32_t acc_step_rate; // needed for deceleration start point
    #endif

    // Exact steps at which an endstop was triggered
    static int32_t endstops_trigsteps;

    // Positions of stepper motors, in step units
    static int32_t count_position;

    // Current stepper motor directions (+1 or -1)
    static int8_t count_direction;

  public:
    // Initialize stepper hardware
    static void init();

    // Interrupt Service Routine and phases

    // The stepper subsystem goes to sleep when it runs out of things to execute.
    // Call this to notify the subsystem that it is time to go to work.
    static inline void wake_up() { ENABLE_STEPPER_DRIVER_INTERRUPT(); }

    static inline bool is_awake() { return STEPPER_ISR_ENABLED(); }

    static inline bool suspend() {
      const bool awake = is_awake();
      if (awake) DISABLE_STEPPER_DRIVER_INTERRUPT();
      return awake;
    }

    // The ISR scheduler
    static void StepperHandler();
    static void isr();

    // The stepper pulse ISR phase
    static void pulse_phase_isr();

    // The stepper block processing ISR phase
    static uint32_t block_phase_isr();

    // Check if the given block is busy or not - Must not be called from ISR contexts
    static bool is_block_busy(const block_t* const block);

    // Get the position of a stepper, in steps
    static int32_t position();

    // Set the current position in steps
    static void set_position(const int32_t c);
    static void set_axis_position(const int32_t v);

    // Discard current block and free any resources
    __INLINE static void discard_current_block()
   {
      current_block = nullptr;
      axis_did_move = 0;
      zPlanner.release_current_block();
    }

    // Quickly stop all steppers
    __INLINE static void quick_stop() { abort_current_block = true; }

    // The direction of a single motor
    __INLINE static bool motor_direction() { return last_direction_bits; }

    // The last movement direction was not null on the specified axis. Note that motor direction is not necessarily the same.
    __INLINE static bool axis_is_moving() { return axis_did_move; }


    // Handle a triggered endstop
    static void endstop_triggered();

    // Triggered position of an axis in steps
    static int32_t triggered_position();

    // Set direction bits for all steppers
    static void set_directions();

  private:

    // Set the current position in steps
    static void _set_position(const int32_t c);

    __INLINE static uint32_t calc_timer_interval(uint32_t step_rate, uint8_t* loops) {
      uint32_t timer;


      uint8_t multistep = 1;
      #ifndef DISABLE_MULTI_STEPPING

        // The stepping frequency limits for each multistepping rate
        static const uint32_t limit[] = {
          (  MAX_STEP_ISR_FREQUENCY_1X     ),
          (  MAX_STEP_ISR_FREQUENCY_2X >> 1),
          (  MAX_STEP_ISR_FREQUENCY_4X >> 2),
          (  MAX_STEP_ISR_FREQUENCY_8X >> 3),
          ( MAX_STEP_ISR_FREQUENCY_16X >> 4),
          ( MAX_STEP_ISR_FREQUENCY_32X >> 5),
          ( MAX_STEP_ISR_FREQUENCY_64X >> 6),
          (MAX_STEP_ISR_FREQUENCY_128X >> 7)
        };

        // Select the proper multistepping
        uint8_t idx = 5;
        while (idx < 7 && step_rate > limit[idx]) {
          step_rate >>= 1;
          multistep <<= 1;
          ++idx;
        };
      #else
        NOMORE(step_rate, uint32_t(MAX_STEP_ISR_FREQUENCY_1X));
      #endif
      *loops = multistep;

        // In case of high-performance processor, it is able to calculate in real-time
        timer = uint32_t(STEPPER_TIMER_RATE) / step_rate;

      return timer;
    }

    #ifdef S_CURVE_ACCELERATION
      static void _calc_bezier_curve_coeffs(const int32_t v0, const int32_t v1, const uint32_t av);
      static int32_t _eval_bezier_curve(const uint32_t curr_step);
    #endif


};
