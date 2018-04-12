//referred from http://wiki.osdev.org
#include <sys/defs.h>
#include <sys/kprintf.h>
#include "common.h"
#include <sys/mmu.h>
#include "enumpci.h"
#include <sys/ahci.h>
#define	AHCI_BASE	0x400000	
#define ATA_DEV_BUSY 0x80
#define ATA_DEV_DRQ 0x08

//void memset (void *add,int val,int len)
//{
//	char *buf=add;
//	for(int i=0;i<len;i++)
//	{
//		*buf=val;
//		buf++;
//	}
//}
int find_cmdslot(hba_port_t *port)
{
	uint64_t slots = (port->sact | port->ci);
	for (int i=0; i<32; i++){
		if ((slots&1) == 0)
			return i;
		slots >>= 1;
	}
	return -1;
}
int write_to_disk(hba_port_t *port, uint32_t startl, uint32_t starth, uint32_t count, uint32_t *buf)
{
        port->is_rwc = (uint32_t)-1;            // Clear pending interrupt bits
        int spin = 0; // Spin lock timeout counter
        int slot = find_cmdslot(port);
        int i=0;
        if (slot == -1)
                return 0;

        hba_cmd_header_t *cmdheader = (hba_cmd_header_t*)port->clb;
        cmdheader += slot;
        cmdheader->cfl = sizeof(fis_reg_h2d_t)/sizeof(uint32_t);        // Command FIS size
        cmdheader->w = 0;               // Read from device
        cmdheader->prdtl = (uint16_t)((count-1)>>4) + 1;        // PRDT entries count

        hba_cmd_tbl_t *cmdtbl = (hba_cmd_tbl_t*)(cmdheader->ctba);
        memset(cmdtbl, 0, sizeof(hba_cmd_tbl_t)+(cmdheader->prdtl-1)*sizeof(hba_prdt_entry_t));

        // 8K bytes (16 sectors) per PRDT
        for (int i=0; i<cmdheader->prdtl-1; i++)
        {
                cmdtbl->prdt_entry[i].dba = (uint64_t)buf;
                cmdtbl->prdt_entry[i].dbc = 8*1024;     // 8K bytes
                cmdtbl->prdt_entry[i].i = 1;
                buf += 4*1024;  // 4K words
                count -= 16;    // 16 sectors
        }
        // Last entry
        cmdtbl->prdt_entry[i].dba = (uint64_t)buf;
        cmdtbl->prdt_entry[i].dbc = count<<9;   // 512 bytes per sector
        cmdtbl->prdt_entry[i].i = 1;

        // Setup command
        fis_reg_h2d_t *cmdfis = (fis_reg_h2d_t*)(&cmdtbl->cfis);

        cmdfis->fis_type = FIS_TYPE_REG_H2D;
        cmdfis->c = 1;  // Command
        cmdfis->command = ATA_CMD_WRITE_DMA_EX;

        cmdfis->lba0 = (uint8_t)startl;
        cmdfis->lba1 = (uint8_t)(startl>>8);
        cmdfis->lba2 = (uint8_t)(startl>>16);
        cmdfis->device = 1<<6;  // LBA mode

        cmdfis->lba3 = (uint8_t)(startl>>24);
        cmdfis->lba4 = (uint8_t)starth;
        cmdfis->lba5 = (uint8_t)(starth>>8);

        cmdfis->count = (uint8_t)count;

        // The below loop waits until the port is no longer busy before issuing a new command
        while ((port->tfd & (ATA_DEV_BUSY | ATA_DEV_DRQ)) && spin < 1000000)
        {
                spin++;
        }
        if (spin == 1000000)
        {
                //trace_ahci("Port is hung\n");
                return 0;
        }
       port->ci = 1<<slot;     // Issue command

        // Wait for completion
        while (1)
        {
                // In some longer duration reads, it may be helpful to spin on the DPS bit 
                // in the PxIS port field as well (1 << 5)
                if ((port->ci & (1<<slot)) == 0)
                        break;
                if (port->is_rwc & HBA_PxIS_TFES)       // Task file error
                {
                        //trace_ahci("Read disk error\n");
                        return 0;
                }
        }

        // Check again
        if (port->is_rwc & HBA_PxIS_TFES)
        {
                //trace_ahci("Read disk error\n");
                return 0;
        }

        return 1;
}

