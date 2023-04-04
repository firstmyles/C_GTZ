/*
 *  ======== util.c ========
 */

#include <xdc/std.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include <stdbool.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "gtz.h"

UInt32 time1, time2;




char digit;

int freq1, freq2;
int mag1, mag2;

int sample, gtz_out[8];

short coef[8] = {0x6D02, 0x68AD, 0x63FC, 0x5EE7, 0x4A70, 0x4090, 0x3290, 0x23CE}; // Goertzel coefficients
int tone[8] = {697, 770, 852, 941, 1209, 1336, 1477, 1633}; // Tones used in DTMF

int input;

void task0_dtmfGen(void)
{



// This task is used to read the symbol from the keyboard and set freq1 and freq2

	System_printf("\n I am in Task 0:\n");
	System_flush();
	scanf(" %c", &digit);

		if (digit == '1')
		{   freq1 = 697;
			freq2 = 1209;
		}
		else if (digit == '2')
		{	freq1 = 697;
			freq2 = 1336;
		}
		else if (digit=='3')
		{	freq1 = 697;
			freq2 = 1477;
		}
		else if (digit=='4')
		{	freq1 = 770;
			freq2 = 1209;
		}
		else if (digit=='5')
		{	freq1 = 770;
			freq2 = 1336;
		}
		else if (digit=='6')
		{	freq1 = 770;
			freq2 = 1477;
		}
		else if (digit=='7')
		{	freq1 = 852;
			freq2 = 1209;
		}
		else if (digit=='8')
		{	freq1 = 852;
			freq2 = 1336;
		}
		else if (digit=='9')
		{	freq1 = 852;
			freq2 = 1477;
		}




	Task_sleep(500); // to allow the GTZ to be calculated.


}

void task1_dtmfDetect(void)
{
	int i, a1=0, a2=0, f1=0, f2=0, dig;
	bool freqDetected = FALSE;



	System_printf("\n I am in Task 1 for the first time, please wait:\n");






	System_flush();
	//static int tick2;


	//tick2 = Clock_getTicks();



	while(1) {





		Task_sleep(2000);  /* 0.25 second for 125us tick */


		System_printf("\n The GTZ is %d\n", gtz_out[0]);

		if (gtz_out[0] > 1000)
		{ freqDetected = TRUE;
		}



		System_printf(" I am leaving Task 1, please wait for a minute or so to get the next GTZ:\n");
		System_flush();


		a1=0, a2=0, f1=0, f2=0;






}
	if(freqDetected == TRUE)
	{ System_printf(" Frequency was Detected in tone\n");
	  System_flush();
	}
	else
	{ System_printf(" Frequency was not detected in tone\n");
	  System_flush();
	}
return;
}

