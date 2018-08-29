/***************************************************************************//**
* \file     L5p2.c
*
* \brief    Base para el laboratorio L5p2
*
* \authors  Javier Romero, Gonzalo Carrasco
*******************************************************************************/

/***************************************************************************//**
**      HEADER FILES
*******************************************************************************/
#include "global_defs.h"        // FRAME_SIZE definition
#include "L138_LCDK_aic3106_init.h"
#include <math.h>
#include "functions.h"
#include "levinson.h"
#include "dsp_lab_utils.h"
#include <stdlib.h>

/******************************************************************************
**      MACRO DEFINITIONS
*******************************************************************************/
// Configuración del Codec
#define CODEC_FS            FS_8000_HZ      // Frecuencia de muestreo, revisar L138_LCDK_aic3106_init.h
#define CODEC_INPUT_CFG     LCDK_MIC_INPUT  // Entrada de DSP: {LCDK_LINE_INPUT , LCDK_MIC_INPUT}
#define CODEC_ADC_GAIN      ADC_GAIN_12DB
#include "flt_samp_time.h"                  // Define TS como el periodo de muestreo
/*----------------------------------------------------------------------*/
// Orden de parámetros lpc y filtro
#define ORDER               15

/***************************************************************************//**
**      GLOBAL VARIABLES
*******************************************************************************/
// Configuración de buffers para DMA
extern int16_t *pingIN, *pingOUT, *pongIN, *pongOUT;
volatile int buffer_full = 0;
int procBuffer;
/*----------------------------------------------------------------------*/
// Contador de segmentos procesados
int index = 0;
/*----------------------------------------------------------------------*/
// Variables del algoritmos LPC
int input[FRAME_SIZE];
float input_rms  = 0.0;

int input_windowed[2*FRAME_SIZE];       // Buffer intermedio
float correlations_array[ORDER+1];
float lpc[ORDER+1];                   // LPC  coefficients

float filtered_out[FRAME_SIZE];       // output buffer
float filtered_rms = 0.0;

int *excitation;

int prand_noise[FRAME_SIZE];

/***************************************************************************//**
**      INTERNAL FUNCTION DECLARATIONS
*******************************************************************************/

/***************************************************************************//**
**      INTERNAL FUNCTION DEFINITIONS
*******************************************************************************/

/***************************************************************************//**
*   \brief  Llenado de buffers con DMA
*
*   \param  Void.
*
*   \return Void.
*******************************************************************************/
interrupt void interrupt4(void) // interrupt service routine
{
 switch(EDMA_3CC_IPR)
  {
    case 1:                     // TCC = 0
      procBuffer = PING;        // process ping
      EDMA_3CC_ICR = 0x0001;    // clear EDMA3 IPR bit TCC
      break;
    case 2:                     // TCC = 1
      procBuffer = PONG;        // process pong
      EDMA_3CC_ICR = 0x0002;    // clear EDMA3 IPR bit TCC
      break;
    default:                    // may have missed an interrupt
      EDMA_3CC_ICR = 0x0003;    // clear EDMA3 IPR bits 0 and 1
      break;
  }
  EVTCLR0 = 0x00000100;
  buffer_full = 1;              // flag EDMA3 transfer
  return;
}

/***************************************************************************//**
*   \brief  Rutina de algoritmos de procesamiento de señales
*
*   \param  Void.
*
*   \return Void.
*******************************************************************************/
void process_buffer(void) // should be called from infinite
                          // loop in main() when process
                          // buffer has been filled
{
  int16_t *inBuf, *outBuf;      // pointers to process buffers
  int16_t right_sample[FRAME_SIZE];
  int16_t left_sample = 0;
  int     right_sample_int[FRAME_SIZE];
  int i;
  /*----------------------------------------------------------------------*/
  // Se apaga led idicador de time slack
  DLU_led_D4_set(OUTPUT_LOW);
  // Contador de segmentos
  index+=1;
  // Gestión de buffers del DMA
  if (procBuffer == PING)       // use ping or pong buffers
  {
    inBuf = pingIN;
    outBuf = pingOUT;
  }
  if (procBuffer == PONG)
  {
    inBuf = pongIN;
    outBuf = pongOUT;
  }

  /*----------------------------------------------------------------------*/
  // Lectura de buffer de entrada desde canal izquierdo hacia buffer 'input'
  for (i = 0; i < (FRAME_SIZE) ; i++)
  {
    right_sample[i] = *inBuf++;
    right_sample_int[i] = (int)right_sample[i];
    input[i] = *inBuf++;
  }




  /************************************************************************/
  /************************************************************************/
  // PROCESAMIENTO PARA IMPLEMENTAR SÍNTESIS DE VOZ
  /*----------------------------------------------------------------------*/
  // 1) Ventana
  window(input_windowed, input);
  /*----------------------------------------------------------------------*/
  // 2) Autocorrelación
  xcorr(&input_rms,correlations_array,input_windowed,ORDER+1);

  /*----------------------------------------------------------------------*/
  // 3) Levinson
  levinson(lpc,correlations_array, ORDER);

  /*----------------------------------------------------------------------*/
  //Detección sonido o silencio
  if(input_rms>400){
      excitation=excitation_pulses_2;
      DLU_led_D7_set(OUTPUT_HIGH);
  }
  else{
//      excitation=excitation_noise;
      excitation=prand_noise;
      DLU_led_D7_set(OUTPUT_LOW);
  }

  /*----------------------------------------------------------------------*/
  // 4) Filter
  filter(&filtered_rms, filtered_out, lpc, ORDER, right_sample_int);

  /*----------------------------------------------------------------------*/
  // Escritura de buffer de salida para el codec
  for (i = 0; i < (FRAME_SIZE) ; i++)
  {
      prand_noise[i]=rand()-32767/2;
    *outBuf++ = right_sample[i];
    *outBuf++ = filtered_out[i]*(input_rms/filtered_rms);

  }
  // Bajada de bandera
  buffer_full = 0; // indicate that buffer has been processed

  /************************************************************************/
  /************************************************************************/
  // Encendido de led de indicación de time slack
  DLU_led_D4_set(OUTPUT_HIGH);

  return;
}

/***************************************************************************//**
*   \brief  MAIN
*
*   \param  Void.
*
*   \return Void.
*******************************************************************************/
int main(void)
{
    int i=0;
    for(i=0;i<2*FRAME_SIZE;i++)
        input_windowed[i]=0;
    /*----------------------------------------------------------------------*/
    // Inicialización de algoritmo de Levinson-Durbin
    levinson_init(ORDER);
    /*----------------------------------------------------------------------*/
    // Inicialización del Codec de audio
    L138_initialise_edma(CODEC_FS,CODEC_ADC_GAIN,DAC_ATTEN_0DB,CODEC_INPUT_CFG);
    /*----------------------------------------------------------------------*/
    // Inicialización de leds de la LCDK
    DLU_leds_init();
    DLU_led_D4_set(OUTPUT_HIGH);
    DLU_led_D5_set(OUTPUT_LOW);
    DLU_led_D6_set(OUTPUT_LOW);
    DLU_led_D7_set(OUTPUT_LOW);
    /*----------------------------------------------------------------------*/
    while(1)
    {
        // Retención hasta que el buffer es llenado por DMA
        while (!buffer_full);
        // Invocación a procesamiento
        process_buffer();
    }
}






