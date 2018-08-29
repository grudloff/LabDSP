################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
linker.out: ../linker.cmd $(GEN_CMDS)
	@echo 'Building file: $<'
	@echo 'Invoking: Linker'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/bin/cl470" -mv5e -g --diag_warning=225 -me --abi=eabi --code_state=32 -z -m"AM1808eXpAudioTest.map" --stack_size=0x800 --heap_size=0x800 --warn_sections -i"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/lib" -i"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/include" --reread_libs --rom_model -o "$@" "$<" "../linker.cmd"
	@echo 'Finished building: $<'
	@echo ' '


