/***************************************************************************//**
* \file     lab_dsp_utils.c
*
* \brief    Este archivo contiene funciones de propósito general para simplificar
*           tareas para varias experiencias.
*
* \authors  Gonzalo Carrasco
*******************************************************************************/

/***************************************************************************//**
**      HEADER FILES
*******************************************************************************/
#include "L138_LCDK_aic3106_init.h"  // Es mandatorio contar con las definiciones del la librería L138_LCDK_aic3106_init
#include "lab_dsp_utils.h"      // Encabezado de lab_dsp_utils.c


/******************************************************************************
**      MACRO DEFINITIONS
*******************************************************************************/
//#define CONSTANT  1 // If necessary, because they should be in code_template.h


/***************************************************************************//**
**      GLOBAL VARIABLES
*******************************************************************************/
//#include "global_vars.h" // If there are too many variables, they will be in a .h
//unsigned int x;


/***************************************************************************//**
**      INTERNAL FUNCTION DEFINITIONS
*******************************************************************************/

/***************************************************************************//**
*   \brief  Abstracción de la escritura al codec y casteo a entero de ambos
*           canales con datos en tipo flotantes.
*
*   \param
*
*   \return Void.
*******************************************************************************/
int write_codec_outputs(float float_out_l, float float_out_r)
{
    int RetVal = 0;             // Sin saturación por defecto
    AIC31_data_type codec_data;
    /*----------------------------------------------------------------------*/
    /* Detección de saturación en canal izquierdo */
    if (float_out_l > 1.0)
    {
        float_out_l = 1.0;
        RetVal = 1;
    }
    else if (float_out_l < -1.0)
    {
        float_out_l = -1.0;
        RetVal = 1;
    }
    /* Detección de saturación en canal derecho */
    if (float_out_r > 1.0)
    {
        float_out_r = 1.0;
        RetVal = 1;
    }
    else if (float_out_r < -1.0)
    {
        float_out_r = -1.0;
        RetVal = 1;
    }
    /*----------------------------------------------------------------------*/
    //typecasting - full range 16-bit signed integer
    codec_data.channel[LEFT]  = FLT_2_AIC(float_out_l);
    codec_data.channel[RIGHT] = FLT_2_AIC(float_out_r);
    output_sample(codec_data.uint);
    /*----------------------------------------------------------------------*/
    return RetVal;
}

/***************************************************************************//**
*   \brief  Abstracción de la lectura de ambos canales del codec y
*           casteo a flotante.
*
*   \param
*
*   \return Void.
*******************************************************************************/
void read_codec_outputs(float *float_in_l, float *float_in_r)
{
    AIC31_data_type codec_data;
    /*----------------------------------------------------------------------*/
    codec_data.uint = input_sample();
    *float_in_l = AIC_2_FLT( codec_data.channel[LEFT]  );
    *float_in_r = AIC_2_FLT( codec_data.channel[RIGHT] );
}

