/***************************************************************************//**
* \file     levinson.c
*
* \brief    Resuelve la ecuación matricial para encontrar el vector 'alpha'
*           R * alpha = r
*
* \authors  ???, Gonzalo Carrasco
*******************************************************************************/

/***************************************************************************//**
**      HEADER FILES
*******************************************************************************/
#include <stdlib.h>
#include "levinson.h"
/******************************************************************************
**      MACRO DEFINITIONS
*******************************************************************************/

/***************************************************************************//**
**      GLOBAL VARIABLES
*******************************************************************************/
float* e;
float* k;
float* alpha_prev;
/***************************************************************************//**
**      INTERNAL FUNCTION DEFINITIONS
*******************************************************************************/

/***************************************************************************//**
*   \brief  Inicialización de variables para función levinson()
*
*   \param order : Tamaño de los vectores
*
*   \return Void.
*******************************************************************************/
/* Inicialización de variables */
void levinson_init(short length){
    e          = (float *)malloc((length+1)*sizeof(float));
    k          = (float *)malloc((length+1)*sizeof(float));
    alpha_prev = (float *)malloc((length+1)*sizeof(float));
}

/***************************************************************************//**
*   \brief  Algoritmo de Levinson-Durbin
*
*   \param alpha    : Vector de parámetros de salida identificados
*   \param r        : Vector de autocorrelaciones desde r(0) a r(length-1)
*
*   \return Void.
*******************************************************************************/
void levinson(float* alpha, float* r, short length){
    int   i=1,j=1;
    float sum=0.0;
    short loop;

    // Initialising the Array variables
    for (loop=0;loop<=length;loop++)
    {
        e[loop]=0;
        k[loop]=0;
        alpha[loop]=0;
        alpha_prev[loop]=0;
    }
    // End initalising
    e[0]=r[0];
    // Iterate with i=1 to length as iteration variable
    i=1;
    for(i=1;i<=length;i++)
    {
        //-----------------------------------
        // start step 1
        sum=0;
        for (j=1;j<=i-1;j++)
            sum+=alpha_prev[j]*r[i-j];

        k[i]=(r[i]-sum)/e[i-1];
        // End step 1
        //-----------------------------------
        //start step 2
        alpha[i]=k[i];
        for(j=1;j<=i-1;j++)
            alpha[j]=alpha_prev[j]- ( k[i]*alpha_prev[i-j] ) ;
        //end step 2
        //-----------------------------------
        //start step 3
        e[i]=e[i-1]*(1-(k[i]*k[i]));
        //end step 3
        for( loop=0;loop<=length;loop++)
            alpha_prev[loop]=alpha[loop];
    }// End For levinson
    /*----------------------------------------------------------------------*/
    // Reformateo de constantes alpha = [1 -alpha(1) -alpha(2) ... -alpha(length-1)]
    for( loop=0;loop<=length;loop++)
        alpha[loop]=-1*alpha[loop];

    alpha[0]=1;

}