int read_from_disk(hba_port_t *port, uint32_t startl, uint32_t starth, uint32_t count, uint32_t *buf)
{
	port->is_rwc = (uint32_t)-1;		// Clear pending interrupt bits
	int spin = 0; // Spin lock timeout counter
	int slot = find_cmdslot(port);
	int i=0;
	if (slot == -1)
		return 0;
 
	hba_cmd_header_t *cmdheader = (hba_cmd_header_t*)port->clb;
	cmdheader += slot;
	cmdheader->cfl = sizeof(fis_reg_h2d_t)/sizeof(uint32_t);	// Command FIS size
	cmdheader->w = 0;		// Read from device
	cmdheader->prdtl = (uint16_t)((count-1)>>4) + 1;	// PRDT entries count
 
	hba_cmd_tbl_t *cmdtbl = (hba_cmd_tbl_t*)(cmdheader->ctba);
	memset(cmdtbl, 0, sizeof(hba_cmd_tbl_t)+(cmdheader->prdtl-1)*sizeof(hba_prdt_entry_t));
 
	// 8K bytes (16 sectors) per PRDT
	for (int i=0; i<cmdheader->prdtl-1; i++)
	{
		cmdtbl->prdt_entry[i].dba = (uint64_t)buf;
		cmdtbl->prdt_entry[i].dbc = 8*1024;	// 8K bytes
		cmdtbl->prdt_entry[i].i = 1;
		buf += 4*1024;	// 4K words
		count -= 16;	// 16 sectors
	}
	// Last entry
	cmdtbl->prdt_entry[i].dba = (uint64_t)buf;
	cmdtbl->prdt_entry[i].dbc = count<<9;	// 512 bytes per sector
	cmdtbl->prdt_entry[i].i = 1;
 
	// Setup command
	fis_reg_h2d_t *cmdfis = (fis_reg_h2d_t*)(&cmdtbl->cfis);
 
	cmdfis->fis_type = FIS_TYPE_REG_H2D;
	cmdfis->c = 1;	// Command
	cmdfis->command = ATA_CMD_READ_DMA_EX;
 
	cmdfis->lba0 = (uint8_t)startl;
	cmdfis->lba1 = (uint8_t)(startl>>8);
	cmdfis->lba2 = (uint8_t)(startl>>16);
	cmdfis->device = 1<<6;	// LBA mode
 
	cmdfis->lba3 = (uint8_t)(startl>>24);
	cmdfis->lba4 = (uint8_t)starth;
	cmdfis->lba5 = (uint8_t)(starth>>8);
 
	cmdfis->count = (uint8_t)count;
 
	// The below loop waits until the port is no longer busy before issuing a new command
	while ((port->tfd & (ATA_DEV_BUSY | ATA_DEV_DRQ)) && spin < 1000000)
	{
		spin++;
	}
	if (spin == 1000000)
	{
		kprintf("Port is hung\n");
		return 0;
	}
 
	port->ci = 1<<slot;	// Issue command
 
	// Wait for completion
	while (1)
	{
		// In some longer duration reads, it may be helpful to spin on the DPS bit 
		// in the PxIS port field as well (1 << 5)
		if ((port->ci & (1<<slot)) == 0) 
			break;
		if (port->is_rwc & HBA_PxIS_TFES)	// Task file error
		{
			kprintf("Read disk error\n");
			return 0;
		}
	}
 
	// Check again
	if (port->is_rwc & HBA_PxIS_TFES)
	{
		kprintf("Read disk error\n");
		return 0;
	}
 
	return 1;
}

void start_cmd(hba_port_t *port)
{
	while (port->cmd & HBA_PxCMD_CR);
	port->cmd|=HBA_PxCMD_FRE;
	port->cmd|=HBA_PxCMD_ST; 
}
void stop_cmd(hba_port_t *port)
{
	port->cmd &= ~HBA_PxCMD_ST;
	port->cmd &= ~HBA_PxCMD_FRE;
	while(1){
		if (port->cmd & HBA_PxCMD_FR)
			continue;
		if (port->cmd & HBA_PxCMD_CR)
		 	continue;
		break;
	}
	port->cmd&=~HBA_PxCMD_FRE;
}

