#include <sys/defs.h>
#include <sys/ahci.h>
#ifndef ENUMPCI_H
#define ENUMPCI_H

uint16_t read(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset);
void walk();
int check(hba_port_t *port);
void probeport(hba_mem_t *abar);
void port_rebase(hba_port_t *port, int portno);
#endif

