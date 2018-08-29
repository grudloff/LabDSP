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

#define M   (int)(0.125*CODEC_FS)

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
float   b_k = 0.3;
//M = 0.125;
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
        // Buffer L
        xL[idx_l] = float_in_l;           //buffer circular
        idx_l = ((idx_l+1)%BUFFERSIZE);
        // Buffer R
        xR[idx_r] = float_in_r;           //buffer circular
        idx_r = ((idx_r+1)%BUFFERSIZE);
		/*----------------------------------------------------------------------*/
        /* Asignación de salidas */
        //Pregunta 2
        if ((idx_r-1)-4*M<0){
            float_out_r=0;
            float_out_l=0;}
        else{
            float_out_r = b_k*xR[(idx_r-1)-(1*M)] + b_k*xR[(idx_r-1)-2*M] + b_k*xR[(idx_r-1)-3*M] + b_k*xR[(idx_r-1)-4*M];
            //Pregunta 3
            //float_out_r = pow(b_k,1)*xR[(idx_r-1)-1*M*CODEC_FS] + pow(b_k,2)*xR[(idx_r-1)-2*M*CODEC_FS] + pow(b_k,3)*xR[(idx_r-1)-3*M*CODEC_FS] + pow(b_k,4)*xR[(idx_r-1)-4*M*CODEC_FS] + pow(b_k,5)*xR[(idx_r-1)-5*M*CODEC_FS] + pow(b_k,6)*xR[(idx_r-1)-6*M*CODEC_FS] + pow(b_k,7)*xR[(idx_r-1)-7*M*CODEC_FS] + pow(b_k,8)*xR[(idx_r-1)-8*M*CODEC_FS] + pow(b_k,9)*xR[(idx_r-1)-9*M*CODEC_FS] + pow(b_k,10)*xR[(idx_r-1)-10*M*CODEC_FS]
            float_out_l = float_in_l;}

        /*----------------------------------------------------------------------*/
        // Escritura en salida del codec
        write_codec_outputs(float_out_l,float_out_r);
    }
}
