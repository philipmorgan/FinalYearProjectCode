#include "mbed.h" 	// include the mbed classes, structures and groups.
int x,y; 			// initialise integers used for 16-bit pass through.
float x1,y1;		// initialise floats for pass through in range 0 - 1.0.

AnalogIn Input(A0); // pin A0 on nucleo L152 & F303K8
AnalogOut Output(PA_4); // pin A2 on nucleo L152 & F303K8

AnalogIn Input1(A1); // pin A1 on nucleo L152. pin A1 on F303k8
AnalogOut Output1(PA_5); // pin D13 on nucleo L152. pin A3 on F303k8

// main function
int main()
{
  while(1)
  {  
  	x = Input.read_u16(); 	// input read from pin A0 as 16 bit integer in range 0 - 65535
    y = x;					// let y = the 16-bit input x.
    Output.write_u16(y);	// write 16-bit output y.
    
    x1 = Input1.read();		// input read from pin A1 as float in range 0 - 1.0.
    y1 = x1;				// let y1 = float input x1.
    Output1.write(y1);	 	// write float output y1.
  }
}

