################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
L138_LCDK_aic3106_init.obj: ../L138_LCDK_aic3106_init.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv8/tools/compiler/c6000_7.4.24/bin/cl6x" -mv6740 --abi=coffabi -g --include_path="C:/Users/gabriel/Desktop/Lab DSP/Lab3p2" --include_path="C:/Users/gabriel/Desktop/Lab DSP/CCSv4-2-4_with_SOM-M1_BSL_WS/DSP BSL/inc" --include_path="C:/ti/ccsv8/tools/compiler/c6000_7.4.24/include" --define=c6748 --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="L138_LCDK_aic3106_init.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

L3p3_2.obj: ../L3p3_2.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv8/tools/compiler/c6000_7.4.24/bin/cl6x" -mv6740 --abi=coffabi -g --include_path="C:/Users/gabriel/Desktop/Lab DSP/Lab3p2" --include_path="C:/Users/gabriel/Desktop/Lab DSP/CCSv4-2-4_with_SOM-M1_BSL_WS/DSP BSL/inc" --include_path="C:/ti/ccsv8/tools/compiler/c6000_7.4.24/include" --define=c6748 --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="L3p3_2.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

dsp_lab_utils.obj: ../dsp_lab_utils.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv8/tools/compiler/c6000_7.4.24/bin/cl6x" -mv6740 --abi=coffabi -g --include_path="C:/Users/gabriel/Desktop/Lab DSP/Lab3p2" --include_path="C:/Users/gabriel/Desktop/Lab DSP/CCSv4-2-4_with_SOM-M1_BSL_WS/DSP BSL/inc" --include_path="C:/ti/ccsv8/tools/compiler/c6000_7.4.24/include" --define=c6748 --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="dsp_lab_utils.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

vectors_poll.obj: ../vectors_poll.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv8/tools/compiler/c6000_7.4.24/bin/cl6x" -mv6740 --abi=coffabi -g --include_path="C:/Users/gabriel/Desktop/Lab DSP/Lab3p2" --include_path="C:/Users/gabriel/Desktop/Lab DSP/CCSv4-2-4_with_SOM-M1_BSL_WS/DSP BSL/inc" --include_path="C:/ti/ccsv8/tools/compiler/c6000_7.4.24/include" --define=c6748 --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="vectors_poll.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


