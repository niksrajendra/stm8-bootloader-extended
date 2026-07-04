#ifndef     PLATFORM_H
#define     PLATFORM_H

#if defined(CHIP_STM8S005K6)
    #include "stm8s005.h"
#elif defined(CHIP_STM8S003F3)
    #include "stm8s003.h"
#elif defined(CHIP_STM8S007C8)
    #include "stm8s007.h"
#else
    #error "No valid MCU target defined in Makefile!"
#endif

#endif  //PLATFORM_H