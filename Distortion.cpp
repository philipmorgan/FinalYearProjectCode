// the below carries out simple symmetrical clipping on the input signal and filters
// this signal with an IIR high pass filter.
// the input is centred in order to allow for equal clipping levels above and below
// the centre input point.
// when the clipping is complete the signal is filtered. the level of the clipping is
// determined by the gain pot connected to analog input A1.
// the level of the filtered signal is determined by the filter pot connected to analog
// input A3.

#include "mbed.h" // include the mbed classes, structures and groups.

float x,y,xMid,yOut; // variables associated with input and output.

float x1,x2,x3; // IIR filter x variables x(n-1), x(n-2) and x(n-3).
float y1,y2,y3; // IIR filter x variables y(n-1), y(n-2) and y(n-3).

float b = 0.2; // filter b coefficients.
float b1 = -0.1; 
float b2 = 0.1;
float b3 = -0.03;

float a = 1; // filter y coefficients.
float a1 = 1.5;
float a2 = 0.1;
float a3 = 0.2;

AnalogIn gainIn(A1); // analog input reading value from gain pot. pin A1.
AnalogIn filterIn(A3); // analog input reading value from filter pot. pin A3.
AnalogOut my_output(PA_4); // analog output. pin A2.
AnalogIn analog_value(A0); // analog input for input signal. pin A0.

int main()
{
    while(1)
    {
    x = analog_value.read(); // input signal
        xMid = x + 0.04f; 	// input signal is centred so signal will enter system at 
        					// midpoint and be clipped equally at the top and bottom.
       
        float gain = gainIn.read(); 	// read signal from gain pot.
        float filter = filterIn.read(); // read signal from filter pot.
        
        float clipHi = 0.5+gain; // set the upper clipping limit.
        if (xMid > clipHi) 		// if the input goes above the upper clipping 
        						// limit it is set at the upper limit.
        {
            if (clipHi > 1)
            clipHi = 1;
          xMid = clipHi;
        }
        float clipLo = 0.5-gain; // set the lower clipping limit.
        if (xMid < clipLo) 		// if the input goes below the lower clipping
        						// limit it is set at the lower limit.
        {
            if (clipLo < 0)
            clipLo = 0;
            xMid = clipLo;
        }
        y = xMid; // set y as the clipped signal.
       
        // the clipped signal is filtered and added to the unfiltered clipped signal.
        // the level of the filtered signal is controlled by the filter float which is
        // controlled by the filter pot 
        yOut = (y +(((b*xMid + b1*x1 + b2*x2 + b3*x3 - a1*y1 - a2*y2 + a3*y3)/a)*2*filter));
        my_output.write(yOut); // write to output.
            xMid = x1; 	// the filter variables are recalculated after output.
            x1 = x2;	// x(n) becomes x(n-1), x(n-1) becomes x(n-2) etc...
            x2 = x3;
            y = y1;		//y(n) becomes y(n-1), y(n-1) becomes y(n-2) etc...
            y1 = y2;
            y2 = y3; 
    }
}