#include <stdio.h>
#include "hdc1000.h"
#include "ccs811.h"
#include "bh1750fvi.h"

typedef struct Measurement
{
    int16_t temperature;
    int16_t humidity;
    uint16_t light;
    uint16_t tvoc;
    uint16_t eco2;
} measurement_t;

measurement_t measure(const hdc1000_t *s, const ccs811_t *c, const bh1750fvi_t *l);
void print_values(measurement_t *m);
void print_raw_values(measurement_t *m);
int is_valid(measurement_t *m);
