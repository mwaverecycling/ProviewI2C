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

typedef struct {
    int socket;
}   io_sLocal;

// Init method
static pwr_tStatus IoCardInit(io_tCtx ctx, io_sAgent *ap, io_sRack *rp, io_sCard *cp)
{
    char config[2];
    
    cp->Address;
    
    errh_Info("CustomEthernetIO Initializing...");
    local->socket = init_connection("10.0.0.108", 6969);
    errh_Info("   socket fd: %d", local->socket);
    return local->socket < 0 ? -1 : IO__SUCCESS;
}
// Close method
static pwr_tStatus IoCardClose(io_tCtx ctx, io_sAgent *ap, io_sRack *rp, io_sCard *cp)
{
    io_sLocal *local = cp->Local;
    close(local->socket);
    free(cp->Local);
    return IO__SUCCESS;
}
// Read Method
static pwr_tStatus IoCardRead(io_tCtx ctx, io_sAgent *ap, io_sRack *rp, io_sCard *cp)
{
    io_sLocal *local = cp->Local;
    //pwr_sClass_CustomEthernetIO *op = (pwr_sClass_CustomEthernetIO *)cp->op;

    i2ccmd_packet_t packet;
    packet.actor[0] = 'i';
    packet.actor[1] = 'o';
    packet.actor[2] = '0';
    packet.actor[3] = '1';
    packet.cmd[0] = 'r';
    packet.cmd[1] = 'r';
    packet.data[0] = 0;
    packet.data[1] = 0;
    int sent = send_data(local->socket, &packet, sizeof(packet));
    if(sent != sizeof(packet)) { return -1; }
    int recieved = recv_data(local->socket, &packet.data[0], 2);
    if(recieved != 2) { return -1; }
    
    unsigned short data = (unsigned short)packet.data[1] << 8 | packet.data[0];
    errh_Info("CustomEthernetIO Read: %d", data);
    for(int i = 0; i < 16; i++) {
        if(cp->chanlist[i].ChanClass == pwr_cClass_ChanDi) {
            *(pwr_tBoolean *)cp->chanlist[i].vbp = (data & (1 << i)) > 0;
        }
    }
    
    return IO__SUCCESS;
}
// Write method
static pwr_tStatus IoCardWrite(io_tCtx ctx, io_sAgent *ap, io_sRack *rp, io_sCard *cp)
{
    io_sLocal *local = cp->Local;

    i2ccmd_packet_t packet;
    packet.actor[0] = 'i';
    packet.actor[1] = 'o';
    packet.actor[2] = '0';
    packet.actor[3] = '1';
    packet.cmd[0] = 'w';
    packet.cmd[1] = 'w';
    packet.data[0] = 0;
    packet.data[1] = 0;
    
    for(int i = 0; i < 16; i++) {
        if(cp->chanlist[i].ChanClass == pwr_cClass_ChanDo) {
            if(i < 8) {
                packet.data[0] |= *(pwr_tBoolean *)cp->chanlist[i].vbp << i;
            } else {
                packet.data[1] |= *(pwr_tBoolean *)cp->chanlist[i].vbp << (i - 8);
            }
        }
    }
    
    int sent = send_data(local->socket, &packet, sizeof(packet));
    errh_Info("CustomEthernetIO Wrote length: %d", sent);
    if(sent != sizeof(packet)) { return -1; }
    
    return IO__SUCCESS;
}
// Every method should be registred here
pwr_dExport pwr_BindIoUserMethods(CustomEthernetIO) = {
    pwr_BindIoUserMethod(IoCardInit),
    pwr_BindIoUserMethod(IoCardClose),
    pwr_BindIoUserMethod(IoCardRead),
    pwr_BindIoUserMethod(IoCardWrite),
    pwr_NullMethod
};
