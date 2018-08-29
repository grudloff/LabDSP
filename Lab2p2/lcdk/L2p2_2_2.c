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
#define CODEC_FS        FS_16000_HZ         // Frecuencia de muestreo, revisar L138_LCDK_aic3106_init.h
#define CODEC_INPUT_CFG LCDK_LINE_INPUT     // Entrada de DSP: {LCDK_LINE_INPUT , LCDK_MIC_INPUT}
#include "flt_samp_time.h"                  // Define TS como el periodo de muestreo
/*----------------------------------------------------------------------*/
//Constantes delay multitap
#define N                   4
#define M                   2000//((int) 0.125*CODEC_FS)  //Para para lograr 125ms
#define b                   0.3

// Constantes buffers
#define BUFFERSIZE          ((int)(N*M+1))
/*----------------------------------------------------------------------*/

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
int     idx = 0;

int Mreal;
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
        // Buffer L
        xL[idx] = float_in_l;           //buffer circular
        // Buffer R
        xR[idx] = float_in_r;           //buffer circular
        idx = ((idx+1)%BUFFERSIZE);
        /*----------------------------------------------------------------------*/
        /* Asignación de salidas */
        float_out_r = 0;
        float_out_l = 0;
        for(i=1;i<=N;i++){
            Mreal=(idx+(BUFFERSIZE-i*M))%BUFFERSIZE;
            float_out_r+=b*xR[Mreal];
            float_out_l+=b*xL[Mreal];
        }
        /*----------------------------------------------------------------------*/
        // Escritura en salida del codec
        write_codec_outputs(float_in_l,float_out_r);
    }
}


