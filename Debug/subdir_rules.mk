################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"/Applications/ti/ccs910/ccs/tools/compiler/msp430-gcc-8.2.0.52_macos/bin/msp430-elf-gcc" -c -mmcu=msp430g2553 -mhwmult=none -I"/Applications/ti/ccs910/ccs/ccs_base/msp430/include_gcc" -I"/Users/apple/Desktop/ece3430/cpe_capstone" -I"/Applications/ti/ccs910/ccs/tools/compiler/msp430-gcc-8.2.0.52_macos/msp430-elf/include" -Og -g -gdwarf-3 -gstrict-dwarf -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


