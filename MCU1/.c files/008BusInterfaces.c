#include <stdint.h>


// this is a const data or ready only data stored in the flash memory of the MCU
char const my_data[]="I love embedded programming"; // preserved in flash 
// instructions of the program are fetched over the I-bus 
#define BASE_ADDRESS_OF_SRAM 0x20000000

int main(void)
{

  for(int i=0; i< sizeof(my_data);i++)
 {

     *((uint8_t*) BASE_ADDRESS_OF_SRAM +i)=my_data[i];  // reading of data takes place over D-bus
 }
  return 0;
}

/* 
instruction fetches are performed over the AHB-Lite bus 
0x0000_0000 to 0x1FFF_FFFC

data & debug accesses to code memory space: 0x00000000 to 0x1FFFFFFF
performed over AHB-Lite bus 

system interface:
instruction fetches and data accesses to address ranges 0x20000000 to 0xDFFFFFFF and 0xE0100000 to 0xFFFFFFFF
performed over the AHB-Lite bus 

order in decreasing priority:
-data acesses 
-instruction & vector fetches
-debug 
*/ 