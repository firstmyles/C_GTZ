/*
 *  ======== util.c ========
 */

#include <xdc/std.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "gtz.h"

int tdiff,tdiff_final;

int sample, gtz_out[8];
int flag = 0;

short coef[8] =
		{ 0x6D02, 0x68AD, 0x63FC, 0x5EE7, 0x4A70, 0x4090, 0x3290, 0x23CE }; // goertzel coefficients
void task1_dtmfDetect();
void task2_dtmfGenerate(char* keys);
extern short* buffer;

void task1_dtmfDetect() {
	int i, n;
	int row, col;
	int max_row, max_col;
	char pad[4][4] = {{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','D'}};
	char result[8];

	for(n=0;n<8;n++) {
		while (!flag) Task_sleep(210);
		/* TODO 3. Complete code to detect the 8 digits based on the GTZ output */
		/* ========================= */
		max_row = gtz_out[0];
		        max_col = gtz_out[4];
		        row = 0;
		        col = 0;

		        for (i = 1; i < 4; i++) {
		            if (gtz_out[i] > max_row) {
		                max_row = gtz_out[i];
		                row = i;
		            }
		            if (gtz_out[i+4] > max_col) {
		                max_col = gtz_out[i+4];
		                col = i;
		            }
		        }
		result[n] = pad[row][col];
		/* ========================= */
		printf("%c\n", result[n]);
		flag = 0;
	}
	printf("\nDetection finished\n");
	printf("Generating audio\n");
	task2_dtmfGenerate(result);
	printf("Finished\n");
}

void task2_dtmfGenerate(char* keys)
{
	int fs = 10000;
	float tone_length = 0.5;
	int n_tones = 8;
	int samples_per_tone = (int) (tone_length * fs);
	int samples_total = samples_per_tone * n_tones;
	int i, n;
	char digit;

	float row_freqs[4] = {697, 770, 852, 941};
	float col_freqs[4] = {1209, 1336, 1477, 1633};
	char pad[4][4] = {{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','D'}};


	for(n=0;n<n_tones;n++) {
		digit = keys[n];
		int row, col;

		        for (row = 0; row < 4; row++) {
		            for (col = 0; col < 4; col++) {
		                if (pad[row][col] == digit) {
		                    break;
		                }
		            }
		            if (col < 4) {
		                break;
		            }
		        }

		        float row_freq = row_freqs[row];
		        float col_freq = col_freqs[col];

		        for (i = 0; i < samples_per_tone; i++) {
		            float t = (float)i / fs;
		            float row_val = sin(2.0 * PI * row_freq * t);
		            float col_val = sin(2.0 * PI * col_freq * t);
		            buffer[n * samples_per_tone + i] = (short)((row_val + col_val) * 0.5 * 32767);
		        }
	}

	/* Writing the data to a wav file */
	FILE* fp = fopen("../answer.wav", "wb");
	int datasize = samples_total*2;
	int filesize = 36+datasize;
	int headersize = 16;
	int datatype = 1;
	int nchannel = 1;
	int byterate = fs*2;
	int align = 2;
	int bitpersample = 16;

	fwrite("RIFF", 1, 4, fp);
	fwrite(&filesize, 4, 1, fp);
	fwrite("WAVE", 1, 4, fp);
	fwrite("fmt ", 1, 4, fp);
	fwrite(&headersize, 4, 1, fp);
	fwrite(&datatype, 2, 1, fp);
	fwrite(&nchannel, 2, 1, fp);
	fwrite(&fs, 4, 1, fp);
	fwrite(&byterate, 4, 1, fp);
	fwrite(&align, 2, 1, fp);
	fwrite(&bitpersample, 2, 1, fp);
	fwrite("data", 1, 4, fp);
	fwrite(&datasize, 4, 1, fp);
	fwrite(buffer, 2, samples_total, fp);
	fclose(fp);
}
