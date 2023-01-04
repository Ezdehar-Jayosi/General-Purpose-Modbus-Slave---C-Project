################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Logic.cpp \
../Message.cpp \
../SlaveManager.cpp \
../TCP.cpp \
../TransportManager.cpp \
../main.cpp \
../regTables.cpp 

CPP_DEPS += \
./Logic.d \
./Message.d \
./SlaveManager.d \
./TCP.d \
./TransportManager.d \
./main.d \
./regTables.d 

OBJS += \
./Logic.o \
./Message.o \
./SlaveManager.o \
./TCP.o \
./TransportManager.o \
./main.o \
./regTables.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean--2e-

clean--2e-:
	-$(RM) ./Logic.d ./Logic.o ./Message.d ./Message.o ./SlaveManager.d ./SlaveManager.o ./TCP.d ./TCP.o ./TransportManager.d ./TransportManager.o ./main.d ./main.o ./regTables.d ./regTables.o

.PHONY: clean--2e-

