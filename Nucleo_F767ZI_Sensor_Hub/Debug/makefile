################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include Drivers/STM32F7xx_HAL_Driver/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Nucleo_F767ZI_Sensor_Hub.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Nucleo_F767ZI_Sensor_Hub.list \


# All Target
all: Nucleo_F767ZI_Sensor_Hub.elf secondary-outputs

# Tool invocations
Nucleo_F767ZI_Sensor_Hub.elf: $(OBJS) $(USER_OBJS) C:\Users\Lars\STM32CubeIDE\workspace_1.0.0\Nucleo_F767ZI_Sensor_Hub\STM32F767ZITX_FLASH.ld
	arm-none-eabi-gcc -o "Nucleo_F767ZI_Sensor_Hub.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m7 -T"C:\Users\Lars\STM32CubeIDE\workspace_1.0.0\Nucleo_F767ZI_Sensor_Hub\STM32F767ZITX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Nucleo_F767ZI_Sensor_Hub.map" -Wl,--gc-sections -static -lc -lrdimon -u _printf_float --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Nucleo_F767ZI_Sensor_Hub.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Nucleo_F767ZI_Sensor_Hub.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
