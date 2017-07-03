################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf
MAJOR := `cat ../inc/cleri/version.h | grep 'define LIBCLERI_VERSION_MAJOR' | tail -c 2 | head -c 1`
MINOR := `cat ../inc/cleri/version.h | grep 'define LIBCLERI_VERSION_MINOR' | tail -c 2 | head -c 1`
PATCH := `cat ../inc/cleri/version.h | grep 'define LIBCLERI_VERSION_PATCH' | tail -c 2 | head -c 1`

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OS := $(shell uname)
ifeq ($(OS),Darwin)
FN := libcleri.dylib
INSTALL_PATH := /usr/local
else
FN := libcleri.so
INSTALL_PATH := /usr
endif

# Add inputs and outputs from these tool invocations to the build variables

# All Target
all: libcleri

# Tool invocations
libcleri: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross GCC Linker'
	gcc -shared -Wl,-soname,$(FN).$(MAJOR) -o $(FN).$(MAJOR).$(MINOR).$(PATCH) $(OBJS) $(USER_OBJS) $(LIBS) $(LDFLAGS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(LIBRARIES)$(OBJS)$(C_DEPS) $(FN).$(MAJOR).$(MINOR).$(PATCH)
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
