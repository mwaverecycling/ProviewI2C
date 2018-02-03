/* 
 * Proview   $Id$
 * Copyright (C) 2005 SSAB Oxelösund.
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with the program, if not, write to the Free Software 
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "pwr.h"
#include "pwr_basecomponentclasses.h"
#include "pwr_cvoli2c.h"
#include "rt_io_base.h"
#include "rt_io_card_init.h"
#include "rt_io_card_close.h"
#include "rt_io_card_read.h"
#include "rt_io_card_write.h"
#include "rt_io_msg.h"

#include "devices/pca9555.h"

// Init method
static pwr_tStatus IoCardInit(io_tCtx ctx, io_sAgent *ap, io_sRack *rp, io_sCard *cp)
{
    pwr_sClass_I2C_Adapter *rack = (pwr_sClass_I2C_Adapter *)rp->op;
    pwr_tInt32 i2cd = rack->FileDescriptor;

    pwr_sClass_I2C_PCA9555 *card = (pwr_sClass_I2C_PCA9555 *)cp->op;
    pwr_tInt32 addr = card->I2C_Address;

    unsigned char config[2] = { 0x00, 0x00 };

    int i;
    for(i = 0; i < cp->ChanListSize; i++)
    {
        if(cp->chanlist[i].ChanClass == pwr_cClass_ChanDi) {
            config[i >= 8] |= (1 << (i % 8));
        }
    }
    int status = pca9555_config_write(i2cd, addr, config);

    return status <= 0 ? -1 : IO__SUCCESS;
}
// Close method
static pwr_tStatus IoCardClose(io_tCtx ctx, io_sAgent *ap, io_sRack *rp, io_sCard *cp)
{
    pwr_sClass_I2C_Adapter *rack = (pwr_sClass_I2C_Adapter *)rp->op;
    pwr_tInt32 i2cd = rack->FileDescriptor;

    pwr_sClass_I2C_PCA9555 *card = (pwr_sClass_I2C_PCA9555 *)cp->op;
    pwr_tInt32 addr = card->I2C_Address;

    unsigned char buffer[2] = { 0x00, 0x00 };
    int status = pca9555_write_output(i2cd, addr, buffer);

    return status <= 0 ? -1 : IO__SUCCESS;
}
// Read Method
static pwr_tStatus IoCardRead(io_tCtx ctx, io_sAgent *ap, io_sRack *rp, io_sCard *cp)
{
    pwr_sClass_I2C_Adapter *rack = (pwr_sClass_I2C_Adapter *)rp->op;
    pwr_tInt32 i2cd = rack->FileDescriptor;

    pwr_sClass_I2C_PCA9555 *card = (pwr_sClass_I2C_PCA9555 *)cp->op;
    pwr_tInt32 addr = card->I2C_Address;

    unsigned char buffer[2] = { 0x00, 0x00 };
    int status = pca9555_read_input(i2cd, addr, buffer);
    if(status <= 0) { return -1; }
    for(i = 0; i < cp->ChanListSize; i++)
    {
        if(cp->chanlist[i].ChanClass == pwr_cClass_ChanDi) {
            *(pwr_tBoolean *)cp->chanlist[i].vbp = (buffer[i >= 8] & (1 << (i % 8))) > 0;
        }
    }
    return IO__SUCCESS;
}
// Write method
static pwr_tStatus IoCardWrite(io_tCtx ctx, io_sAgent *ap, io_sRack *rp, io_sCard *cp)
{
    pwr_sClass_I2C_Adapter *rack = (pwr_sClass_I2C_Adapter *)rp->op;
    pwr_tInt32 i2cd = rack->FileDescriptor;

    pwr_sClass_I2C_PCA9555 *card = (pwr_sClass_I2C_PCA9555 *)cp->op;
    pwr_tInt32 addr = card->I2C_Address;

    unsigned char buffer[2] = { 0x00, 0x00 };
    
    for(int i = 0; i < cp->ChanListSize; i++) {
        if(cp->chanlist[i].ChanClass == pwr_cClass_ChanDo) {
            buffer[i >= 8] |= (1 << (i % 8));
        }
    }

    int status = pca9555_write_output(i2cd, addr, buffer);
    return status <= 0 ? -1 : IO__SUCCESS;
}
// Every method should be registred here
pwr_dExport pwr_BindIoUserMethods(I2C_PCA9555) = {
    pwr_BindIoUserMethod(IoCardInit),
    pwr_BindIoUserMethod(IoCardClose),
    pwr_BindIoUserMethod(IoCardRead),
    pwr_BindIoUserMethod(IoCardWrite),
    pwr_NullMethod
};
