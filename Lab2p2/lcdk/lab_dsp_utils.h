#ifndef LAB_DSP_UTILS_H
#define LAB_DSP_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif
/*************************************************************************************************/

/* Constantes más usadas */
#define PI          3.141592653589793

/* Macros */
/*----------------------------------------------------------------------*/
#define CODEC_WORD_LENGTH   16      // Es fijo en 16 bits para la librería L138_LCDK_aic3106
/*----------------------------------------------------------------------*/
// Macro Typecasting CODEC a flotante
#define AIC_2_FLT(x) ( (float)(x)  * ( 1.0 / ( (1 << CODEC_WORD_LENGTH) - 1.0) ) );
#define FLT_2_AIC(x) ( (int16_t)(x * ( (1 << CODEC_WORD_LENGTH) - 1) ) );

/* Declaración de funciones */
int write_codec_outputs(float float_out_l, float float_out_r);
void read_codec_outputs(float *float_in_l, float *float_in_r);


/*************************************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
