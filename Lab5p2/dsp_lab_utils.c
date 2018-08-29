/***************************************************************************//**
* \file     dsp_lab_utils.c  Version 1.1
*
* \brief    Este archivo contiene funciones de propósito general para simplificar
*           tareas para varias experiencias.
*
* \authors  Gonzalo Carrasco
*******************************************************************************/

/***************************************************************************//**
**      HEADER FILES
*******************************************************************************/
#include <L138_LCDK_aic3106_init.h>  // Es mandatorio contar con las definiciones del la librería L138_LCDK_aic3106_init
#include "dsp_lab_utils.h"      // Encabezado de lab_dsp_utils.c
#include <c6x.h>
/******************************************************************************
**      MACRO DEFINITIONS
*******************************************************************************/
//#define CONSTANT  1 // If necessary, because they should be in code_template.h

#define TICTOC_TSC_NIT     (TSCL = 0;)
#define TIC_TSC(cu,cl) cl = TSCL; cu = TSCH;
#define TOC_TSC(cu,cl) cl = TSCL; cu = TSCH;

/***************************************************************************//**
**      GLOBAL VARIABLES
*******************************************************************************/
//#include "lab_dsp_utils_gvars.h" // If there are too many variables, they will be in a .h
deb_stat pb_user1_deb_stat = {0,0};
deb_stat pb_user2_deb_stat = {0,0};

uint32_t pb_states_stat = 0;
uint32_t pb_trim_counter_delay_count = 0;
uint32_t pb_trim_counter_autoincdec_count = 0;

extern int32_t pb_trim_counter;


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
int DLU_write_codec_outputs(float float_out_l, float float_out_r)
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
void DLU_read_codec_outputs(float *float_in_l, float *float_in_r)
{
    AIC31_data_type codec_data;
    /*----------------------------------------------------------------------*/
    codec_data.uint = input_sample();
    *float_in_l = AIC_2_FLT( codec_data.channel[LEFT]  );
    *float_in_r = AIC_2_FLT( codec_data.channel[RIGHT] );
}

/***************************************************************************//**
*   \brief  Establece estado del LED D4
*
*   \param value    : 0 - Apagado (OUTPUT_LOW)
*                   : 1 - Encendido (OUTPUT_HIGH)
*
*   \return Void.
*******************************************************************************/
void DLU_led_D4_set(PB_INT_TYPE value){
    // LED D4 -> GPIO6[13]
    GPIO_setOutput(GPIO_BANK6, GPIO_PIN13, value);
}

/***************************************************************************//**
*   \brief  Establece estado del LED D5
*
*   \param value    : 0 - Apagado (OUTPUT_LOW)
*                   : 1 - Encendido (OUTPUT_HIGH)
*
*   \return Void.
*******************************************************************************/
void DLU_led_D5_set(PB_INT_TYPE value){
    // LED D5 -> GPIO6[12]
    GPIO_setOutput(GPIO_BANK6, GPIO_PIN12, value);
}

/***************************************************************************//**
*   \brief  Establece estado del LED D6
*
*   \param value    : 0 - Apagado (OUTPUT_LOW)
*                   : 1 - Encendido (OUTPUT_HIGH)
*
*   \return Void.
*******************************************************************************/
void DLU_led_D6_set(PB_INT_TYPE value){
    // LED D6 -> GPIO2[12]
    GPIO_setOutput(GPIO_BANK2, GPIO_PIN12, value);
}

/***************************************************************************//**
*   \brief  Establece estado del LED D7
*
*   \param value    : 0 - Apagado (OUTPUT_LOW)
*                   : 1 - Encendido (OUTPUT_HIGH)
*
*   \return Void.
*******************************************************************************/
void DLU_led_D7_set(PB_INT_TYPE value){
    // LED D7 -> GPIO0[9]
    GPIO_setOutput(GPIO_BANK0, GPIO_PIN9, value);
}

