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
#include <stdio.h>
#include <string.h>

/******************************************************************************
**      MACRO DEFINITIONS
*******************************************************************************/
// Configuración del Codec
#define CODEC_FS        FS_32000_HZ         // Frecuencia de muestreo, revisar L138_LCDK_aic3106_init.h
#define CODEC_INPUT_CFG LCDK_LINE_INPUT     // Entrada de DSP: {LCDK_LINE_INPUT , LCDK_MIC_INPUT}
#include "flt_samp_time.h"                  // Define TS como el periodo de muestreo
/*----------------------------------------------------------------------*/
// Constantes buffers
//#define BUFFERSIZE        48                //3ms 16 k
//#define BUFFERSIZE          450             //28ms 16k
//#define BUFFERSIZE          450             //56.25ms 8k
//#define BUFFERSIZE          900             //112.5ms 8k
#define BUFFERSIZE          220               //6.875ms 32k


/*----------------------------------------------------------------------*/
// Constantes de la distorsion
#define G               1.0
#define BETA            0.05
#define ALPHA           0.1

/***************************************************************************//**
**      GLOBAL VARIABLES
*******************************************************************************/
// Entradas y Salidas del AIC Codec
AIC31_data_type codec_data;
float float_in_r,float_in_l;
float float_out_r,float_out_l;
/*----------------------------------------------------------------------*/
// Buffers e índices
float   xR[BUFFERSIZE];
float   xL[BUFFERSIZE];
//int     idx_r = 0, idx_l = 0;
int i;

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
        /*----------------------------------------------------------------------*/
        for (i=BUFFERSIZE-1;i!=0;i--){
            xL[i]=xL[i-1];
            xR[i]=xR[i-1];
        }
        // Buffer L
        //memmove(&xR[0],&xR[1],(BUFFERSIZE-1)*sizeof(float));//se mueve todo un elemento a la derecha
        xL[0] = float_in_l;           //buffer circular
        // Buffer R
        //memmove(&xL[0],&xL[1],(BUFFERSIZE-1)*sizeof(float));//se mueve todo un elemento a la derecha
        xR[0] = float_in_r;           //buffer circular
        /*----------------------------------------------------------------------*/
        /* Asignación de salidas */
        float_out_r = xR[BUFFERSIZE-1];
        float_out_l = float_in_r;
        /*----------------------------------------------------------------------*/
        // Escritura en salida del codec
        write_codec_outputs(float_out_l,float_out_r);
    }
}


