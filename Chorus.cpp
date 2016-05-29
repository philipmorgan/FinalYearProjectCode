// the following code uses the previously created sine wave to modulate the length 
// of the buffer in the previous delay code. the sound of this effect relies on a short
// delay which is modulated. delayed signal goes out of phase with the input and mixing
// the 2 signals together will cause audible frequency modulation.

#include "mbed.h" // include the mbed classes, structures and groups.
#define BUFFER_MAX   750 // define buffer maximum size
#define BUFFER_MIN   50 // define buffer minimum size

float x,x1,y,y1,z,z1; // initialise floats
int n = 0;
int n1 = 0;
 
AnalogIn analogValue(A0);           // analog input from guitar & signal buffer.
AnalogOut myOutput(PA_4);           // analog output from nucleo
AnalogIn delay_time(A1);            // control length of delay with pot. linear pot used.
AnalogIn repeats(A3);               // control gain of repeats (how many repeats there will be). linear pot used. 

Ticker tick; // ticker class is initialised. this ticker will generate the sine wave required to modulate the buffer length.

void ReadPotsTask(void);
int buffer[BUFFER_MAX]; 	// initialise the buffer to the maximum buffer length.
float gain; 				// this float will be used to read the input from the gain pot.
int delay = BUFFER_MAX;     // delay length is initialised to the maximum buffer size.
  
void s1hz_task(void);
const float wave[1000]={}; 	// sine wave wavetable omitted to save space.
							// copy wavetable from above sine wave code.
int main() 
{
    int i;
    for (i = 0; i < z1; i++)     // the length of the buffer is modulated by the generated sine wave.
        buffer[i] = analogValue.read_u16(); // the first samples of the buffer are filled up with the input signal
    while(1)
    {
        s1hz_tick.attach_us(&s1hz_task,500);
        buffer[i] = buffer[i]*(1-gain) + (analogValue.read_u16()*(gain-1)); // previous sample is multiplied by the gain and added to the current input signal.
        myOutput.write_u16(buffer[i]/3); // the current buffer sample is written to the analog output
        i = (i+1) % z1;          // i is equal to the remainder of i+1 divided by the delay length. the length of the delay variable is the length of the buffer that is utilised.
        if (i == 0)                 // if i is equal to 0 read the inputs from the pots.
        ReadPotsTask();
    }
}

void ReadPots(void)
    {
    gain = repeats;                 // previously initialised float gain reads the repeats input and  scales from 0.0 - 1.0.
    delay = delay_time*BUFFER_MAX;  // the delay_time input is a float in the range of 0.0 - 1.0 and the max buffer is multiplied by this float.
    if (delay < BUFFER_MIN)			// the buffer is limited. if the length of the buffer goes below the BUFFER_MIN level it is set to the buffer minimum.
        delay = BUFFER_MIN;
    }

void s1hz_task(void)
{
    y1 = (wave[n1]); 	// every time the function is called the wavetable is incremented
    n1++;				// the current value of the wavetable is divided by 65535 in
    z1 = (y1/65535);	// order to get the output in the range of 0 - 1.0. this value
    if (n1 == 1000)		// is then multiplied by the value of the depth input from the
        {				// pot which will also be in the range of 0 - 1.0.
            n1 = 0;
        }
}