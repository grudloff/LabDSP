/***************************************************************************//**
* \file     main_l3p2.c
*
* \brief    Base para el laboratorio L2p2
*
* \authors  Gonzalo Carrasco
*******************************************************************************/

/***************************************************************************//**
**      HEADER FILES
*******************************************************************************/
#include "L138_LCDK_aic3106_init.h"
#include "dsp_lab_utils.h"
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

/***************************************************************************//**
**      GLOBAL VARIABLES
*******************************************************************************/
// Entradas y Salidas del AIC Codec
AIC31_data_type codec_data;
float float_in_r,float_in_l;
float float_out_r,float_out_l;
/*----------------------------------------------------------------------*/
// Parámetros para uso de funciones de estados controlados por push bottons
#define PB_TRIM_COUNTER_DELAY_PERIOD    (16000)     // (16000) para 1s @ 16ksps
#define PB_TRIM_COUNTER_AUTOINC_PERIOD  (320)       // (320)    para 20ms @16ksps
#define PB_TRIM_COUNTER_MIN             (0)         // (int32_t) Puede ser negativo
#define PB_TRIM_COUNTER_MAX             (100)       // (int32_t)
// Variables de estado para uso de push bottons
uint8_t pb1;
uint8_t pb2;
uint32_t pb_toggles = 0;
int32_t pb_trim_counter = 0;
/*----------------------------------------------------------------------*/
// Buffers e índices
//float   xR[BUFFERSIZE];
//float   xL[BUFFERSIZE];
//int     idx_r = 0, idx_l = 0;
/*----------------------------------------------------------------------*/
// Filtro notch
float a_k[3];
float b_k[3];
float w0;
float xr[3]={0,0,0};
float w[3] = {0,0,0};

//filtro pasabanda
float r=0.99;

//detector freq
float f;
uint8_t flag=0;
uint32_t count=0;


/***************************************************************************//**
**      INTERNAL FUNCTION DEFINITIONS
*******************************************************************************/
void main()
{
    // Inicialización BSL y AIC31 Codec
    L138_initialise_poll(CODEC_FS, ADC_GAIN_0DB, DAC_ATTEN_0DB, CODEC_INPUT_CFG);
    // Inicialización de LEDs
    DLU_leds_init();
    // Inicialización de Pulsadores User 1 y User 2
    DLU_push_bottons_init();
    while(1)
    {
        // Lectura de entradas del codec
        DLU_read_codec_outputs(&float_in_l, &float_in_r);
        /*----------------------------------------------------------------------*/
        //Frecuencia
        xr[2] = xr[1];
        xr[1] = xr[0];
        xr[0]=float_in_r;

       if((xr[0]>0)&&(xr[2]<0)&&(flag==0)){
           f=CODEC_FS/(2.0*count);
           flag=1;
           count=0;
       }
       else if((xr[0]<0)&&(xr[2]>0)&&(flag==1)){
           f=CODEC_FS/(2.0*count);
           flag=0;
           count=0;
       }
       count++;



        /*----------------------------------------------------------------------*/
//        // Buffer L
//        xL[idx_l] = float_in_l;           //buffer circular
//        idx_l = ((idx_l+1)%BUFFERSIZE);
//        // Buffer R
//        xR[idx_r] = float_in_r;           //buffer circular
//        idx_r = ((idx_r+1)%BUFFERSIZE);
        /*----------------------------------------------------------------------*/
        // USO DE PULSADORES
        // Obtención de nivel de pulsadores (con antirebote)
        pb1 = DLU_pb_USER1_deb_get();
        pb2 = DLU_pb_USER2_deb_get();
        // Generación de estados "pb_toggles" y cuenta de ajuste "pb_trim_counter" con los pulsadores
        DLU_pb_toggle_update(&pb_toggles, pb1, pb2);
        DLU_pb_trim_counter_update(&pb_trim_counter,  PB_TRIM_COUNTER_DELAY_PERIOD, PB_TRIM_COUNTER_AUTOINC_PERIOD, PB_TRIM_COUNTER_MIN, PB_TRIM_COUNTER_MAX);
        /*----------------------------------------------------------------------*/
        // USO DE LEDS
        // Led D4 en función de valor de pulsador
        DLU_led_D4_set(pb1);
        // Obtención de bits de estados para controlar estado de leds
        // Led D5
        if ( DLU_REG_BIT_GET(pb_toggles, PB_TOGGLES_T1_BIT) )
            DLU_led_D5_set(OUTPUT_HIGH);
        else
            DLU_led_D5_set(OUTPUT_LOW);
        // Led D6
        if ( DLU_REG_BIT_GET(pb_toggles, PB_TOGGLES_T2_BIT) )
            DLU_led_D6_set(OUTPUT_HIGH);
        else
            DLU_led_D6_set(OUTPUT_LOW);
        // led D7
        if ( DLU_REG_BIT_GET(pb_toggles, PB_TOGGLES_T12_BIT) )
            DLU_led_D7_set(OUTPUT_HIGH);
        else
            DLU_led_D7_set(OUTPUT_LOW);
        /*----------------------------------------------------------------------*/
        // Implementacion directa II, para el canal izquierdo
        //Calculo del w0
       w0=(2*PI*f)/CODEC_FS;
       //asignacion de constantes:
       //Filtro Notch
       if(DLU_REG_BIT_GET(pb_toggles, PB_TOGGLES_T1_BIT)){
           a_k[0]=sinf(w0);
           a_k[1]=0;
           a_k[2]=0;
           b_k[0]=1;
           b_k[1]=-2*cosf(w0);
           b_k[2]=1;

       }
       //Filtro pasabanda
       else{
          a_k[0]=1;
          a_k[1]=-2*cosf(w0)*r;
          a_k[2]=r*r;
          b_k[0]=(1-r);
          b_k[1]=0;
          b_k[2]=0;
       }

       // Implementacion directa II, para el canal izquierdo
       // variable intermedia

       w[0] = float_in_l - a_k[1]*w[1] - a_k[2]*w[2];
       // Salida L
       float_out_l = (b_k[0]*w[0] + b_k[1]*w[1] + b_k[2]*w[2])/a_k[0];
       // Actualizacion
       w[2] = w[1];
       w[1] = w[0];

        // Salida R
        float_out_r = (float)pb_trim_counter * float_in_r / PB_TRIM_COUNTER_MAX;
        /*----------------------------------------------------------------------*/
        // Escritura en salida del codec
        DLU_write_codec_outputs(float_out_l,float_out_r);
    }
}


