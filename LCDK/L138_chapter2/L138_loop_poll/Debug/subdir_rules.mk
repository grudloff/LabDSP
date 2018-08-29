################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
L138_LCDK_aic3106_init.obj: C:/LCDK/L138_support/L138_LCDK_aic3106_init.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6740 -g --include_path="C:/ti/ccsv5/tools/compiler/c6000/include" --include_path="C:/omapl138/bsl/inc" --include_path="C:/LCDK/L138_support" --diag_warning=225 --preproc_with_compile --preproc_dependency="L138_LCDK_aic3106_init.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

L138_loop_poll.obj: ../L138_loop_poll.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6740 -g --include_path="C:/ti/ccsv5/tools/compiler/c6000/include" --include_path="C:/omapl138/bsl/inc" --include_path="C:/LCDK/L138_support" --diag_warning=225 --preproc_with_compile --preproc_dependency="L138_loop_poll.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

vectors_poll.obj: C:/LCDK/L138_support/vectors_poll.asm $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c6000/bin/cl6x" -mv6740 -g --include_path="C:/ti/ccsv5/tools/compiler/c6000/include" --include_path="C:/omapl138/bsl/inc" --include_path="C:/LCDK/L138_support" --diag_warning=225 --preproc_with_compile --preproc_dependency="vectors_poll.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


