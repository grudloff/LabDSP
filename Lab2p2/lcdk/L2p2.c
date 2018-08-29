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

/******************************************************************************
**      MACRO DEFINITIONS
*******************************************************************************/
// Configuración del Codec
#define CODEC_FS        FS_16000_HZ         // Frecuencia de muestreo, revisar L138_LCDK_aic3106_init.h
#define CODEC_INPUT_CFG LCDK_LINE_INPUT     // Entrada de DSP: {LCDK_LINE_INPUT , LCDK_MIC_INPUT}
#include "flt_samp_time.h"                  // Define TS como el periodo de muestreo
/*----------------------------------------------------------------------*/
// Constantes buffers
#define BUFFERSIZE        4800              //
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
int     idx_r = 0, idx_l = 0;

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

        /*----------------------------------------------------------------------*/
        /* Asignación de salidas */
        float_out_r = overdrive(float_in_r);
        float_out_l = float_in_l;
        /*----------------------------------------------------------------------*/
        // Escritura en salida del codec
        write_codec_outputs(float_out_l,float_out_r);
    }
}

float overdrive(float x){
    if(abs(x)>=ALPHA)
        return G*(BETA*x+sign(x)*(1-BETA)*ALPHA);
    else
        return G*x;
}

