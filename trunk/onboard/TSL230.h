#ifndef TSL230_h
#define TSL230_h

#include <stdint.h>

// TSL230R Pin Definitions

// output use digital pin2 for interrupt
#define TSL_FREQ_PIN 2

#define TSL_S0       31
#define TSL_S1       25
#define TSL_S2       29
#define TSL_S3       27


void tsl230_init();
unsigned long tsl230_get_freq();
float tsl230_calc_lux_single(float uw_cm2, float efficiency);
float tsl230_calc_uwatt_cm2(unsigned long freq);
void tsl230_set_scaling(int what);
void tsl230_sensitivity(uint8_t dir);


#endif // TSL230_h


