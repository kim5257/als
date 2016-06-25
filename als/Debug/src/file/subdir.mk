################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/file/dir.cpp \
../src/file/dir_entry.cpp 

OBJS += \
./src/file/dir.o \
./src/file/dir_entry.o 

CPP_DEPS += \
./src/file/dir.d \
./src/file/dir_entry.d 


# Each subdirectory must supply rules for building sources it contributes
src/file/%.o: ../src/file/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/arun/workspace/als/als/include" -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


