/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id$
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "tim.h"
#include "gpio.h"

/* ----------------------- static functions ---------------------------------*/
//#define USE_HAL_TIM_REGISTER_CALLBACKS 1
//void prvvTIMERExpiredISR( TIM_HandleTypeDef *htim );

/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{
	LL_TIM_DisableCounter(TIM16);
	LL_TIM_DisableIT_UPDATE(TIM16);
	LL_TIM_ClearFlag_UPDATE(TIM16);

    return TRUE;
}


inline void
vMBPortTimersEnable(  )
{
    /* Enable the timer with the timeout passed to xMBPortTimersInit( ) */
	TIM16->CNT = 0;
	LL_TIM_ClearFlag_UPDATE(TIM16);
	LL_TIM_EnableIT_UPDATE(TIM16);
	LL_TIM_EnableCounter(TIM16);

}

inline void
vMBPortTimersDisable(  )
{
    /* Disable any pending timers. */
	LL_TIM_DisableCounter(TIM16);
	LL_TIM_DisableIT_UPDATE(TIM16);
	LL_TIM_ClearFlag_UPDATE(TIM16);
    LL_GPIO_SetOutputPin(TP2_GPIO_Port, TP2_Pin);

}

/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */
void prvvTIMERExpiredISR( void )
{
    ( void )pxMBPortCBTimerExpired(  );


}