/***************************************************************************//**
*   \brief  Configura El pinmux y la dirección de los GPIOs correspondientes
*           a los LEDs D4, D5, D6 y D7 como salidas.
*
*   \param Void.
*
*   \return Void.
*******************************************************************************/
void DLU_leds_init(void){
    /*----------------------------------------------------------------------*/
    // LED D4 -> GPIO6[13]
    // LED D5 -> GPIO6[12]
    EVMOMAPL138_pinmuxConfig(PINMUX_LCDK_LEDS_D4_D5_GPIO_OE_REG, PINMUX_LCDK_LEDS_D4_D5_GPIO_OE_MASK, PINMUX_LCDK_LEDS_D4_D5_GPIO_OE_VAL);
    GPIO_setDir(GPIO_BANK6, GPIO_PIN13, GPIO_OUTPUT);
    GPIO_setDir(GPIO_BANK6, GPIO_PIN12, GPIO_OUTPUT);
    /*----------------------------------------------------------------------*/
    // LED D6 -> GPIO2[12]
    EVMOMAPL138_pinmuxConfig(PINMUX_LCDK_LEDS_D6_GPIO_OE_REG, PINMUX_LCDK_LEDS_D6_GPIO_OE_MASK, PINMUX_LCDK_LEDS_D6_GPIO_OE_VAL);
    GPIO_setDir(GPIO_BANK2, GPIO_PIN12, GPIO_OUTPUT);
    /*----------------------------------------------------------------------*/
    // LED D7 -> GPIO0[9]
    EVMOMAPL138_pinmuxConfig(PINMUX_LCDK_LEDS_D7_GPIO_OE_REG, PINMUX_LCDK_LEDS_D7_GPIO_OE_MASK, PINMUX_LCDK_LEDS_D7_GPIO_OE_VAL);
    GPIO_setDir(GPIO_BANK0, GPIO_PIN9, GPIO_OUTPUT);
}


/***************************************************************************//**
*   \brief  Lee el estado del pulsador USER 1
*
*   \param Void.
*
*   \return PB_INT_TYPE     1 : pulsador presionado.
*                       0 : pulsador liberado
*******************************************************************************/
PB_INT_TYPE DLU_pb_USER1_get(void){
    PB_INT_TYPE value;
    // USER 1, S2 -> GPIO2[4]
    GPIO_getInput(GPIO_BANK2, GPIO_PIN4, &value);
    if (value == PB_HW_PRESSED)
        return(PB_SW_PRESSED);
    else
        return(PB_SW_RELEASED);
}

/***************************************************************************//**
*   \brief  Lee el estado del pulsador USER 2
*
*   \param Void.
*
*   \return PB_INT_TYPE     1 : pulsador presionado.
*                       0 : pulsador liberado
*******************************************************************************/
PB_INT_TYPE DLU_pb_USER2_get(void){
    PB_INT_TYPE value;
    // USER 2, S3 -> GPIO2[5]
    GPIO_getInput(GPIO_BANK2, GPIO_PIN5, &value);
    if (value == PB_HW_PRESSED)
        return(PB_SW_PRESSED);
    else
        return(PB_SW_RELEASED);
}

/***************************************************************************//**
*   \brief  Configura El pinmux y la dirección de los GPIOs correspondientes
*           a los pulsadores S2 (USER 1) y S3 (USER 2) como entradas.
*
*   \param Void.
*
*   \return Void.
*******************************************************************************/
void DLU_push_bottons_init(void){
    /*----------------------------------------------------------------------*/
    // USER 1, S2 -> GPIO2[4]
    // USER 2, S3 -> GPIO2[5]
    EVMOMAPL138_pinmuxConfig(PINMUX_LCDK_USER_1_USER_2_GPIO_IE_REG, PINMUX_LCDK_USER_1_USER_2_GPIO_IE_MASK, PINMUX_LCDK_USER_1_USER_2_GPIO_IE_VAL);
    GPIO_setDir(GPIO_BANK2, GPIO_PIN4, GPIO_INPUT);
    GPIO_setDir(GPIO_BANK2, GPIO_PIN5, GPIO_INPUT);
}

