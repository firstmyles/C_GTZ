/*
 *  ======== gtz.c ========
 */    

#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "gtz.h"

void clk_SWI_Generate_DTMF(UArg arg0);
void clk_SWI_GTZ_0697Hz(UArg arg0);

extern void task0_dtmfGen(void);
extern void task1_dtmfDetect(void);

extern char digit;
extern int sample, mag1, mag2, freq1, freq2, gtz_out[8];
extern short coef[8];



/*
 *  ======== main ========
 */
void main(void)
{


	System_printf("\n I am in main :\n");
	System_flush();
	/* Create a Clock Instance */
    Clock_Params clkParams;

    /* Initialise Clock Instance with time period and timeout (system ticks) */
    Clock_Params_init(&clkParams);
    clkParams.period = 1;
    clkParams.startFlag = TRUE;

    /* Instantiate ISR for tone generation  */
	Clock_create(clk_SWI_Generate_DTMF, TIMEOUT, &clkParams, NULL);

    /* Instantiate 8 parallel ISRs for each of the eight Goertzel coefficients */
	Clock_create(clk_SWI_GTZ_0697Hz, TIMEOUT, &clkParams, NULL);

	mag1 = 32768.0; mag2 = 32768.0; freq1 = 697; // I am setting freq1 = 697Hz to test my GTZ algorithm with one frequency.

	/* Start SYS_BIOS */
    BIOS_start();
}

/*
 *  ====== clk0Fxn =====
 *  Dual-Tone Generation
 *  ====================
 */
void clk_SWI_Generate_DTMF(UArg arg0)
{
	static int tick;


	tick = Clock_getTicks();

//	sample = (int) 32768.0*sin(2.0*PI*freq1*TICK_PERIOD*tick) + 32768.0*sin(2.0*PI*freq2*TICK_PERIOD*tick);
	sample = (int) 32768.0*sin(2.0*PI*freq1*TICK_PERIOD*tick) + 32768.0*sin(2.0*PI*0*TICK_PERIOD*tick);
	sample = sample >>12;
}

/*
 *  ====== clk_SWI_GTZ =====
 *  gtz sweep
 *  ====================
 */
void clk_SWI_GTZ_0697Hz(UArg arg0)
{
  // 	static int N = 0;
  // 	static int Goertzel_Value = 0;

   	//static short delay;
  // 	static short delay_1 = 0;
  // 	static short delay_2 = 0;

  // 	int prod1, prod2, prod3;

   //	short input, coef_1;


// to be completed


    //	gtz_out[0] = Goertzel_Value;

    static int n = 0;
    static int32_t Goertzel_Value = 0;
  //  static int loop_counter = 0;
  //  int task1_loop_counter = 0;
    static int16_t delay;
    static int16_t delay_1 = 0;
    static int16_t delay_2 = 0;

    int32_t prod1, prod2, prod3;

    int16_t input, coef_1;

    	    // Calculate the Goertzel coefficient for the target frequency
   // float omega = 2 * PI * 697 / SAMPLING_RATE;
    int16_t coef = 2* 0x6D02    ;//(int16_t)(32768.0 * 2.0 * cos(omega));

    	    // Get the input sample
    input = (int16_t)sample;

    	    // Implement the Goertzel algorithm
    delay = input + ((coef * delay_1) >> 14) - delay_2;
    delay_2 = delay_1;
    delay_1 = delay;

    	    // Increment the sample count
    n++;

    	    // After processing N_SAMPLES, calculate the Goertzel value (magnitude squared)
	if (n == 206)
	{
		prod1 = (int32_t)delay_1 * (int32_t)delay_1;
		prod2 = (int32_t)delay_2 * (int32_t)delay_2;
		prod3 = (int32_t)coef * (int32_t)delay_1 * (int32_t)delay_2;

		Goertzel_Value = (prod1 + prod2 - (prod3 >> 14)) >> 8;

		// Reset the variables for the next run
		n = 0;
		delay_1 = 0;
		delay_2 = 0;

		//loop_counter++;


	//	if (loop_counter >= 10)
			   // {
			        // Stop the clock instance
			//       Clock_stop((Clock_Handle)arg0);
			//    }
		}



	// Store the Goertzel value in the gtz_out array at index 0
	gtz_out[0] = Goertzel_Value;
	//loop_counter++;
	//if (loop_counter >= 10)
	  //  {
	    //    // Stop the clock instance
	      //  Clock_stop((Clock_Handle)arg0);
	    //}
}

