################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/sha/sha1.cpp \
../src/sha/sha512.cpp 

OBJS += \
./src/sha/sha1.o \
./src/sha/sha512.o 

CPP_DEPS += \
./src/sha/sha1.d \
./src/sha/sha512.d 


# Each subdirectory must supply rules for building sources it contributes
src/sha/%.o: ../src/sha/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/arun/workspace/als/als/include" -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


