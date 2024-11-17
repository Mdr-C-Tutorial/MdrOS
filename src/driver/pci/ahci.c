#include "ahci.h"
#include "pci.h"
#include "klog.h"

void ahci_init(){
    pci_device_t *device = pci_find_class(0);
    if(device == NULL){
        klogf(false,"Cannot find ahci.\n");
        return;
    } else klogf(true,"Find ahci.\n");


}