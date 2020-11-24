################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../practica_1A_rtems_2021-v1/._controladorA.c \
../practica_1A_rtems_2021-v1/._displayA.c \
../practica_1A_rtems_2021-v1/controladorA.c \
../practica_1A_rtems_2021-v1/displayA.c 

OBJS += \
./practica_1A_rtems_2021-v1/._controladorA.o \
./practica_1A_rtems_2021-v1/._displayA.o \
./practica_1A_rtems_2021-v1/controladorA.o \
./practica_1A_rtems_2021-v1/displayA.o 

C_DEPS += \
./practica_1A_rtems_2021-v1/._controladorA.d \
./practica_1A_rtems_2021-v1/._displayA.d \
./practica_1A_rtems_2021-v1/controladorA.d \
./practica_1A_rtems_2021-v1/displayA.d 


# Each subdirectory must supply rules for building sources it contributes
practica_1A_rtems_2021-v1/%.o: ../practica_1A_rtems_2021-v1/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	i386-rtems7-gcc -O0 -g3 -Wall -c -fmessage-length=0 -B/home/yo/others/material_uni/development/rtems/bsps/i386/pc386/i386-rtems7/pc386/lib -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


