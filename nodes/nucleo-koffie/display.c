
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "xtimer.h"
#include "msg.h"
#include "kernel_types.h"
#include "errno.h"
#include "shell.h"
#include "hd44780.h"
#include "periph/pwm.h"
#include "periph/adc.h"
#include "saul_reg.h"
#include "saul.h"
#include "phydat.h"
#include "evtimer.h"
#include "evtimer_msg.h"
#include "display.h"
#include "fmt.h"

#define SCROLL_SPEED    1000U
#define TEMP_DELAY    10000U
#define RES             ADC_RES_10BIT
#define DELAY           (100U)


static char display_stack[THREAD_STACKSIZE_DEFAULT];
static evtimer_t display_evtimer;
//static char long_str[] = "Hello from RIOT-OS, this is a long string test. ";
static char temp_str[17] = "Temp: ";

static hd44780_params_t params =
{
    .cols   = 16,                   \
    .rows   = 2,                    \
    .rs     = (GPIO_PIN(0, 9)),     \
    .rw     = HD44780_RW_OFF,       \
    .enable = (GPIO_PIN(2,7)),        \
    .data   = {(GPIO_PIN(1, 5)), (GPIO_PIN(1, 4)), (GPIO_PIN(1, 10)), (GPIO_PIN(0, 8)), \
               HD44780_RW_OFF, HD44780_RW_OFF, HD44780_RW_OFF, HD44780_RW_OFF} 
};

//static void _hd44780_print_substr(hd44780_t *dev, char *str, uint8_t offset, size_t length)
//{
//    for (uint8_t i = 0; i < length; i++) {
//        uint8_t curpos = (i + offset)%strlen(str);
//        hd44780_write(dev, str[curpos]);
//    }
//}

static void *display_thread(void *args) {
    (void)args;
    uint8_t brightness = 50;
    saul_reg_t *temp_reg = saul_reg_find_type(SAUL_SENSE_TEMP);
    phydat_t temp_measure;
    msg_t msg, _display_msg_queue[4];
    msg_init_queue(_display_msg_queue, 4);
    hd44780_t dev;
    
    pwm_init(PWM_DEV(2), PWM_LEFT, 2000U, 250U);
    pwm_set(PWM_DEV(2), 0, brightness);

    adc_init(ADC_LINE(0));

    /* initialize evtimer */
    evtimer_msg_event_t _button_event = { .event = { .offset = DELAY }, .msg = { .type = DISPLAY_MSG_BUTTON }};
    evtimer_msg_event_t _temp_event = { .event = { .offset = 10 }, .msg = { .type = DISPLAY_MSG_TEMP }};
    evtimer_init_msg(&display_evtimer);
    evtimer_add_msg(&display_evtimer, &_button_event, sched_active_pid);
    evtimer_add_msg(&display_evtimer, &_temp_event, sched_active_pid);


    //uint8_t scroll_offset = 0;
    //uint8_t direction = 1;

    hd44780_init(&dev, &params);
    hd44780_clear(&dev);
    hd44780_home(&dev);
    puts("Initialized device");
    for(;;) {
        msg_receive(&msg);
        switch(msg.type) {
            case DISPLAY_MSG_BUTTON:
                _button_event.event.offset = DELAY;
                evtimer_add_msg(&display_evtimer, &_button_event, sched_active_pid);
                int sample = adc_sample(ADC_LINE(0), RES);
                if ( sample > 1000 ) {
                /* select button */
                    printf("select button\n");
                } else if ( sample < 10 ) {
                    /* right */
                    printf("right button\n");
                } else if ( sample < 250 && sample > 200 ) {
                    if(brightness < 250) {
                        brightness+=10;
                    }
                    pwm_set(PWM_DEV(2), 0, brightness);
                    printf("up button\n");
                } else if ( sample < 770 && sample > 750 ) {
                    printf("left button\n");
                } else if ( sample < 550 && sample > 450 ) {
                    if(brightness > 0) {
                        brightness-=10;
                    }
                    pwm_set(PWM_DEV(2), 0, brightness);
                    printf("down button\n");
                }
                break;
            case DISPLAY_MSG_TEMP:
                _temp_event.event.offset = TEMP_DELAY;
                evtimer_add_msg(&display_evtimer, &_temp_event, sched_active_pid);
                saul_reg_read(temp_reg, &temp_measure);
                fmt_s16_dfp(&temp_str[6], temp_measure.val[0], -temp_measure.scale);
                printf("temperature scale: %d\n", -1 * temp_measure.scale);
                hd44780_set_cursor(&dev, 0, 0);
                hd44780_print(&dev, temp_str);
                break;
            default:
                puts("Error");
        }


    }
    return NULL;
}

void display_init(void)
{
    int display_pid = thread_create(display_stack, sizeof(display_stack),
                                    THREAD_PRIORITY_MAIN + 1,
                                    THREAD_CREATE_STACKTEST, display_thread,
                                    NULL, "Display");
    if (display_pid == -EINVAL || display_pid == -EOVERFLOW) {
        puts("Error: failed to create sensor thread, exiting\n");
    }
    else {
        puts("Successfuly created sensor thread !\n");
    }
}
