################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
src/main_mii_loopback.obj: ../src/main_mii_loopback.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/c6000/bin/cl6x" -mv6740 -g --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/c6000/include" --include_path="../../../../bsl/inc/" --include_path="../inc/" --diag_warning=225 --preproc_with_compile --preproc_dependency="src/main_mii_loopback.pp" --obj_directory="src" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

src/test_mii_loopback.obj: ../src/test_mii_loopback.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/c6000/bin/cl6x" -mv6740 -g --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/c6000/include" --include_path="../../../../bsl/inc/" --include_path="../inc/" --diag_warning=225 --preproc_with_compile --preproc_dependency="src/test_mii_loopback.pp" --obj_directory="src" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

