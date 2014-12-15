/* Name: main.c
 * Author: Francisco de Villalobos
 * Copyright: Francisco de Villalobos
 * License: Francisco de Villalobos
 */


#include <avr/interrupt.h>
#include <avr/io.h>
#include "Settings.h"
#include "math.h"
#include "m_usb.h"
#include "m_general.h"
#include "m_bus.h"
#include "m_rf.h"

char in_key = 'a';
int timer_count = 0;

int main(void)
{
/******************************************************************************************/
/*****************                    Initialization                      *****************/
/******************************************************************************************/
    
    init();                             // Run Initialization Code for Ports and Timers. Settings.c
    sei();
    
    // Outputs
    set(DDRC, 6);       // Dir 1
    set(DDRC, 7);       // Dir 2
    set(DDRE, 6);       // Enable
    set(DDRB, 1);
    set(DDRB, 2);
    set(DDRB, 6);       // Timer 1 PWM Output
    
    // Initialize Outputs
    set(PORTF, 4);
    clear(PORTF, 6);
    
    // Start Direction
    set(PORTC, 6);
    clear(PORTC, 7);
    
/******************************************************************************************/
/*****************                       Code Loop                        *****************/
/******************************************************************************************/
    while(1){
    
        // Exit Screen With CTRL+A, then CTRL+\ and then y.
        
        // Makes the last recieved signal last for a second if nothing new comes in
        if(timer_count < 100){
            set(PORTE, 6);          // Turn Enable ON
            m_green(OFF);
        }
        else{
            clear(PORTE, 6);        // Turn Enable OFF
            m_green(ON);
        }
        
        m_usb_tx_string("  Timer Count = ");
        m_usb_tx_uint(timer_count);
        m_usb_tx_string("                \r");
        
        // If it recieves something over the serial port
        if(m_usb_rx_available())
        {
            in_key = m_usb_rx_char();           // Get input char
            m_usb_tx_uint(in_key);              // Echo Char
            
            // We cannot use the RED LED!!!!!
            
            switch(in_key){
                case key_e:
                    // Do fast speed and reset timer
                    timer_count = 0;
                    OCR1B = 15000;
                    break;
                    
                case key_g:
                    // Do slow speed and reset timer
                    timer_count = 0;
                    OCR1B = 2000;
                    break;
                
                // Increase duty cyle by keyboard
                case key_plus:
                    OCR1B = OCR1B + 100;
                    break;
                    
                // Decrease Duty Cycle by keyboard
                case key_minus:
                    OCR1B = OCR1B - 100;
                    break;
                    
                case key_equal:
                    OCR1B = OCR1B + 100;
                    break;
                    
                case key_underscore:
                    OCR1B = OCR1B - 100;
                    break;

                default:
                    m_usb_tx_string(" Invalid Input");
                    break;
            }                                           // End Switch Case.
//            m_usb_tx_string("  OCR1B = ");
//            m_usb_tx_uint(OCR1B);
//            m_usb_tx_string("                \r");
        }
    }

    return 1;                       /* never reached */

}


/******************************************************************************************/
/*****************                    Interrupt Handler                   *****************/
/******************************************************************************************/
//


ISR(INT2_vect){                                     // Wi-Fi Reciever Interrupt Handler.

}


// Interrupt Handler for Timer 3 Overflow.
// Counter used to count 1s for disabeling the Motor ENABLE
ISR(TIMER3_OVF_vect){
    timer_count += 1;
}

// Timer 3 is used to change the duty cycle pin of the motor
ISR(TIMER1_OVF_vect){
    //
}






