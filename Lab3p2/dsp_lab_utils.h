#ifndef DSP_LAB_UTILS_H
#define DSP_LAB_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif
/*************************************************************************************************/

/* Constantes más usadas */
#define PI          3.141592653589793

/*----------------------------------------------------------------------*/
/* GPIOs de Leds en LCDK */
/*----------------------------------------------------------------------*/
// Configuración del Pinmux para usar DSP_BSL en la LCDK
// LED D4 -> GPIO6[13]
// LED D5 -> GPIO6[12]
#define PINMUX_LCDK_LEDS_D4_D5_GPIO_OE_REG      (13)
#define PINMUX_LCDK_LEDS_D4_D5_GPIO_OE_MASK     (0x0000FF00)
#define PINMUX_LCDK_LEDS_D4_D5_GPIO_OE_VAL      (0x00008800)

// LED D6 -> GPIO2[12]
#define PINMUX_LCDK_LEDS_D6_GPIO_OE_REG         (5)
#define PINMUX_LCDK_LEDS_D6_GPIO_OE_MASK        (0x0000F000)
#define PINMUX_LCDK_LEDS_D6_GPIO_OE_VAL         (0x00008000)

// LED D7 -> GPIO0[9]
#define PINMUX_LCDK_LEDS_D7_GPIO_OE_REG         (0)
#define PINMUX_LCDK_LEDS_D7_GPIO_OE_MASK        (0x0F000000)
#define PINMUX_LCDK_LEDS_D7_GPIO_OE_VAL         (0x08000000)

/*----------------------------------------------------------------------*/
/* GPIOs de Pulsadores USER_1 y USER_2 en LCDK */
/*----------------------------------------------------------------------*/
// Configuración del Pinmux para usar DSP_BSL en la LCDK
// USER 1, S2 -> GPIO2[4]
// USER 2, S3 -> GPIO2[5]
#define PINMUX_LCDK_USER_1_USER_2_GPIO_IE_REG   (6)
#define PINMUX_LCDK_USER_1_USER_2_GPIO_IE_MASK  (0x0000FF00)
#define PINMUX_LCDK_USER_1_USER_2_GPIO_IE_VAL   (0x00008800)

#define PB_HW_PRESSED                   (0u)    // Por diseño de la LCDK
#define PB_HW_RELEASED                  (1u)    // Por diseño de la LCDK

#define PB_SW_PRESSED                   (1u)    // Por convención. Solo puede ser (1u) o (0u)
#define PB_SW_RELEASED                  (0u)    // Por convención. Solo puede ser (1u) o (0u)

#define PB_DEBOUNCE_MAXCOUNT            (160)

#define PB_TOGGLES_T1_BIT               (0u)
#define PB_TOGGLES_T2_BIT               (1u)
#define PB_TOGGLES_T12_BIT              (2u)

#define PB_STATES_STAT_P1BUFF_BIT       (0u)
#define PB_STATES_STAT_P1_PEDGE_BIT     (1u)
#define PB_STATES_STAT_P2BUFF_BIT       (2u)
#define PB_STATES_STAT_P2_PEDGE_BIT     (3u)
#define PB_STATES_STAT_P12_PEDGE_BIT    (4u)
#define PB_STATES_STAT_AUTO_INCDEC_BIT  (14u)
#define PB_STATES_STAT_WAIT_DELAY_BIT   (15u)

typedef uint8_t     PB_INT_TYPE;

/* Macros */
/*----------------------------------------------------------------------*/
#define CODEC_WORD_LENGTH   16      // Es fijo en 16 bits para la librería L138_LCDK_aic3106
/*----------------------------------------------------------------------*/
// Macro Typecasting CODEC a flotante
#define AIC_2_FLT(x)            ( (float)(x)  * ( 1.0 / ( (1 << CODEC_WORD_LENGTH) - 1.0) ) );
#define FLT_2_AIC(x)            ( (int16_t)(x * ( (1 << CODEC_WORD_LENGTH) - 1) ) );

#define DLU_REG_BIT_GET(reg,b_position)     ( ( reg & ( 1 << b_position ) ) >> b_position )

/*---------------------------------------------------*/
/* Internal states for every Input to debounce using dlu_debouncer */
typedef struct {
    int integral_selector;
    PB_INT_TYPE debounced;
} deb_stat;

/*----------------------------------------------------------------------*/
/* Declaración de funciones lab_dsp_utils.c */
/*----------------------------------------------------------------------*/
int DLU_write_codec_outputs(float float_out_l, float float_out_r);
void DLU_read_codec_outputs(float *float_in_l, float *float_in_r);

void DLU_led_D4_set(PB_INT_TYPE value);
void DLU_led_D5_set(PB_INT_TYPE value);
void DLU_led_D6_set(PB_INT_TYPE value);
void DLU_led_D7_set(PB_INT_TYPE value);
void DLU_leds_init(void);

PB_INT_TYPE DLU_pb_USER1_get(void);
PB_INT_TYPE DLU_pb_USER2_get(void);
void DLU_push_bottons_init(void);

PB_INT_TYPE DLU_debouncer(deb_stat *ds, PB_INT_TYPE input);
PB_INT_TYPE DLU_pb_USER1_deb_get(void);
PB_INT_TYPE DLU_pb_USER2_deb_get(void);

void DLU_pb_toggle_update(uint32_t *pb_toggles, PB_INT_TYPE pb1, PB_INT_TYPE pb2);
void DLU_pb_trim_counter_update(int32_t *pb_trim_counter,  int32_t pb_trim_counter_delay_period, int32_t pb_trim_counter_autoinc_period, int32_t pb_trim_counter_min, int32_t pb_trim_counter_max);


/*************************************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
