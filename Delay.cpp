// the below code uses a circular buffer to implement the delay. the start of the buffer
// is filled with the input until the buffer is full to the length determined by the
// "delay" variable. when the first index of the buffer comes around again the data in that
// index is multiplied by a gain that is less than 1 in order to reduce the gain of the
// data and the current input data is also added to the buffer.
// the old data in any given buffer index will eventually be inaudible as the buffer is
// filled up with new data.
// the below link features original code which was modified for my implementation of the
// delay:
// https://developer.mbed.org/users/vsergeev/code/echo_effect/file/97643e5b2d5e/main.cpp

#include "mbed.h"	// include the mbed classes, structures and groups.
#define BUFFER_MAX   20000 // the maximum buffer size is defined. the buffer size is restricted by the 
#define BUFFER_MIN   50
 
AnalogIn analogValue(A0);   // analog input to pin pin A0
AnalogOut myOutput(PA_4);  	// analog output from nucleo pin A2
AnalogIn delay_time(A1);    // control length of delay with pot. linear pot used.
AnalogIn repeats(A3);     	// pot controls gain of repeats from analog input A3.
void ReadPotsTask(void);

int buffer[BUFFER_MAX]; // buffer is initialised to the BUFFER_MAX length
float gain;
int delay = BUFFER_MAX;   // delay length is initialised to the maximum buffer size.
    
int main() 
{
    int i;
    for (i = 0; i < delay; i++)     // the first samples of the buffer are filled up with the with the input signal
        buffer[i] = analogValue.read_u16();
    while(1)
    {
        buffer[i] = buffer[i]*gain + analogValue.read_u16();
        myOutput.write_u16(buffer[i]/2); // 16-bit output is written
        i = (i+1) % delay; // i is equal to the remainder of i+1 divided by the delay length. 
						// when the buffer reaches the end it returns to the start of the buffer.
						
		gain = repeats;                 // previously initialised float gain reads the repeats input and  scales from 0.0 - 1.0.
    	delay = delay_time*BUFFER_MAX;  // the delay_time input is a float in the range of 0.0 - 1.0 and the max buffer is multiplied by this float.
    	
    	if (delay < BUFFER_MIN)	// the buffer size is limited to the minimum. 
    	{					    // if the buffer becomes too small there will be no 
        	delay = BUFFER_MIN;	// delay and the output will just be the input.
		}
    }
}