/***************************************************************************//**
*   \brief  This function debounces the input signal. This function must
*           be called at a regular rate so as to create a sampled version
*           of the input and filtered here. The debounced output only changes
*           when 'integral_select' has accumulated more than DEBOUNCE_MAXCOUNT
*           number of readings of the same state.
*
*           Every signal to be debounce using this function, must have its
*           own structure of inner states with 'deb_sta' structure.
*
*   \param  input : integer representing the binary input to be debounced.
*   \param  *ds : pointer to the structure that stores the states for the
*                 processed signal.
*   \return debounced : binary output already debounced.
*******************************************************************************/
PB_INT_TYPE DLU_debouncer(deb_stat *ds, PB_INT_TYPE input){
    /* Integration with saturation */
    if (input)
        if (++ds->integral_selector > PB_DEBOUNCE_MAXCOUNT){
            ds->integral_selector = PB_DEBOUNCE_MAXCOUNT;
            ds->debounced = PB_SW_PRESSED;
            return ds->debounced;
        }
        else
            return ds->debounced;
    else
        if (--ds->integral_selector < (-PB_DEBOUNCE_MAXCOUNT)){
            ds->integral_selector = -PB_DEBOUNCE_MAXCOUNT;
            ds->debounced = PB_SW_RELEASED;
            return ds->debounced;
        }
        else
            return ds->debounced;
}

/***************************************************************************//**
*   \brief  Retorna el estado del pulsador USER 1, con filtro antirebote.
*
*   \param Void.
*
*   \return PB_INT_TYPE     1 : pulsador presionado.
*                       0 : pulsador liberado
*******************************************************************************/
PB_INT_TYPE DLU_pb_USER1_deb_get(void){
    PB_INT_TYPE ret;
    ret = DLU_debouncer(&pb_user1_deb_stat, DLU_pb_USER1_get() );
    return(ret);
}

/***************************************************************************//**
*   \brief  Retorna el estado del pulsador USER 2, con filtro antirebote.
*
*   \param Void.
*
*   \return PB_INT_TYPE     1 : pulsador presionado.
*                       0 : pulsador liberado
*******************************************************************************/
PB_INT_TYPE DLU_pb_USER2_deb_get(void){
    PB_INT_TYPE ret;
    ret = DLU_debouncer(&pb_user2_deb_stat, DLU_pb_USER2_get() );
    return(ret);
}

