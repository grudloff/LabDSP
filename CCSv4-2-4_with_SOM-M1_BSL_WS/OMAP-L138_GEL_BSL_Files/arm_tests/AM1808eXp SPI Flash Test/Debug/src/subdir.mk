################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main_spiflash.c \
../src/test_spiflash.c 

OBJS += \
./src/main_spiflash.obj \
./src/test_spiflash.obj 

C_DEPS += \
./src/main_spiflash.pp \
./src/test_spiflash.pp 

OBJS__QTD += \
".\src\main_spiflash.obj" \
".\src\test_spiflash.obj" 

C_DEPS__QTD += \
".\src\main_spiflash.pp" \
".\src\test_spiflash.pp" 

C_SRCS_QUOTED += \
"../src/main_spiflash.c" \
"../src/test_spiflash.c" 


# Each subdirectory must supply rules for building sources it contributes
src/main_spiflash.obj: ../src/main_spiflash.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/bin/cl470" -mv5e -g --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/include" --include_path="C:/CCS4/1016316A_AM1808_BSL_WS/AM1808eXp SPI Flash Test/inc" --include_path="C:/CCS4/1016316A_AM1808_BSL_WS/AM1808 BSL/inc" --diag_warning=225 -me --abi=eabi --code_state=32 --preproc_with_compile --preproc_dependency="src/main_spiflash.pp" --obj_directory="src" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

src/test_spiflash.obj: ../src/test_spiflash.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/bin/cl470" -mv5e -g --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/include" --include_path="C:/CCS4/1016316A_AM1808_BSL_WS/AM1808eXp SPI Flash Test/inc" --include_path="C:/CCS4/1016316A_AM1808_BSL_WS/AM1808 BSL/inc" --diag_warning=225 -me --abi=eabi --code_state=32 --preproc_with_compile --preproc_dependency="src/test_spiflash.pp" --obj_directory="src" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

