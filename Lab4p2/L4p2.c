/***************************************************************************//**
* \file     L4p2.c
*
* \brief    Base para el laboratorio L4p2
*
* \authors  Javier Romero, Gonzalo Carrasco
*******************************************************************************/

/***************************************************************************//**
**      HEADER FILES
*******************************************************************************/
#include "L138_LCDK_aic3106_init.h"
#include <math.h> 
#include "FFT_radix2.h"
#include "complex.h"

/******************************************************************************
**      MACRO DEFINITIONS
*******************************************************************************/
// Configuración del Codec
#define CODEC_FS            FS_8000_HZ          // Frecuencia de muestreo, revisar L138_LCDK_aic3106_init.h
#define CODEC_INPUT_CFG     LCDK_LINE_INPUT     // Entrada de DSP: {LCDK_LINE_INPUT , LCDK_MIC_INPUT}
#define CODEC_ADC_GAIN      ADC_GAIN_0DB
#include "flt_samp_time.h"                      // Define TS como el periodo de muestreo
/*----------------------------------------------------------------------*/

#define PI 3.14159265358979

#define FREQ_ALARM  1000.0
#define AMP_ALARM  10000.0 //32767.0

const float ratio    = 5.011;             //14 [db] below of fundamental, app.


/***************************************************************************//**
**      GLOBAL VARIABLES
*******************************************************************************/
// Configuración de buffers para DMA
extern int16_t *pingIN, *pingOUT, *pongIN, *pongOUT;
volatile int buffer_full = 0;
int procBuffer;
int index = 0;
/*----------------------------------------------------------------------*/
// Variables para FFT
float    alarm[N]; // alarm signal  buffer
Complex  input[N]; // complex input buffer (don't need reverse)
float auxi[N]; // entrada
Complex x_n[N];
Complex    FFT[N]; // FFT result    buffer
float   absFFT[N]; // FFT magnitude buffer
short dist = 0;
float max_fft_val;
unsigned int max_fft_idx;

/*----------------------------------------------------------------------*/
// Señales de prueba con 8 muestras
Complex fft_test_cte[8] = {{1.0, 0.0},{1.0, 0.0},{1.0, 0.0},{1.0, 0.0},{1.0, 0.0},{1.0, 0.0},{1.0, 0.0},{1.0, 0.0}};

Complex fft_test_delta_k[8] = {{1.0, 0.0},{0.0, 0.0},{0.0, 0.0},{0.0, 0.0},{0.0, 0.0},{0.0, 0.0},{0.0, 0.0},{0.0, 0.0}};

Complex fft_test_cos[8] = {{1.0, 0.0},{0.707106781186548, 0.0},{0.0, 0.0},{-0.707106781186547, 0.0},{-1.0, 0.0},{-0.707106781186547, 0.0},{0.0, 0.0},{0.707106781186547, 0.0}};

Complex fft_test_exp[8] = {{1.0, 0.0},{0.707106781186548, -0.707106781186548},{0.0, -1.0},{-0.707106781186548, -0.707106781186548},{-1.0, 0.0},{-0.707106781186548, 0.707106781186548},{0.0, 1.0},{0.707106781186548, 0.707106781186548}};

Complex fft_test_out[8];

float fft_test_mag_vec[8];