/***************************************************************************//**
*   \brief  Función que interpreta la señal de dos pulsadores para generar
*           y modificar la varable de estado "pb_toggles".
*           "pb_toggles" cuenta con tres bits:
*           PB_TOGGLES_T1_BIT cambia entre 0 y 1 con presionar pb1
*           PB_TOGGLES_T2_BIT cambia entre 0 y 1 con presionar pb2
*           PB_TOGGLES_T12_BIT  cambia entre 0 y 1 con presionar pb1 y  pb2 a la vez.
*
*   \param  pb_toggles :    puntero a variable entera sin signo para registrar bits
*                           de estado.
*           pb1 :           señal del pulsador 1 (debe estár filtrada contra rebotes)
*           pb2 :           señal del pulsador 1 (debe estár filtrada contra rebotes)
*
*   \return Void
*******************************************************************************/
void DLU_pb_toggle_update(uint32_t *pb_toggles, PB_INT_TYPE pb1, PB_INT_TYPE pb2){
    /*----------------------------------------------------------------------*/
    // Detectando canto de subida en pb1
    if ( ( pb1 == PB_SW_PRESSED ) && ( DLU_REG_BIT_GET(pb_states_stat, PB_STATES_STAT_P1BUFF_BIT) == PB_SW_RELEASED ) )
        pb_states_stat |= (1 << PB_STATES_STAT_P1_PEDGE_BIT );
    else
        pb_states_stat &= ~(1 << PB_STATES_STAT_P1_PEDGE_BIT );
    /*----------------------------------------------------------------------*/
    // Actualización del estado Toggle 1
    if ( pb_states_stat & ( 1 << PB_STATES_STAT_P1_PEDGE_BIT ) )
        *pb_toggles ^= ( 1 << PB_TOGGLES_T1_BIT );
    /*----------------------------------------------------------------------*/
    /*----------------------------------------------------------------------*/
    // Detectando canto de subida en pb2
    if ( ( pb2 == PB_SW_PRESSED ) && ( ( DLU_REG_BIT_GET(pb_states_stat, PB_STATES_STAT_P2BUFF_BIT) == PB_SW_RELEASED ) ) )
        pb_states_stat |= (1 << PB_STATES_STAT_P2_PEDGE_BIT );
    else
        pb_states_stat &= ~(1 << PB_STATES_STAT_P2_PEDGE_BIT );
    /*----------------------------------------------------------------------*/
    // Actualización del estado Toggle 1
    if ( pb_states_stat & ( 1 << PB_STATES_STAT_P2_PEDGE_BIT ) )
        *pb_toggles ^= ( 1 << PB_TOGGLES_T2_BIT );
    /*----------------------------------------------------------------------*/
    /*----------------------------------------------------------------------*/
    // Detectando canto de presionar pb1 y pb2
    if (       ( pb1 == PB_SW_PRESSED )
            && ( pb2 == PB_SW_PRESSED )
            && (    ( DLU_REG_BIT_GET(pb_states_stat, PB_STATES_STAT_P1BUFF_BIT) == PB_SW_RELEASED ) ||
                    ( DLU_REG_BIT_GET(pb_states_stat, PB_STATES_STAT_P2BUFF_BIT) == PB_SW_RELEASED )    )  )
        pb_states_stat |= (1 << PB_STATES_STAT_P12_PEDGE_BIT );
    else
        pb_states_stat &= ~(1 << PB_STATES_STAT_P12_PEDGE_BIT );
    /*----------------------------------------------------------------------*/
    // Actualización del estado Toggle 12
    if ( pb_states_stat & ( 1 << PB_STATES_STAT_P12_PEDGE_BIT ) )
        *pb_toggles ^= ( 1 << PB_TOGGLES_T12_BIT );
    /*----------------------------------------------------------------------*/
    /*----------------------------------------------------------------------*/
    // Limpiando buffers
    pb_states_stat &= ~(1 << PB_STATES_STAT_P1BUFF_BIT );
    pb_states_stat &= ~(1 << PB_STATES_STAT_P2BUFF_BIT );
    // Actualización de buffers
    pb_states_stat |= ( (uint32_t)pb1 << PB_STATES_STAT_P1BUFF_BIT );
    pb_states_stat |= ( (uint32_t)pb2 << PB_STATES_STAT_P2BUFF_BIT );
}

