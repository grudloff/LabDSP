################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
src/main_uart.obj: ../src/main_uart.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/bin/cl470" -mv5e -g -O2 --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/include" --include_path="C:/ccs4/1016316A_AM1808_BSL_WS/AM1808eXp UART Test/inc" --include_path="C:/ccs4/1016316A_AM1808_BSL_WS/AM1808 BSL/inc" --diag_warning=225 -me --abi=eabi --code_state=32 --preproc_with_compile --preproc_dependency="src/main_uart.pp" --obj_directory="src" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

src/test_uart.obj: ../src/test_uart.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/bin/cl470" -mv5e -g -O2 --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/include" --include_path="C:/ccs4/1016316A_AM1808_BSL_WS/AM1808eXp UART Test/inc" --include_path="C:/ccs4/1016316A_AM1808_BSL_WS/AM1808 BSL/inc" --diag_warning=225 -me --abi=eabi --code_state=32 --preproc_with_compile --preproc_dependency="src/test_uart.pp" --obj_directory="src" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '


