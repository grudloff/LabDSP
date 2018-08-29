#ifndef FLT_SAMP_TIME_H
#define FLT_SAMP_TIME_H

#ifdef __cplusplus
extern "C" {
#endif
/*************************************************************************************************/
/* DEBE SER INVOCADO SIEMPRE DESPUES DEFINIR CODEC_FS */
#if     CODEC_FS == FS_8000_HZ
#define TS              1.25e-4
#elif   CODEC_FS == FS_9600_HZ
#define TS              1.04166667e-4
#elif   CODEC_FS == FS_11025_HZ
#define TS              9.07029478e-5
#elif   CODEC_FS == FS_12000_HZ
#define TS              8.33333333e-5
#elif   CODEC_FS == FS_16000_HZ
#define TS              6.25e-5
#elif   CODEC_FS == FS_19200_HZ
#define TS              5.20833333e-5
#elif   CODEC_FS == FS_22050_HZ
#define TS              4.53514739e-5
#elif   CODEC_FS == FS_24000_HZ
#define TS              4.16666667e-5
#elif   CODEC_FS == FS_32000_HZ
#define TS              3.125e-5
#elif   CODEC_FS == FS_44100_HZ
#define TS              2.26757370e-5
#elif   CODEC_FS == FS_48000_HZ
#define TS              2.08333333e-5
#endif
/*************************************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
