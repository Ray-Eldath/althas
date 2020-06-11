//
// Created by Ray Eldath on 2020/6/7 0007.
//

#ifndef ALTHAS_CONFIG_H
#define ALTHAS_CONFIG_H

// config macros
#define RESET_LINE_ENABLED // whether the DUT have the RESET line. if not, logic for reset will not be compiled.
#define CLOCK_LINE_ENABLED // whether the DUT have the CLOCK line. if not, logic for clocking will not be compiled.
// #define FAIL_ASSERTION // whether use a C assert at every althas assertion. if enable, failed assertion will cause the program interrupted.

#define CLOCK_WIRE clk // the name of the clock wire
#define RESET_WIRE rst // the name of the reset wire

#endif //ALTHAS_CONFIG_H
