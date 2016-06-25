################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/structure/integer.cpp \
../src/structure/streamqueue.cpp 

OBJS += \
./src/structure/integer.o \
./src/structure/streamqueue.o 

CPP_DEPS += \
./src/structure/integer.d \
./src/structure/streamqueue.d 


# Each subdirectory must supply rules for building sources it contributes
src/structure/%.o: ../src/structure/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/arun/workspace/als/als/include" -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