void port_rebase(hba_port_t *port, int portno)
{
        stop_cmd(port);
        port->clb=AHCI_BASE+(portno<<10);
        memset((void*)(port->clb), 0, 1024);
        port->fb=AHCI_BASE+(32<<10)+(portno<<8);
        memset((void*)(port->fb), 0, 256);
        hba_cmd_header_t *cmdheader=(hba_cmd_header_t*)(port->clb);
        for (int i=0;i<32;i++){
                cmdheader[i].prdtl=8;
                cmdheader[i].ctba=AHCI_BASE+(40<<10)+(portno<<13)+(i<<8);
                memset((void*)cmdheader[i].ctba, 0, 256);
        }
        start_cmd(port);
}
uint16_t read(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset){
	uint32_t address = (uint32_t) ((uint32_t)bus << 16)| ((uint32_t)device << 11)| ((uint32_t)function << 8)| (offset & 0xfc)|((uint32_t)0x80000000);
	
	//kprintf("%x",offset);
	if(offset == 0x24){
		//kprintf("updated");
		outl(0xcf8,address);
		outl(0xcfc,0x3ebf1000);
	        uint16_t result = (uint16_t)(inl(0xcfc) >> (8* ((offset) & 2)) & 0xffff);
		//kprintf("result %x ",result);
		return result;
	}
	//uint32_t address = (uint32_t) ((uint32_t)bus << 16)| ((uint32_t)device << 11)| ((uint32_t)function << 8)| (offset & 0xfc)|((uint32_t)0x80000000);
	outl(0xcf8,address);
	
	uint16_t result = (uint16_t)(inl(0xcfc) >> (8* (offset & 2)) & 0xffff);
	//kprintf("val %x",result);
	return result;
	
} 
int check(hba_port_t *port){
	//kprintf("Hello %x ",port->sig);
	if(port->sig == AHCI_DEV_SATAPI){return 1;}
        else if(port->sig == AHCI_DEV_SEMB){return 2;}
        else if(port->sig == AHCI_DEV_PM){return 3;}
        else if(port->sig == AHCI_DEV_SATA){return 4;}
	return 0;
}

void probeport(hba_mem_t *abar){
//kprintf("in fnc");
uint32_t pi=abar->pi;
int i=0;
	while(i<32){
		if(pi&1){
			int dt=check(&abar->ports[i]);
			if (dt == 4)
			{
				kprintf("SATA drive found at port %d\n", i);
				if(i==1)
				{
					//kprintf("in the sata 1");
					abar->ghc=(uint32_t)1<<1;
					port_rebase(&abar->ports[i],i);
					uint32_t *buf=(uint32_t*)0x300000;
					uint8_t *nbuf=(uint8_t*)buf;
					//*buf=1;
					/*for(int by=0;by<512;by++) {
                                                 *nbuf=1;
                                                 nbuf++;
                                                }
					*/for(int bl=0;bl<100;bl++)
					{
						nbuf=(uint8_t*)0x300000;
					 	for(int by=0;by<512;by++) {
                                               	 *nbuf=bl+1;
                                                 nbuf++;
                                                }

						for(int sect=0;sect<8;sect++)
						{ 							
							//nbuf = (uint8_t*)0x300000;
							write_to_disk(&abar->ports[i],bl*8+sect, 0, 1,buf);
							//kprintf("%d ",bl*8+sect);
						}
					}
					//write_to_disk(&abar->ports[i],1, 0, 1,buf);
					kprintf("write done at SATA Drive %d\n",i);
					uint32_t *buf2=(uint32_t*)0x400000;
					read_from_disk(&abar->ports[i], 20, 0, 1,buf2);
					kprintf("%d\n\n",*buf2);
					kprintf("%d\n\n",*(buf2+4));
					kprintf("%d\n\n",*(buf2+8));
					kprintf("%d\n\n",*(buf2+12));


				}
			}
			else if (dt == 1)
			{
				kprintf("SATAPI drive found at port %d\n", i);
			}
			else if (dt == 2)
			{
				kprintf("SEMB drive found at port %d\n", i);
			}
			else if (dt == 3)
			{
				kprintf("PM drive found at port %d\n", i);
			}
			else
			{
				kprintf("No drive found at port %d\n", i);
			}
		}
	pi=pi>>1;
	i++;
}

}

void walk(){

	for (int bus=0;bus<254;bus++){
		for (int device =0; device<32;device++){
				for(int fn=0;fn<8;fn++)
				{
				uint16_t vendor_id = read(bus, device, fn, 0x0a);
    				if (vendor_id>>8 == 0x01){
					if ((vendor_id & (0x00ff)) == 0x06){
						kprintf("bus");
                                		kprintf("%x \n",bus&0xff);
                                		kprintf("device");
                                		kprintf("%x \n",device&0xff);
								kprintf("function %d\n",fn);
						uint16_t abar1 = read(bus, device, fn, 0x24);
						uint16_t abar2 = read(bus, device, fn, 0x26);
						//probeport(&abar);
						kprintf("updated value %x\n",(abar2<<16)|abar1);
						uint32_t abar = ((abar2<<16)|abar1);//((uint32_t)abar2<<16)|abar1;
						//kprintf("abar %xbb",abar);
                                                probeport((hba_mem_t*)(uint64_t)abar);
						//outl(0xcfc,);
						//uint16_t result = (uint16_t)(inl(0xcfc) >> (8* ((0x24) & 2)) & 0xffff);
					}
				}
				}

		}
	}

}
