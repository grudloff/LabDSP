#include "L138_LCDK_aic3106_init.h"
#include <math.h>
#include <mathf.h>

// Constantes
#define buffersize      800                 // Longitud del buffer
#define PI              3.141592653589793   // constante trigonometrica  3.1415926
#define Ts              6.25e-5             // Tiempo de Muestreo 1/16000 16kHz
// constantes del OBQ para f_o=50[Hz]
#define b0              0.019633
#define a0              1.0
#define a1             -1.999614
#define a2              1.0

// Entradas y Salidas del AIC Codec
AIC31_data_type codec_data;
int16_t int_in_r, int_in_l;
float float_in_r,float_in_l;
float float_out_r,float_out_l;

// variables a graficar
float   y_watch[buffersize];
int     i = 0;

// Inicializacion de variables osciladores biquad
float   x[1] = {1.0};
float   y[3] = {0.0,0.0,0.0};

// Inicializacion de variables de la portadora
float dThetak=0, Thetak=0, Ttack, Ttask;
float freq = 500.0;

// Inicializacion de variables de modulacion
float yAM = 0.0;
float yFM = 0.0;

void main()
{
    // Inicializacion BSL y AIC31 Codec
    int32_t fs = FS_16000_HZ; // Frecuencia de muestreo, revisar L138_LCDK_aic3106_init.h
    int8_t input_conf = LCDK_LINE_INPUT; //Entrada de DSP, LINE o MIC;

    L138_initialise_poll(fs, ADC_GAIN_0DB,DAC_ATTEN_0DB, input_conf);

    while(1)
    {
        //Input channel read
        codec_data.uint = input_sample();
        int_in_l = codec_data.channel[LEFT];
        int_in_r = codec_data.channel[RIGHT];

        //typecasting - unitary float variable
        float_in_l=((float)(int_in_l))/32767.0;
        float_in_r=((float)(int_in_r))/32767.0;

        // Sinusoidal reference generation
        dThetak = 2.0*PI*freq*Ts;
        Thetak = Thetak + dThetak;
        // Theta e [0, 2*pi]
        if ( Thetak > 2.0*PI ) Thetak = Thetak - 2.0*PI;
        // sin(Theta) and cos(Theta)
        Ttack = cosf(Thetak);
        Ttask = sinf(Thetak);

        // Oscilador Biquad f_o=50[Hz] 
        y[0] = (b0/a0)*x[0] - (a1/a0)*y[1] - (a2/a0)*y[2];
        y[2] = y[1];
        y[1] = y[0];
        x[0] = 0.0;

        // se�al AM
        yAM = 0; 
        // se�al FM
        yFM = 0; 

        //signal manipulation
        float_out_l = y[0];
        float_out_r = Ttask; 

        //Para Graph
        y_watch[i] = y[0];          //moduladora
        i = ((i+1)%buffersize);     //buffer circular

        //typecasting - full range 16-bit signed integer
        codec_data.channel[LEFT] = (int16_t)(32767.0*float_out_l);
        codec_data.channel[RIGHT] = (int16_t)(32767.0*float_out_r);
        output_sample(codec_data.uint);
    }
}
