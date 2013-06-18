PREFIX=/home/danne/gcc-arm-none-eabi-4_7-2013q1
ARM=$(PREFIX)/bin/arm-none-eabi
CFLAGS=-Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding -nodefaultlibs -fno-builtin
LINKSCRIPT=linker.ld
LDFLAGS=-T$(LINKSCRIPT)
CSOURCES=kernel_main.c uart.c irq_handler.c timer.c irq_controller.c mmu.c
SSOURCES=boot.S
SOURCES=$(CSOURCES) $(SSOURCES)
COBJECTS=$(CSOURCES:.c=.o) 
SOBJECTS=$(SSOURCES:.S=.o)
OBJECTS=$(COBJECTS) $(SOBJECTS)
EXECUTABLE=boot.bin
CC=$(ARM)-gcc

all: $(SOURCES) $(EXECUTABLE)
	cp $(EXECUTABLE) /tftpboot/$(EXECUTABLE)

clean:
	rm $(EXECUTABLE) $(OBJECTS) $(EXECUTABLE).elf


$(EXECUTABLE): $(EXECUTABLE).elf
	$(ARM)-objdump -d $(EXECUTABLE).elf > boot.list
	$(ARM)-objcopy $(EXECUTABLE).elf -O binary $(EXECUTABLE)

$(EXECUTABLE).elf: $(OBJECTS) $(LINKSCRIPT)
	$(ARM)-ld $(LDFLAGS) $(OBJECTS) -o $@

.S.o:
	$(CC) -c $< -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@	
