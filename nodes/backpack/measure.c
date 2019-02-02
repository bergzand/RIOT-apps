#include "measure.h"

measurement_t measure(const hdc1000_t *s, const ccs811_t *c, const bh1750fvi_t *l)
{
    measurement_t m;

    /* Measure temperature/humidity */
    if (s != NULL) {
        int res = hdc1000_read(s, &m.temperature, &m.humidity);
        if (res != HDC1000_OK) {
            return m;
        }
    }

    /* Measure CO2 and volatile gas */
    if (c != NULL) {
        int res = ccs811_set_environmental_data(c, m.temperature, m.humidity);
        if (res != CCS811_OK) {
            return m;
        }

        res = ccs811_read_iaq(c, &m.tvoc, &m.eco2, NULL, NULL);
        if (res != CCS811_OK) {
            return m;
        }
    }

    /* Measure light */
    if (l != NULL) {
        m.light = bh1750fvi_sample(l);
    }
    return m;
}

void print_values(measurement_t *m)
{
    int16_t t_v;
    int16_t h_v;
    uint8_t t_d, h_d;

    t_v = m->temperature / 100;
    t_d = m->temperature % 100;
    h_v = m->humidity / 100;
    h_d = m->humidity % 100;

    printf("%d.%02u °C; %d.%02u %%H, %d lx, %d ppb, %d ppm\n",
           t_v, t_d, h_v, h_d, m->light, m->tvoc, m->eco2);
}

void print_raw_values(measurement_t *m)
{
    printf("%d,%d,%d,%d,%d\n", m->temperature, m->humidity, m->light, m->tvoc, m->eco2);
}

int is_valid(measurement_t *m)
{
    // return (m->t != -32768) && (m->l != 21333);
    return m->eco2 > 0;
}
