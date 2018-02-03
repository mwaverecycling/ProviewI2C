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

#include "i2cfunc.h"

// Init method
static pwr_tStatus IoRackInit(io_tCtx ctx, io_sAgent *ap, io_sRack *rp)
{
    pwr_sClass_I2C_Adapter *rack = (pwr_sClass_I2C_Adapter *)rp->op;
    pwr_tString80 filename = rack->Filename;

    int i2cd = i2c_init_file(filename);
    rack->FileDescriptor = i2cd;

    return i2cd < 0 ? -1 : IO__SUCCESS;
}
// Close method
static pwr_tStatus IoRackClose(io_tCtx ctx, io_sAgent *ap, io_sRack *rp)
{
    pwr_sClass_I2C_Adapter *rack = (pwr_sClass_I2C_Adapter *)rp->op;
    pwr_tInt32 i2cd = rack->FileDescriptor;

    int status = i2c_close(i2cd);

    return status < 0 ? -1 : IO__SUCCESS;
}

// Every method should be registred here
pwr_dExport pwr_BindIoUserMethods(I2C_Adapter) = {
    pwr_BindIoUserMethod(IoRackInit),
    pwr_BindIoUserMethod(IoRackClose),
    pwr_NullMethod
};