/***************************************************************************//**
**      INTERNAL FUNCTION DECLARATIONS
*******************************************************************************/
void max_val_idx(unsigned int size, float *vec, float *max_val, unsigned int *max_idx);
short too_much_distortion(unsigned int size, int max_fft_idx,float max_fft_val,float *absFFT,float ratio);


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
*   \brief  Llenado de buffers con DMA
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
  int16_t right_sample[N];
  int idx;
  index+=1;
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
  // Llenado del vector complejo de entrada

  for (idx = 0; idx < (BUFCOUNT/2) ; idx++)
  {
    // Se debe leer en orden el inBuff para sacar:
    // el canal izquierdo, y
    input[idx].real = (float)(*inBuf++);
    input[idx].img = 0;
    auxi[idx]=input[idx].real;
    // luego el derecho.
    right_sample[idx] = *inBuf++;
  } 
  /*----------------------------------------------------------------------*/
  // Cómputo de FFR radix-2

  fft_radix2(N, input, FFT);

  // Cómputo de magnitudes en frecuencia
  fft_mag(N, FFT, absFFT);

  // Obtención de frecuencia fundamental, considerando que ésta tiene
  // la mayor magnitud en el vector de frecuencias.
  max_val_idx(N, absFFT, &max_fft_val, &max_fft_idx);

  /*----------------------------------------------------------------------*/
  // Detección de alta distorsión armónica
  dist = too_much_distortion(N,max_fft_idx,max_fft_val,absFFT,ratio);

  /*----------------------------------------------------------------------*/
  // Escritura en el buffer de salida para el DMA
  // Se escribe el canal izquierdo primero y luego el derecho
  for (idx = 0; idx < (BUFCOUNT/2) ; idx++)
  {
    // Canal izquierdo
    if (dist)
        *outBuf++ = (int16_t)(alarm[idx]);
    else
        *outBuf++ = right_sample[idx];
    // Canal derecho
    *outBuf++ = right_sample[idx];
  }
  // indicate that buffer has been processed
  buffer_full = 0;
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
    int n;

    /*----------------------------------------------------------------------*/
    // Configuración inicial de buffers
    for (n=0 ; n< N ; n++){
        W_N[n].real = cos(2*PI*n/((float) N));
        W_N[n].img  =-sin(2*PI*n/((float) N));
        alarm[n]    = AMP_ALARM*cos(2*PI*n*FREQ_ALARM/FS_8000_HZ);
        input[n].img = 0.0;
        x_n[n].img=0.0;
        if(n==(N/2))
            auxi[n]=(2.0/3.0);
        else
            auxi[n] = (2.0/3.0)*(sin((2.0/3.0)*(((float)n) -(((float)N)/2.0)))/((2.0/3.0)*(((float)n) -(((float)N)/2.0))));
        x_n[n].real = auxi[n];
    }
    /*----------------------------------------------------------------------*/
    // Pruebas Offline de fft_radix2

    //sinc
    fft_radix2(N, x_n, FFT);
    fft_mag(N, FFT, absFFT);
    //max_val_idx(N, fft_test_mag_vec, &max_fft_val, &max_fft_idx);


    // Para muestras constantes
    fft_radix2(8, fft_test_cte, fft_test_out);
    fft_mag(8, fft_test_out, fft_test_mag_vec);
    max_val_idx(8, fft_test_mag_vec, &max_fft_val, &max_fft_idx);

    // Para muestras de delta kronecker
    fft_radix2(8, fft_test_delta_k, fft_test_out);
    fft_mag(8, fft_test_out, fft_test_mag_vec);

    // Para muestras de un coseno
    fft_radix2(8, fft_test_cos, fft_test_out);
    fft_mag(8, fft_test_out, fft_test_mag_vec);
    max_val_idx(8, fft_test_mag_vec, &max_fft_val, &max_fft_idx);

    // Para muestras de una exponencial de rotación negativa
    fft_radix2(8, fft_test_exp, fft_test_out);
    fft_mag(8, fft_test_out, fft_test_mag_vec);
    max_val_idx(8, fft_test_mag_vec, &max_fft_val, &max_fft_idx);

    /*----------------------------------------------------------------------*/
    // Inicialización del Codec de audio
    L138_initialise_edma(CODEC_FS,CODEC_ADC_GAIN,DAC_ATTEN_0DB,CODEC_INPUT_CFG);
    /*----------------------------------------------------------------------*/
    // Loop infinito
    while(1)
    {
        // Retención hasta que buffer_full es alto
        while (!buffer_full);
        // Una vez llenado el buffer, se procesa
        process_buffer();
    }
}

/***************************************************************************//**
*   \brief  Función que obtiene el mayor valor de un vector de flotantes, y
*           escribe dicho valor en 'max_val' y el índice en el vector en 'max_idx'.
*
*           Notar que la búsqueda no considera el primer elemento, para su uso en las
*           magnitudes de FFT, y así no considerar la frecuencia 0.
*
*   \param  size    : largo del vector a calcular.
*   \param  *vec    : puntero a vector de flotantes.
*   \param  *max_val : puntero a flotante que quedará con valor máximo.
*   \param  *max_idx : puntero a entero que quedará con el índice valor máximo.
*
*   \return Void.
*******************************************************************************/
void max_val_idx(unsigned int size, float *vec, float *max_val, unsigned int *max_idx){
    unsigned int idx;
    *max_val = 0.0;

    for (idx = 1; idx < (size>>1); idx++)
        if (vec[idx] > *max_val){
            *max_val = vec[idx];
            *max_idx = idx;
        }
}

/***************************************************************************//**
*   \brief  Función a completar para detectar distorsión
*
*   \param  Void.
*
*   \return Void.
*******************************************************************************/
short too_much_distortion(unsigned int size, int max_fft_idx,float max_fft_val,float *absFFT,float ratio){
    int i;
    for(i=2;i*max_fft_idx+2<N/2;i++){
            if((ratio*absFFT[(unsigned int)(i*max_fft_idx)])>max_fft_val)
                return 1;
            if((ratio*absFFT[(unsigned int)((i*max_fft_idx)+1)])>max_fft_val)
                return 1;
//            if((ratio*absFFT[(unsigned int)((i*max_fft_idx)+2)])>max_fft_val)
//                return 1;
            if((ratio*absFFT[(unsigned int)((i*max_fft_idx)-1)])>max_fft_val)
                return 1;
//            if((ratio*absFFT[(unsigned int)((i*max_fft_idx)-2)])>max_fft_val)
//                return 1;

        }
    return 0;
}



