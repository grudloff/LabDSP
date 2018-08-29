################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
lcdk/L138_LCDK_aic3106_init.obj: ../lcdk/L138_LCDK_aic3106_init.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv8/tools/compiler/c6000_7.4.24/bin/cl6x" -mv6740 --abi=coffabi -g --include_path="C:/Users/gabriel/Desktop/Lab DSP/Lab2p2" --include_path="C:/Users/gabriel/Desktop/Lab DSP/CCSv4-2-4_with_SOM-M1_BSL_WS/DSP BSL/inc" --include_path="C:/ti/ccsv8/tools/compiler/c6000_7.4.24/include" --define=c6748 --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="lcdk/L138_LCDK_aic3106_init.d_raw" --obj_directory="lcdk" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

lcdk/L2p2_2_4_v2.obj: ../lcdk/L2p2_2_4_v2.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv8/tools/compiler/c6000_7.4.24/bin/cl6x" -mv6740 --abi=coffabi -g --include_path="C:/Users/gabriel/Desktop/Lab DSP/Lab2p2" --include_path="C:/Users/gabriel/Desktop/Lab DSP/CCSv4-2-4_with_SOM-M1_BSL_WS/DSP BSL/inc" --include_path="C:/ti/ccsv8/tools/compiler/c6000_7.4.24/include" --define=c6748 --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="lcdk/L2p2_2_4_v2.d_raw" --obj_directory="lcdk" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

lcdk/lab_dsp_utils.obj: ../lcdk/lab_dsp_utils.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv8/tools/compiler/c6000_7.4.24/bin/cl6x" -mv6740 --abi=coffabi -g --include_path="C:/Users/gabriel/Desktop/Lab DSP/Lab2p2" --include_path="C:/Users/gabriel/Desktop/Lab DSP/CCSv4-2-4_with_SOM-M1_BSL_WS/DSP BSL/inc" --include_path="C:/ti/ccsv8/tools/compiler/c6000_7.4.24/include" --define=c6748 --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="lcdk/lab_dsp_utils.d_raw" --obj_directory="lcdk" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

lcdk/vectors_poll.obj: ../lcdk/vectors_poll.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv8/tools/compiler/c6000_7.4.24/bin/cl6x" -mv6740 --abi=coffabi -g --include_path="C:/Users/gabriel/Desktop/Lab DSP/Lab2p2" --include_path="C:/Users/gabriel/Desktop/Lab DSP/CCSv4-2-4_with_SOM-M1_BSL_WS/DSP BSL/inc" --include_path="C:/ti/ccsv8/tools/compiler/c6000_7.4.24/include" --define=c6748 --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="lcdk/vectors_poll.d_raw" --obj_directory="lcdk" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


