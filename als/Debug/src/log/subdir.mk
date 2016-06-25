################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/log/logwriter.cpp 

OBJS += \
./src/log/logwriter.o 

CPP_DEPS += \
./src/log/logwriter.d 


# Each subdirectory must supply rules for building sources it contributes
src/log/%.o: ../src/log/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/arun/workspace/als/als/include" -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


