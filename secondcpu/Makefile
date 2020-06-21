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
	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr $(BUILD) *.elf *.bin *.o2x
	@make -C $(CURDIR)/$(ARM920_DIR) clean ARM940_MEMORY_BANK=$(ARM940_MEMORY_BANK)
	@make -C $(CURDIR)/$(ARM940_DIR) clean ARM940_MEMORY_BANK=$(ARM940_MEMORY_BANK)

#---------------------------------------------------------------------------------
else

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------

$(OUTPUT).o2x   : arm920 arm940
	@$(eval PARAMS_ADDR := $(shell readelf -s $(CURDIR)/../$(ARM920_DIR)/arm920.elf | grep "__params_addr" | grep -Po ': \K[0-9A-Fa-f]+ ' | xargs printf "0x%s"))
	@$(eval PARAMS_LENGTH := $(shell readelf -s $(CURDIR)/../$(ARM920_DIR)/arm920.elf | grep "__params_length" | grep -Po ': \K[0-9A-Fa-f]+ ' | xargs printf "0x%s"))

	mko2x --name "$(TITLE)" --out $@ --section $(CURDIR)/../$(ARM920_DIR)/arm920.bin 0x0 --section $(CURDIR)/../$(ARM940_DIR)/arm940.940.bin $$(( 16777216 * $(ARM940_MEMORY_BANK) )) --params ${PARAMS_ADDR} ${PARAMS_LENGTH} $(MKO2X_PARAMS)

arm920:
	make -C $(CURDIR)/../$(ARM920_DIR) ARM940_MEMORY_BANK=$(ARM940_MEMORY_BANK)
arm940:
	make -C $(CURDIR)/../$(ARM940_DIR) ARM940_MEMORY_BANK=$(ARM940_MEMORY_BANK)

#---------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------
