################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

EMPTY := 
SPACE := $(EMPTY) $(EMPTY)
CG_TOOL_ROOT := C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470

ORDERED_OBJS += \
$(GEN_CMDS_QUOTED) \
"./src/test_lcd_char.obj" \
"./src/main_lcd_char.obj" \
-l"C:\ccs4\1016316A_AM1808_BSL_WS\AM1808 BSL\lib\am1808_bsl.lib" \
-l"C:\ccs4\1016316A_AM1808_BSL_WS\AM1808 BSL\am1808_rts.lib" \
"../linker.cmd" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include src/subdir_vars.mk
-include subdir_rules.mk
-include src/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: AM1808EVMCharacterLCDTest.out

dependents:
	-cd C:/ccs4/1016316A_AM1808_BSL_WS/AM1808\ BSL/Debug && $(MAKE) clean all

# Tool invocations
AM1808EVMCharacterLCDTest.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS) C:/ccs4/1016316A_AM1808_BSL_WS/AM1808\ BSL/Debug/../lib/am1808_bsl.lib
	@echo 'Building target: $@'
	@echo 'Invoking: Linker'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/bin/cl470" -mv5e -g --diag_warning=225 -me --abi=eabi --code_state=32 -z -m"AM1808EVMCharacterLCDTest.map" --stack_size=0x800 --heap_size=0x800 --warn_sections -i"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/lib" -i"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/include" --reread_libs --rom_model -o "AM1808EVMCharacterLCDTest.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(TMS470_EXECUTABLE_OUTPUTS__QTD) "AM1808EVMCharacterLCDTest.out"
	-$(RM) ".\src\main_lcd_char.obj" ".\src\test_lcd_char.obj" 
	-$(RM) ".\src\main_lcd_char.pp" ".\src\test_lcd_char.pp" 
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
C:/ccs4/1016316A_AM1808_BSL_WS/AM1808\ BSL/Debug/../lib/am1808_bsl.lib:

-include ../makefile.targets
