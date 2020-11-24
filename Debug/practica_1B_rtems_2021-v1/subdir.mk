################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../practica_1B_rtems_2021-v1/controladorB.c \
../practica_1B_rtems_2021-v1/displayB.c 

OBJS += \
./practica_1B_rtems_2021-v1/controladorB.o \
./practica_1B_rtems_2021-v1/displayB.o 

C_DEPS += \
./practica_1B_rtems_2021-v1/controladorB.d \
./practica_1B_rtems_2021-v1/displayB.d 


# Each subdirectory must supply rules for building sources it contributes
practica_1B_rtems_2021-v1/%.o: ../practica_1B_rtems_2021-v1/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	i386-rtems7-gcc -O0 -g3 -Wall -c -fmessage-length=0 -B/home/yo/others/material_uni/development/rtems/bsps/i386/pc386/i386-rtems7/pc386/lib -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


