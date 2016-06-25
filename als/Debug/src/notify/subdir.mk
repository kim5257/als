################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/notify/file_notify.cpp \
../src/notify/notify_fd.cpp \
../src/notify/watch_fd.cpp 

OBJS += \
./src/notify/file_notify.o \
./src/notify/notify_fd.o \
./src/notify/watch_fd.o 

CPP_DEPS += \
./src/notify/file_notify.d \
./src/notify/notify_fd.d \
./src/notify/watch_fd.d 


# Each subdirectory must supply rules for building sources it contributes
src/notify/%.o: ../src/notify/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/arun/workspace/als/als/include" -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


