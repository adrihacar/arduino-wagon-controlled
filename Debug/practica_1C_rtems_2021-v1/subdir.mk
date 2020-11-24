################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../practica_1C_rtems_2021-v1/._controladorC.c \
../practica_1C_rtems_2021-v1/._displayC.c \
../practica_1C_rtems_2021-v1/controladorC.c \
../practica_1C_rtems_2021-v1/displayC.c 

OBJS += \
./practica_1C_rtems_2021-v1/._controladorC.o \
./practica_1C_rtems_2021-v1/._displayC.o \
./practica_1C_rtems_2021-v1/controladorC.o \
./practica_1C_rtems_2021-v1/displayC.o 

C_DEPS += \
./practica_1C_rtems_2021-v1/._controladorC.d \
./practica_1C_rtems_2021-v1/._displayC.d \
./practica_1C_rtems_2021-v1/controladorC.d \
./practica_1C_rtems_2021-v1/displayC.d 


# Each subdirectory must supply rules for building sources it contributes
practica_1C_rtems_2021-v1/%.o: ../practica_1C_rtems_2021-v1/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	i386-rtems7-gcc -O0 -g3 -Wall -c -fmessage-length=0 -B/home/yo/others/material_uni/development/rtems/bsps/i386/pc386/i386-rtems7/pc386/lib -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


