
/*!
    \file  main.c
    \brief Entry point
    \version 0.1
*/

#include "gd32vf103.h"
#include <stdio.h>
#include "lcd.h"

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
    LCD_Clear(RED);
    while(1){
        red_on();
        LCD_Clear(RED);
        delay_1ms(100);
        red_off();
        delay_1ms(100);
        green_on();
        LCD_Clear(GREEN);
        delay_1ms(100);
        green_off();
        delay_1ms(100);
        blue_on();
        LCD_Clear(BLUE);
        delay_1ms(100);
        blue_off();
        delay_1ms(100);
    }
}
