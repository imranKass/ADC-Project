/**
 * @file ADC.c
 *
 * @brief Source code for the ADC driver.
 *
 * This file contains the function definitions for the ADC driver.
 *
 * ADC Module 0 is used to sample the potentiometer and the analog
 * light sensor that are connected on the EduBase board. ADC Module 0
 * is configured to sample the potentiometer first and the light sensor after.
 *
 * After the light sensor is sampled, an interrupt signal is set to 
 * indicate that the sampling sequence has ended. After the conversion
 * results have been read from the corresponding FIFO, the interrupt is cleared.
 *
 * The following pins are used:
 *  - Potentiometer   <-->  Tiva LaunchPad Pin PE2 (Channel 1)
 *  - Light Sensor    <-->  Tiva LaunchPad Pin PE1 (Channel 2)
 *
 * @Irvin
 *
 */

#include "ADC.h"

void ADC_Init(void)
{
    SYSCTL->RCGCADC |= 0x01;     // Enable ADC0 clock
    SysTick_Delay1ms(1);

    SYSCTL->RCGCGPIO |= 0x10;    // Enable GPIOE clock

    GPIOE->DIR &= ~0x06;         // Configure PE1 and PE2 as inputs 

    GPIOE->DEN &= ~0x06;         // Disable digital function for PE1 and PE2

    GPIOE->AMSEL |= 0x06;        // Enable analog function for PE1 and PE2

    GPIOE->AFSEL |= 0x06;        // Enable alternate function for PE1 and PE2

    ADC0->ACTSS &= ~0x1;         // Disable SS0 during configuration

    ADC0->EMUX &= ~0x000F;       // Configure for processor-triggered sampling

    ADC0->SSMUX0 &= ~0xFFFFFFFF; // Clear SS0 input selections

    ADC0->SSMUX0 = 0x0021;       // Set channel 1 (PE2) for first sample, channel 2 (PE1) for second

    ADC0->SSCTL0 |= 0x00000020; // Clear SSCTL0 register
    ADC0->SSCTL0 |= 0x00000060;  // Set END1 (bit 5) and IE1 (bit 6) for second sample

    ADC0->ACTSS |= 0x1;          // Enable SS0 now that it's configured
}

void ADC_Sample(double analog_value_buffer[])
{
ADC0->PSSI |= 0x01;
	
	while((ADC0->RIS & 0x01) == 0);
	
	uint32_t potentiometer_sample_result = ADC0->SSFIFO0;
	uint32_t light_sensor_sample_result = ADC0->SSFIFO0;
	
	ADC0->ISC |= 0x01;
	
	analog_value_buffer[0] = (potentiometer_sample_result * 3.3) / 4096;
	analog_value_buffer[1] = (light_sensor_sample_result * 3.3) / 4096;
}


