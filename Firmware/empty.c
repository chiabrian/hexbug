/*
 * Copyright (c) 2021, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "ti_msp_dl_config.h"

typedef enum{
    STATE_FWD,
    STATE_REV,
    STATE_LEFT,
    STATE_RIGHT,
    STATE_TRIG_LEFT,
    STATE_TRIG_RIGHT,
    STATE_IDLE,
}state_t;

volatile int32_t g_sys_tick = 0;
volatile state_t curr_state = STATE_IDLE;
volatile state_t next_state = STATE_IDLE;

void ir_send_start(void)
{
    DL_TimerG_stopCounter(PWM_IR_INST);
    DL_TimerG_stopCounter(TIMER_0_INST);   
    DL_TimerG_setLoadValue(TIMER_0_INST,1739);
    DL_TimerG_startCounter(TIMER_0_INST);            
    DL_TimerG_startCounter(PWM_IR_INST);
    while(DL_TimerG_isRunning(TIMER_0_INST)){}
    DL_TimerG_stopCounter(PWM_IR_INST);
}

void ir_send_off(void)
{
    DL_TimerG_stopCounter(PWM_IR_INST);
    DL_TimerG_stopCounter(TIMER_0_INST);   
    DL_TimerG_setLoadValue(TIMER_0_INST,270);
    DL_TimerG_startCounter(TIMER_0_INST);          
    while(DL_TimerG_isRunning(TIMER_0_INST)){}
    
    DL_TimerG_setLoadValue(TIMER_0_INST,400);
    DL_TimerG_startCounter(TIMER_0_INST);     
    DL_TimerG_startCounter(PWM_IR_INST);
    while(DL_TimerG_isRunning(TIMER_0_INST)){}
    DL_TimerG_stopCounter(PWM_IR_INST);
}

void ir_send_on(void)
{
    DL_TimerG_stopCounter(PWM_IR_INST);
    DL_TimerG_stopCounter(TIMER_0_INST);   
    DL_TimerG_setLoadValue(TIMER_0_INST,640);
    DL_TimerG_startCounter(TIMER_0_INST);
    DL_TimerG_startCounter(PWM_IR_INST);
    while(DL_TimerG_isRunning(TIMER_0_INST)){}
    DL_TimerG_stopCounter(PWM_IR_INST);
}

void ir_send_fwd(void)
{
    ir_send_start();
    ir_send_off();
    ir_send_on();
    ir_send_off();
    ir_send_off();
    ir_send_off();
    ir_send_off();
    ir_send_off();
}

void ir_send_rev(void)
{
    ir_send_start();
    ir_send_off();
    ir_send_off();
    ir_send_off();
    ir_send_off();
    ir_send_on();
    ir_send_off();
    ir_send_off();
}

void ir_send_left(void)
{
    ir_send_start();
    ir_send_off();
    ir_send_off();
    ir_send_on();
    ir_send_off();
    ir_send_off();
    ir_send_off();
    ir_send_off();
}

void ir_send_right(void)
{
    ir_send_start();
    ir_send_off();
    ir_send_off();
    ir_send_off();
    ir_send_on();
    ir_send_off();
    ir_send_off();
    ir_send_off();
}

void ir_send_trig_left(void)
{
    ir_send_start();
    ir_send_off();
    ir_send_off();
    ir_send_off();
    ir_send_off();
    ir_send_off();
    ir_send_on();
    ir_send_off();
}

void ir_send_trig_right(void)
{
    ir_send_start();
    ir_send_off();
    ir_send_off();
    ir_send_off();
    ir_send_off();
    ir_send_off();
    ir_send_on();
    ir_send_off();
}

void ir_send_ch1(void)
{
    ir_send_on();
    ir_send_off();
    ir_send_on();
    ir_send_off();
    ir_send_on();
}

void ir_send_ch2(void)
{
    ir_send_off();
    ir_send_off();
    ir_send_on();
}

void ir_send_ch3(void)
{
    ir_send_on();
    ir_send_off();
    ir_send_off();
}

void ir_send_ch4(void)
{
    ir_send_off();
    ir_send_on();
    ir_send_off();
}

int main(void)
{
    SYSCFG_DL_init();

    while (1) {
        if(g_sys_tick == 500)
        {
            g_sys_tick = 0;
            // DL_GPIO_togglePins(LED_G_PORT, LED_G_PIN_1_PIN);
            if( DL_TimerG_isRunning(PWM_0_INST))
            {
                DL_TimerG_stopCounter(PWM_0_INST);
            }
            else
            {
                DL_TimerG_startCounter(PWM_0_INST);
            }

            curr_state = next_state;
            next_state = STATE_IDLE;
            if( curr_state != STATE_IDLE)
            {
                switch(curr_state)
                {
                    case STATE_FWD:
                        ir_send_fwd();
                        break;
                    case STATE_REV:
                        ir_send_rev();
                        break;
                    case STATE_LEFT:
                        ir_send_left();
                        break;
                    case STATE_RIGHT:
                        ir_send_right();
                        break;
                    case STATE_TRIG_LEFT:        
                        ir_send_trig_left();
                        break;
                    case STATE_TRIG_RIGHT:
                        ir_send_trig_right();
                        break;
                    default:                        
                        break;
                }
                if (DL_GPIO_readPins(GPIOA,CH2_SW7_2_PIN) == 0)
                {
                    ir_send_ch2();
                }
                else if (DL_GPIO_readPins(GPIOA,CH3_SW7_3_PIN) == 0)
                {
                    ir_send_ch3();
                }
                else if (DL_GPIO_readPins(GPIOA,CH4_SW7_4_PIN) == 0)
                {
                    ir_send_ch4();
                }
                else {
                    ir_send_ch1();
                }
            }
            // // CH1 Forward
            // ir_send_start();
            // ir_send_off();
            // ir_send_on();
            // ir_send_off();
            // ir_send_off();
            // ir_send_off();
            // ir_send_off();
            // ir_send_off();

            // ir_send_on();
            // ir_send_off();
            // ir_send_on();
            // ir_send_off();
            // ir_send_on();

            // // CH1 Reverse
            // ir_send_start();
            // ir_send_off();
            // ir_send_off();
            // ir_send_off();
            // ir_send_off();
            // ir_send_on();
            // ir_send_off();
            // ir_send_off();

            // ir_send_on();
            // ir_send_off();
            // ir_send_on();
            // ir_send_off();
            // ir_send_on();

            // // CH1 Left
            // ir_send_start();
            // ir_send_off();
            // ir_send_off();
            // ir_send_on();
            // ir_send_off();
            // ir_send_off();
            // ir_send_off();
            // ir_send_off();

            // ir_send_on();
            // ir_send_off();
            // ir_send_on();
            // ir_send_off();
            // ir_send_on();

            // // CH1 Right
            // ir_send_start();
            // ir_send_off();
            // ir_send_off();
            // ir_send_off();
            // ir_send_on();
            // ir_send_off();
            // ir_send_off();
            // ir_send_off();

            // ir_send_on();
            // ir_send_off();
            // ir_send_on();
            // ir_send_off();
            // ir_send_on();

            // // CH1 Trigger
            // ir_send_start();
            // ir_send_off();
            // ir_send_off();
            // ir_send_off();
            // ir_send_off();
            // ir_send_off();
            // ir_send_on();
            // ir_send_off();

            // ir_send_on();
            // ir_send_off();
            // ir_send_on();
            // ir_send_off();
            // ir_send_on();

            // // CH2 Forward
            // ir_send_start();
            // ir_send_off();
            // ir_send_on();
            // ir_send_off();
            // ir_send_off();
            // ir_send_off();
            // ir_send_off();
            // ir_send_off();

            // ir_send_off();
            // ir_send_off();
            // ir_send_on();

            // // CH2 Reverse
            // ir_send_start();
            // ir_send_off();
            // ir_send_off();
            // ir_send_off();
            // ir_send_off();
            // ir_send_on();
            // ir_send_off();
            // ir_send_off();

            // ir_send_off();
            // ir_send_off();
            // ir_send_on();
        }
    }
}

uint32_t button_active(state_t button)
{
    uint32_t active = 0;
    switch(button)
    {
        case STATE_FWD:
            if(DL_GPIO_readPins(GPIOA,SW4_UP_PIN) == 0)
            {
                active = 1;
            }            
            break;
        case STATE_REV:
            if(DL_GPIO_readPins(GPIOA,SW3_DOWN_PIN) == 0)
            {
                active = 1;
            }
            break;
        case STATE_LEFT:
            if(DL_GPIO_readPins(GPIOA,SW5_LEFT_PIN) == 0)
            {
                active = 1;
            }
            break;
        case STATE_RIGHT:
            if(DL_GPIO_readPins(GPIOA,SW6_RIGHT_PIN) == 0)
            {
                active = 1;
            }
            break;
        case STATE_TRIG_LEFT:        
            if(DL_GPIO_readPins(GPIOA,SW2_TRIG_LEFT_PIN) == 0)
            {
                active = 1;
            }
            break;
        case STATE_TRIG_RIGHT:
            if(DL_GPIO_readPins(GPIOA,SW1_TRIG_RIGHT_PIN) == 0)
            {
                active = 1;
            }
            break;
        default:
            active = 0;
            break;
    }
    return active;
}

state_t check_button(state_t current)
{
    state_t button_start;
    state_t button = STATE_IDLE;
    for(int32_t i=0;i<(STATE_IDLE);i++)
    {
        button_start = current + i + 1;
        if(button_start >= STATE_IDLE)
        {
            button_start -= (STATE_IDLE);
        }
        if( button_active(button_start) )
        {
            button = button_start;
            break;
        }         
    }
    return button;
}

void update_state(void)
{
    if(next_state == STATE_IDLE)
    {
        next_state = check_button(curr_state);
    }
}

void SysTick_Handler(void)
{
    g_sys_tick++;
    update_state();
}
