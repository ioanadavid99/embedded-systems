CC=arm-none-eabi-gcc
MACH=cortex-m4
CFLAGS= -c -mcpu=$(MACH) -mthumb -std=gnu11 -Wall -O0
LDFLAGS= -nostdlib -T stm32_ls.ld -Wl,-Map=final.map

all:main.o led.o stm32_startup.o syscalls.o final.elf

main.o:main.c
	$(CC) $(CFLAGS) main.c -o main.o

led.o:led.c
	$(CC) $(CFLAGS) led.c -o led.o

stm32_startup.o:stm32_startup.c
	$(CC) $(CFLAGS) stm32_startup.c -o stm32_startup.o 
	
syscalls.o:syscalls.c
	$(CC) $(CFLAGS) -o $@ $^
	
final.elf:main.o led.o stm32_startup.o syscalls.o
	$(CC) $(LDFLAGS) -o $@ $^
	
clean:
	rm -rf *.o *.elf
	
load:
	openocd -f board/stm32f4discovery.cfg