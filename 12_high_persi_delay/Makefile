CROSS_COMPILE 	?= arm-linux-gnueabihf-
TARGET		  	?= GPT_delay

CC 				:= $(CROSS_COMPILE)gcc
LD				:= $(CROSS_COMPILE)ld
OBJCOPY 		:= $(CROSS_COMPILE)objcopy
OBJDUMP 		:= $(CROSS_COMPILE)objdump

# 将 imx6ul 改为 imx6u
INCDIRS 		:= imx6u \
				   bsp/clk \
				   bsp/led \
				   bsp/delay\
				   bsp/beep\
				   bsp/key\
				   bsp/gpio\
				   bsp/int\
				   bsp/exit\
				   bsp/epit\
				   bsp/key_filter
				   			   
SRCDIRS			:= project \
				   bsp/clk \
				   bsp/led \
				   bsp/delay\
				   bsp/beep\
				   bsp/key\
				   bsp/gpio\
				   bsp/int\
				   bsp/exit\
				   bsp/epit\
				   bsp/key_filter
				   
				   
INCLUDE			:= $(patsubst %, -I %, $(INCDIRS))

SFILES			:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.S))
CFILES			:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))

SFILENDIR		:= $(notdir  $(SFILES))
CFILENDIR		:= $(notdir  $(CFILES))

SOBJS			:= $(patsubst %, obj/%, $(SFILENDIR:.S=.o))
COBJS			:= $(patsubst %, obj/%, $(CFILENDIR:.c=.o))
OBJS			:= $(SOBJS) $(COBJS)

VPATH			:= $(SRCDIRS)

.PHONY: clean
	
$(TARGET).bin : $(OBJS)
	$(LD) -Timx6u.lds -o $(TARGET).elf $^
	$(OBJCOPY) -O binary -S $(TARGET).elf $@
	$(OBJDUMP) -D -m arm $(TARGET).elf > $(TARGET).dis

$(SOBJS) : obj/%.o : %.S
	$(CC) -Wall -nostdlib -c -O2  $(INCLUDE) -o $@ $<

$(COBJS) : obj/%.o : %.c
	$(CC) -Wall -nostdlib -c -O2  $(INCLUDE) -o $@ $<
	
clean:
	rm -rf $(TARGET).elf $(TARGET).dis $(TARGET).bin $(COBJS) $(SOBJS)