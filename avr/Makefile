###########################################
# Darkstar Mk.II
#
# Jakub Kaderka 2017
###########################################

MODE=simple

#used MCU
MCU=attiny841
#used frequency
F_CPU=4000000UL

#program name
PROGRAM=${MODE}

#dirs
OBJDIR=obj
SRCDIRS=src/library
MODESDIR=src/modes

#compiler and linker config
CC=avr-gcc
LD=avr-gcc
CFLAGS=-Wall -Wextra -std=c99 -Os -I $(SRCDIRS) -DF_CPU=$(F_CPU) -mmcu=$(MCU) -std=gnu99 -Wstrict-prototypes -ffunction-sections -fdata-sections
LDFLAGS=-mmcu=$(MCU) -Wl,--gc-sections
OBJCOPY=avr-objcopy
UPLOADER=avrdude
UPLOADER_FLAGS=-c arduino -b 9600 -P /dev/ttyUSB0 -p $(MCU)
FUSES_FLAGS=-c usbtiny -p $(MCU)

#resulting bin
TARGET=$(PROGRAM).hex

C_SRC = $(shell find $(SRCDIRS) $(MODESDIR)/$(MODE).c -name '*.c')
OBJS = $(addprefix $(OBJDIR)/, $(C_SRC:.c=.o) $(AS_SRC:.s=.o))
DEPS = $(addprefix $(OBJDIR)/, $(C_SRC:.c=.d))
DIRS=$(addprefix $(OBJDIR)/, $(shell find $(SRCDIRS) $(MODESDIR) -type d))

.PHONY : clean all hierarchy program

all: hierarchy $(TARGET)

#create file hierarchy
hierarchy: $(OBJDIR) $(DIRS)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(DIRS):
	mkdir -pv $(DIRS)

#compile sources
$(OBJDIR)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@
	@$(CC) $(CFLAGS) -MM -MT '$@ $(basename $@).d' $< > $(basename $@).d

#link them together
$(OBJDIR)/$(PROGRAM).elf: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(OBJDIR)/$(PROGRAM).elf

$(TARGET): $(OBJDIR)/$(PROGRAM).elf
	$(OBJCOPY) -O ihex -R .eeprom $(OBJDIR)/$(PROGRAM).elf $(TARGET)
	#$(OBJCOPY) -O ihex -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 --no-change-warnings $(OBJDIR)/$(PROGRAM).elf $(PROGRAM).epp
	avr-size $(TARGET)

-include $(DEPS)

clean:
	-rm -r $(OBJDIR) *.hex

program:
	$(UPLOADER) $(UPLOADER_FLAGS) -U flash:w:$(TARGET)

#SELFPRGEN enable
fuses:
	$(UPLOADER) $(FUSES_FLAGS) -U efuse:w:0xFE:m
