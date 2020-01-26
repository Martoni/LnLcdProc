
/*!
    \file  main.c
    \brief Entry point
    \version 0.1
*/

#include "gd32vf103.h"
#include "drv_usb_hw.h"
#include "cdc_acm_core.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lcd.h"

#define FONT_COLOR GREEN

/* USB part */
extern uint8_t packet_sent, packet_receive;
extern uint32_t receive_length;
extern uint8_t usb_data_buffer[CDC_ACM_DATA_PACKET_SIZE];

usb_core_driver USB_OTG_dev =
{
    .dev = {
        .desc = {
            .dev_desc       = (uint8_t *)&device_descriptor,
            .config_desc    = (uint8_t *)&configuration_descriptor,
            .strings        = usbd_strings,
        }
    }
};

// Write to USB CDC, reading should be similar
void cdc_print (usb_dev *pudev, uint8_t *str, uint32_t data_len)
{
    packet_sent = 0;
    usbd_ep_send(pudev, CDC_ACM_DATA_IN_EP, str, data_len);
}

/* RGB LED is connected as follows:
    PC13 : Red
    PA1  : Green
    PA2  : Blue
*/

void led_init()
{
    /* enable the led clock */
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOA);

    /* configure led GPIO port */
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);

    GPIO_BOP(GPIOC) = GPIO_PIN_13;
    GPIO_BOP(GPIOA) = GPIO_PIN_1;
    GPIO_BOP(GPIOA) = GPIO_PIN_2;
}
/*!
    Turn on the red part of the RGB LED
    by clearing PC13
*/
void red_on()
{
    GPIO_BC(GPIOC) = GPIO_PIN_13;
}
/*!
    Turn off the red part of the RGB LED
    by setting PC13
*/
void red_off()
{
    GPIO_BOP(GPIOC) = GPIO_PIN_13;
}
/*!
    Turn on the blue part of the RGB LED
    by clearing PA1
*/
void green_on()
{
    GPIO_BC(GPIOA) = GPIO_PIN_1;
}
/*!
    Turn off the green part of the RGB LED
    by setting PA1
*/
void green_off()
{
    GPIO_BOP(GPIOA) = GPIO_PIN_1;
}
/*!
    Turn on the blue part of the RGB LED
    by clearing PA2
*/
void blue_on()
{
    GPIO_BC(GPIOA) = GPIO_PIN_2;
}
/*!
    Turn off the blue part of the RGB LED
    by setting PA2
*/
void blue_off()
{
    GPIO_BOP(GPIOA) = GPIO_PIN_2;
}


/*!
    \brief      delay a time in milliseconds
    \param[in]  count: count in milliseconds
    \param[out] none
    \retval     none
*/
void delay_1ms(uint32_t count)
{
    uint64_t start_mtime, delta_mtime;

    // Don't start measuruing until we see an mtime tick
    uint64_t tmp = get_timer_value();
    do {
    start_mtime = get_timer_value();
    } while (start_mtime == tmp);

    do {
    delta_mtime = get_timer_value() - start_mtime;
    }while(delta_mtime <(SystemCoreClock/4000.0 *count ));
}


/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    led_init();
    Lcd_Init();// init OLED
    LCD_Clear(WHITE);
    BACK_COLOR=GREEN;
    red_off();
    green_off();
    blue_off();
    LCD_Clear(FONT_COLOR);

    uint8_t strin[100] = "Test\r\n";
    int i = 0;
    eclic_global_interrupt_enable();
    eclic_priority_group_set(ECLIC_PRIGROUP_LEVEL2_PRIO2);
    usb_rcu_config();
    usb_timer_init();
    usb_intr_config();
    usbd_init (&USB_OTG_dev, USB_CORE_ENUM_FS, &usbd_cdc_cb);
    /* check if USB device is enumerated successfully */
    while (USBD_CONFIGURED != USB_OTG_dev.dev.cur_status) {
    }

    LCD_Clear(FONT_COLOR);
    green_on();
    while(1){
        if (USBD_CONFIGURED == USB_OTG_dev.dev.cur_status) {
            if (1 == packet_receive && 1 == packet_sent) {
                packet_sent = 0;
                /* receive datas from the host when the last packet datas have sent to the host */
                cdc_acm_data_receive(&USB_OTG_dev);
            } else {
                if (0 != receive_length) {
		            /* print receive data on LCD */
		            LCD_ShowString(0,  0, usb_data_buffer, BLACK);
                    receive_length = 0;
                }
            }
        }
        delay_1ms(500);
    }
}


