################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../practica_1_arduino_2021-v1/extras/rtems_msg_router/._rtems_msg_router.c \
../practica_1_arduino_2021-v1/extras/rtems_msg_router/rtems_msg_router.c 

OBJS += \
./practica_1_arduino_2021-v1/extras/rtems_msg_router/._rtems_msg_router.o \
./practica_1_arduino_2021-v1/extras/rtems_msg_router/rtems_msg_router.o 

C_DEPS += \
./practica_1_arduino_2021-v1/extras/rtems_msg_router/._rtems_msg_router.d \
./practica_1_arduino_2021-v1/extras/rtems_msg_router/rtems_msg_router.d 


# Each subdirectory must supply rules for building sources it contributes
practica_1_arduino_2021-v1/extras/rtems_msg_router/%.o: ../practica_1_arduino_2021-v1/extras/rtems_msg_router/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	i386-rtems7-gcc -O0 -g3 -Wall -c -fmessage-length=0 -B/home/yo/others/material_uni/development/rtems/bsps/i386/pc386/i386-rtems7/pc386/lib -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


