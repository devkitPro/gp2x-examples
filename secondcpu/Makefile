#---------------------------------------------------------------------------------
# Clear the implicit built in rules
#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/gp2x_rules

#---------------------------------------------------------------------------------
TITLE := Second CPU Example
MKO2X_PARAMS := 
ARM940_MEMORY_BANK := 3
#---------------------------------------------------------------------------------

#---------------------------------------------------------------------------------
# TARGET is the name of the output
#---------------------------------------------------------------------------------
TARGET		:=	$(shell basename $(CURDIR))
BUILD		:=	build
ARM920_DIR      :=      arm920
ARM940_DIR      :=      arm940

#---------------------------------------------------------------------------------
# path to tools - this can be deleted if you set the path in windows
#---------------------------------------------------------------------------------
export PATH		:=	$(DEVKITARM)/bin:$(PATH)


#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export OUTPUT	:=	$(CURDIR)/$(TARGET)

.PHONY: $(BUILD) clean

#---------------------------------------------------------------------------------
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr $(BUILD) *.elf *.bin *.o2x
	@$(MAKE) -C $(CURDIR)/$(ARM920_DIR) clean ARM940_MEMORY_BANK=$(ARM940_MEMORY_BANK)
	@$(MAKE) -C $(CURDIR)/$(ARM940_DIR) clean ARM940_MEMORY_BANK=$(ARM940_MEMORY_BANK)

#---------------------------------------------------------------------------------
else

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------

$(OUTPUT).o2x   : arm920 arm940
	mko2x --name "$(TITLE)" --out $@ --section $(CURDIR)/../$(ARM920_DIR)/arm920.bin 0x0 --section $(CURDIR)/../$(ARM940_DIR)/arm940.940.bin $$(( 16777216 * $(ARM940_MEMORY_BANK) )) $(MKO2X_PARAMS)

arm920:
	make -C $(CURDIR)/../$(ARM920_DIR) ARM940_MEMORY_BANK=$(ARM940_MEMORY_BANK)
arm940:
	make -C $(CURDIR)/../$(ARM940_DIR) ARM940_MEMORY_BANK=$(ARM940_MEMORY_BANK)

#---------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------
