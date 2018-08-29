/***************************************************************************//**
* \file     L2p2.c
*
* \brief    Base para el laboratorio L2p2
*
* \authors  Gonzalo Carrasco
*******************************************************************************/

/***************************************************************************//**
**      HEADER FILES
*******************************************************************************/
#include "L138_LCDK_aic3106_init.h"
#include "lab_dsp_utils.h"
#include <math.h>
#include <mathf.h>
#include <string.h>


/******************************************************************************
**      MACRO DEFINITIONS
*******************************************************************************/
// Configuración del Codec
#define CODEC_FS        FS_16000_HZ         // Frecuencia de muestreo, revisar L138_LCDK_aic3106_init.h3
#define CODEC_INPUT_CFG LCDK_LINE_INPUT     // Entrada de DSP: {LCDK_LINE_INPUT , LCDK_MIC_INPUT}
#include "flt_samp_time.h"                  // Define TS como el periodo de muestreo
/*----------------------------------------------------------------------*/
// Constantes buffers
#define BUFFERSIZE        (unsigned int 0.250*CODEC_FS)

//Constantes delay multitap
#define M                  (unsigned int) (0.250*CODEC_FS)  //Para para lograr 250ms
#define wet                 0.3
#define D                   0.002*CODEC_FS//amplitud de variación del delay


/*----------------------------------------------------------------------*/

/***************************************************************************//**
**      GLOBAL VARIABLES
*******************************************************************************/

// Inicializacion de variables de la portadora
float dThetak=0, Thetak=0, Ttack, Ttask;
float freq = 2.0;
float Ts = 1/CODEC_FS;

// Entradas y Salidas del AIC Codec
AIC31_data_type codec_data;
float float_in_r,float_in_l;
float float_out_r,float_out_l;
/*----------------------------------------------------------------------*/
// Buffers
float   xR[BUFFERSIZE];
float   xL[BUFFERSIZE];

float xR_d;
float xL_d;
int i;
float d;

/***************************************************************************//**
**      INTERNAL FUNCTION DEFINITIONS
*******************************************************************************/
void main()
{
    // Inicialización BSL y AIC31 Codec
    L138_initialise_poll(CODEC_FS, ADC_GAIN_0DB, DAC_ATTEN_0DB, CODEC_INPUT_CFG);
    while(1)
    {
        // Lectura de entradas del codec
        read_codec_outputs(&float_in_l, &float_in_r);
        // Sinusoidal reference generation
        dThetak = 2.0*PI*freq*Ts;
        Thetak = Thetak + dThetak;
        // Theta e [0, 2*pi]
        if ( Thetak > 2.0*PI ) Thetak = Thetak - 2.0*PI;
        // sin(Theta) and cos(Theta)
        d = D*sinf(Thetak);
        /*----------------------------------------------------------------------*/
        // Buffer L
        xL[idx] = float_in_l;           //buffer circular
        // Buffer R
        xR[idx] = float_in_r;           //buffer circular
        idx = ((idx+1)%BUFFERSIZE);
        /*----------------------------------------------------------------------*/
        xR_d=(1-wet)*xR[0];
        xL_d=(1-wet)*xL[0];

        Mreal=(M+d+idx)%BUFFERSIZE;
        xR_d+=wet*xR[Mreal-1];
        xL_d+=wet*xL[Mreal-1];

        /*----------------------------------------------------------------------*/
        /* Asignación de salidas */
        float_out_r = xR_d;
        float_out_l = xL_d;
        /*----------------------------------------------------------------------*/
        // Escritura en salida del codec
        write_codec_outputs(float_out_l,float_out_r);
    }
}


