################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../practica_1D_rtems_2021-v1/._controladorD.c \
../practica_1D_rtems_2021-v1/._displayD.c \
../practica_1D_rtems_2021-v1/controladorD.c \
../practica_1D_rtems_2021-v1/displayD.c 

OBJS += \
./practica_1D_rtems_2021-v1/._controladorD.o \
./practica_1D_rtems_2021-v1/._displayD.o \
./practica_1D_rtems_2021-v1/controladorD.o \
./practica_1D_rtems_2021-v1/displayD.o 

C_DEPS += \
./practica_1D_rtems_2021-v1/._controladorD.d \
./practica_1D_rtems_2021-v1/._displayD.d \
./practica_1D_rtems_2021-v1/controladorD.d \
./practica_1D_rtems_2021-v1/displayD.d 


# Each subdirectory must supply rules for building sources it contributes
practica_1D_rtems_2021-v1/%.o: ../practica_1D_rtems_2021-v1/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	i386-rtems7-gcc -O0 -g3 -Wall -c -fmessage-length=0 -B/home/yo/others/material_uni/development/rtems/bsps/i386/pc386/i386-rtems7/pc386/lib -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