/***************************************************************************//**
*   \brief  Función que interpreta los estados "pb_states_stat" internos de la
*           función DLU_pb_toggle_update() para modificar el valor de cuenta en
*           "pb_trim_counter". Este valor entero se puede incrementar o decrementar
*           entre un rango máximo y mínimo usando dos pulsadores.
*
*           SE DEBE INVOCAR SIEMPRE DESPUÉS DE INVOCAR LA FUNCIÓN DLU_pb_toggle_update().
*
*           Al presionar pb1, la cuenta en "pb_trim_counter" se decrementa en 1.
*           Al presionar pb2, la cuenta en "pb_trim_counter" se inrementa en 1.
*
*           AL presionar uno de los pulsador por más de PB_TRIM_COUNTER_DELAY_PERIOD
*           llamados de función, se ingresa al modo autoincdec, que permite
*           incrementar o decrementar "pb_trim_counter" cada
*           PB_TRIM_COUNTER_AUTOINC_PERIOD llamados de función.
*
*   \param  pb_trim_counter :       puntero a variable entera con signo para registrar
                                    la cuenta de ajuste.
*                                   de estado.
*           pb_trim_counter_min :   valor de saturación mínima de la cuenta
*           pb_trim_counter_max :   valor de saturación máxima de la cuenta
*
*           "pb_trim_counter_max" debe ser siempre mayor que "pb_trim_counter_min", de
*           caso contrario, "pb_trim_counter" tomará siempre el valor "pb_trim_counter_min".
*
*   \return Void
*******************************************************************************/
void DLU_pb_trim_counter_update(int32_t *pb_trim_counter, int32_t pb_trim_counter_delay_period, int32_t pb_trim_counter_autoinc_period, int32_t pb_trim_counter_min, int32_t pb_trim_counter_max){
    // Solo un pulsador prsonado a la vez iniciará la cuenta del retardo para incrementar
    if ( DLU_REG_BIT_GET(pb_states_stat, PB_STATES_STAT_P1BUFF_BIT) ^ DLU_REG_BIT_GET(pb_states_stat, PB_STATES_STAT_P2BUFF_BIT) )
    {
        if ( !DLU_REG_BIT_GET(pb_states_stat, PB_STATES_STAT_AUTO_INCDEC_BIT) )
            pb_trim_counter_delay_count++;

        if (pb_trim_counter_delay_count > pb_trim_counter_delay_period)
            pb_states_stat |= ( 1 << PB_STATES_STAT_AUTO_INCDEC_BIT );
    }
    else
    {
        pb_trim_counter_delay_count = 0;
        pb_states_stat &= ~( 1 << PB_STATES_STAT_AUTO_INCDEC_BIT );
    }
    /*----------------------------------------------------------------------*/
    // Autoincremento o por pulsos manuales
    if ( DLU_REG_BIT_GET(pb_states_stat, PB_STATES_STAT_AUTO_INCDEC_BIT) )
    {
        // Modo autoincremento
        pb_trim_counter_autoincdec_count++;
        if ( pb_trim_counter_autoincdec_count > pb_trim_counter_autoinc_period )
        {
            pb_trim_counter_autoincdec_count = 0;
            // Autoincrementa solo cuando ha transcurrido el periodo de autoincremento
            if ( DLU_REG_BIT_GET(pb_states_stat, PB_STATES_STAT_P2BUFF_BIT) )
                (*pb_trim_counter)++;
            if ( DLU_REG_BIT_GET(pb_states_stat, PB_STATES_STAT_P1BUFF_BIT) )
                (*pb_trim_counter)--;
        }
    }
    else
    {
        // Modo pulsos manuales
        if ( DLU_REG_BIT_GET(pb_states_stat, PB_STATES_STAT_P2_PEDGE_BIT) )
            (*pb_trim_counter)++;
        if ( DLU_REG_BIT_GET(pb_states_stat, PB_STATES_STAT_P1_PEDGE_BIT) )
            (*pb_trim_counter)--;
    }
    /*----------------------------------------------------------------------*/
    // Saturación
    if ( *pb_trim_counter > pb_trim_counter_max )
        *pb_trim_counter = pb_trim_counter_max;
    if ( *pb_trim_counter < pb_trim_counter_min )
        *pb_trim_counter = pb_trim_counter_min;
}


/***************************************************************************//**
*   \brief Función que inicializa el Time Stamp Counter de la CPU
*
*   \param Void.
*
*   \return Void.
*******************************************************************************/
void DLU_tictoc_init(void){
    TSCL = 0;
}

/***************************************************************************//**
*   \brief Función que registra time-stamp inicial
*
*   \param *tsc_samp :   es el puntero a un entero sin signo de 64
*                       bits donde de registrará el tiempo inicial (con TIC) y
*                       tiempo final (con TOC).
*
*   \return Void.
*******************************************************************************/
void DLU_tic(unsigned long long *tsc_samp){
    unsigned int aux;
    aux = TSCL;
    *tsc_samp = ((unsigned long long)TSCH << 32);
    *tsc_samp += (unsigned long long)aux;
}

/***************************************************************************//**
*   \brief Función que captura el time-stamp actual y calcula la diferencia con
*           el valor inicial registrado con TIC, dejando la diferencia en cuentas
*           entre TIC y TOC en la variable apuntada por el parámetro de esta
*           función.
*
*   \param *tsc_samp : es el puntero a un entero sin signo de 64
*                       bits donde se registrará la diferencia en cuentas entre
*                       TIC y TOC. Las cuentas ocurren a la frecuencia del CPU.
*
*   \return Void.
*******************************************************************************/
void DLU_toc(unsigned long long *tsc_samp){
    unsigned int aux;
    unsigned long long aux_long;

    aux = TSCL;
    aux_long = ((unsigned long long)TSCH << 32);
    aux_long += (unsigned long long)aux;

    *tsc_samp = aux_long - *tsc_samp;
}
