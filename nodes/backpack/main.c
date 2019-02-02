/*
 * Copyright (C) 2018 Koen Zandberg <koen@bergzand.net>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     tests
 * @{
 *
 * @file
 * @brief       Littlevgl LVGL test application
 *
 * @author      Koen Zandberg <koen@bergzand.net>
 * @}
 */

#include <stdio.h>
#include "msg.h"
#include "xtimer.h"
#include "lvgl.h"
#include "measure.h"

#include "lvgl.h"
#include "demo.h"

#include "fmt.h"
#include "hdc1000.h"
#include "ccs811.h"
#include "ili9341.h"
#include "ili9341_params.h"
#include "hdc1000_params.h"
#include "ccs811_params.h"

static ili9341_t dev;
static lv_disp_drv_t disp_drv;
static hdc1000_t sensor;
static ccs811_t co2;
static lv_obj_t *temp_ob, *humm_ob, *co2_ob;
static lv_obj_t *chart_ob;

static const char prefix_temp[] = "Temperature: ";
static const char prefix_humm[] = "Humidity: ";
static const char prefix_co2[] = "co2: ";

static char templine[100];
static char hummline[100];
static char co2line[100];

static lv_chart_series_t *ser_temp;

static void disp_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t* color)
{
    ili9341_map(&dev, x1, x2, y1, y2, (const uint16_t*)color);
    lv_flush_ready();
}

static void disp_map(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t* color)
{
    ili9341_map(&dev, x1, x2, y1, y2, (const uint16_t*)color);
}

static void disp_fill_conv(int32_t x1, int32_t y1, int32_t x2, int32_t y2, lv_color_t color)
{
    ili9341_fill(&dev, x1, x2, y1, y2, lv_color_to16(color));
}

static void sensor_init(void)
{
    /* Initialise HDC1000 */
    int init = hdc1000_init(&sensor, hdc1000_params);
    if (init != HDC1000_OK) {
        printf("HDC1000 INIT FAILED: %i\n", init);
    }

    /* Initialise HDC1000 */
    init = ccs811_init(&co2, ccs811_params);
    if (init != CCS811_OK) {
        printf("CCS811 INIT FAILED: %i\n", init);
    }
	puts("Sensors initialized");
}

/**
 * Initialize the Hardware Abstraction Layer (HAL) for the Littlev graphics library
 */
static void hal_init(void)
{
    /* Add a display */
    printf("Initializing display...");

    if (ili9341_init(&dev, &ili9341_params[0]) == 0) {
        puts("[OK]");
    }
    else {
        puts("[Failed]");
    }

    lv_disp_drv_init(&disp_drv);            /*Basic initialization*/
    disp_drv.disp_flush = disp_flush;
    disp_drv.disp_fill = disp_fill_conv;
    disp_drv.disp_map = disp_map;
    lv_disp_drv_register(&disp_drv);
}
void screen_create(void) {
    temp_ob = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(temp_ob, templine);
    lv_obj_align(temp_ob, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 10);  /*Align to the top*/

	humm_ob = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(humm_ob, hummline);
    lv_obj_align(humm_ob, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 35);  /*Align to the top*/

	co2_ob = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(co2_ob, co2line);
    lv_obj_align(co2_ob, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 60);  /*Align to the top*/

    chart_ob = lv_chart_create(lv_scr_act(), NULL);
    lv_obj_set_size(chart_ob, 100, 100);
    lv_obj_align(chart_ob, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 85);
    lv_chart_set_type(chart_ob, LV_CHART_TYPE_LINE);
    lv_chart_set_series_width(chart_ob, 4);

    lv_chart_set_point_count(chart_ob, 30);
    lv_chart_set_range(chart_ob, -10, 35);
    ser_temp = lv_chart_add_series(chart_ob, LV_COLOR_RED);
}

int main(void)
{
    puts("RIOT littlevgl test application");

	sensor_init();
    /*Initialize LittlevGL*/
    lv_init();
    hal_init();

    /*Load a demo*/
    //lv_theme_t *th = lv_theme_alien_init(10, NULL);
    screen_create();
    while(1)
    {
    	measurement_t m = measure(&sensor, &co2, NULL);
		memcpy(templine, prefix_temp, strlen(prefix_temp));
		size_t offset = strlen(prefix_temp);
		offset += fmt_s16_dfp(&templine[offset], m.temperature, -2);
		memcpy(&templine[offset], "°C", 3); // ° is 2 byte
		offset += 3;
		templine[offset] = '\0';

		memcpy(hummline, prefix_humm, strlen(prefix_humm));
		offset = strlen(prefix_humm);
		offset += fmt_s16_dfp(&hummline[offset], m.humidity, -2);
		memcpy(&hummline[offset], "%H", 2);
		offset += 2;
		hummline[offset] = '\0';

		memcpy(co2line, prefix_co2, strlen(prefix_co2));
		offset = strlen(prefix_co2);
		offset += fmt_s16_dfp(&co2line[offset], m.eco2, 0);
		memcpy(&co2line[offset], "ppm", 3);
		offset += 3;
		co2line[offset] = '\0';

		lv_label_set_text(temp_ob, templine);
		lv_label_set_text(humm_ob, hummline);
		lv_label_set_text(co2_ob, co2line);

        lv_chart_set_next(chart_ob, ser_temp, m.temperature/100);

        lv_task_handler();
        lv_tick_inc(5000);
        xtimer_sleep(5);
		print_values(&m);
    }
    /* should be never reached */
    return 0;
}
