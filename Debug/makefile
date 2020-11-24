################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include practica_1_arduino_2021-v1/extras/rtems_msg_router/subdir.mk
-include practica_1C_rtems_2021-v1/subdir.mk
-include practica_1B_rtems_2021-v1/subdir.mk
-include practica_1A_rtems_2021-v1/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: practica_1_arduino

# Tool invocations
practica_1_arduino: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross GCC Linker'
	i386-rtems7-gcc -B/home/yo/others/material_uni/development/rtems/bsps/i386/pc386/i386-rtems7/pc386/lib -qrtems -Wl,-Ttext,0x00100000 -o "practica_1_arduino" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) practica_1_arduino
	-@echo ' '

.PHONY: all clean dependents

-include ../makefile.targets
