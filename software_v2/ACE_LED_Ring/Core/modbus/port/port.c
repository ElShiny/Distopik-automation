/*
 * FreeModbus Libary: BARE Demo Application
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id$
 */

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "port.h"
/* ----------------------- Defines ------------------------------------------*/


/* ----------------------- Static variables ---------------------------------*/
USHORT   usRegInputStart = REG_INPUT_START;
USHORT   usRegInputBuf[REG_INPUT_NREGS];

USHORT   usRegHoldingStart = REG_HOLDING_START;
USHORT   usRegHoldingBuf[REG_HOLDING_NREGS];

USHORT   usRegCoilStart = REG_COIL_START;
USHORT   usRegCoilBuf[REG_COIL_NREGS];

USHORT   usRegDiscreteStart = REG_DISCRETE_START;
USHORT   usRegDiscreteBuf[REG_DISCRETE_NREGS];
/* ----------------------- Start implementation -----------------------------*/

eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    usAddress--;

    if( ( usAddress >= REG_INPUT_START )
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ =
                ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ =
                ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }


    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                 eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    usAddress--;

    if( ( usAddress >= REG_HOLDING_START )
        && ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS )
		&& (eMode == MB_REG_READ ))
    {
        iRegIndex = ( int )( usAddress - usRegHoldingStart );
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ =
                ( unsigned char )( usRegHoldingBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ =
                ( unsigned char )( usRegHoldingBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }

    else if( (  usAddress >= REG_HOLDING_START )
    			&& ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS )
				&& (eMode == MB_REG_WRITE ))
    {
        iRegIndex = ( int )( usAddress - usRegHoldingStart );
        while( usNRegs > 0 )
        {
            usRegHoldingBuf[iRegIndex] = (*pucRegBuffer++)<< 8;
            usRegHoldingBuf[iRegIndex] |= (*pucRegBuffer++);


            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}


eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
               eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    usAddress--;

    if( ( usAddress >= REG_COIL_START )
        && ( usAddress + usNCoils <= REG_COIL_START + REG_COIL_NREGS )
		&& (eMode == MB_REG_READ ))
    {
        iRegIndex = ( int )( usAddress - usRegCoilStart );
        while( usNCoils > 0 )
        {
            *pucRegBuffer++ =
                ( unsigned char )( usRegCoilBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ =
                ( unsigned char )( usRegCoilBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNCoils--;
        }
    }

    else if( (  usAddress >= REG_COIL_START )
    			&& ( usAddress + usNCoils <= REG_COIL_START + REG_COIL_NREGS )
				&& (eMode == MB_REG_WRITE ))
    {
        iRegIndex = ( int )( usAddress - usRegCoilStart );
        while( usNCoils > 0 )
        {
        	usRegCoilBuf[iRegIndex] = (*pucRegBuffer++) << 8;
        	usRegCoilBuf[iRegIndex] |= (*pucRegBuffer++);


            iRegIndex++;
            usNCoils--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    usAddress--;

    if( ( usAddress >= REG_DISCRETE_START )
        && ( usAddress + usNDiscrete <= REG_DISCRETE_START + REG_DISCRETE_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegDiscreteStart );
        while( usNDiscrete > 0 )
        {
            *pucRegBuffer++ =
                ( unsigned char )( usRegDiscreteBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ =
                ( unsigned char )( usRegDiscreteBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNDiscrete--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}
