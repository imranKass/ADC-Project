/*
 * @file main.c
 *
 * @brief Main source code for the ADC program.
 *
 * This file contains the main entry point and function definitions for the ADC program.
 *
 * It interfaces with the following:
 *  - EduBase Board Potentiometer (PE2)
 *  - EduBase Board Analog Light Sensor (PE1)
 *
 * @author
 */
#include "ADC.h"
#include "EduBase_LCD.h"
#include "GPIO.h"
#include "SysTick_Delay.h"
#include "TM4C123GH6PM.h"
#include "math.h"

static double adc_buffer[8];
int value;
char buffer[8];

int Analog_Voltage_to_Digital(double voltage, double in_min, double in_max,
                              int out_min, int out_max) {
  if (voltage < in_min) {
    return out_min;
  }
  if (voltage > in_max) {
    return out_max;
  }

  return (int)(((voltage - in_min) * (out_max - out_min)) / (in_max - in_min) +
               out_min);
}

int main(void) {
  SysTick_Delay_Init();
  EduBase_LCD_Init();
  ADC_Init();
  EduBase_LEDs_Init();

  EduBase_LCD_Enable_Display();
  EduBase_LCD_Clear_Display();
  EduBase_LCD_Set_Cursor(0, 0);
  EduBase_LCD_Display_String("Light Sensor");

  while (1) {
    ADC_Sample(adc_buffer);
    EduBase_LCD_Set_Cursor(0, 1);
    EduBase_LCD_Display_Double(adc_buffer[1]);
    SysTick_Delay1ms(1000);

    value = Analog_Voltage_to_Digital(adc_buffer[1], 0.0, 3.3, 0, 255);

    // Display converted value
    EduBase_LCD_Set_Cursor(13, 1);
    sprintf(buffer, "%3d", value);
    EduBase_LCD_Display_String(buffer);

    // Control LEDs based on converted value
    if (value >= 200)

      EduBase_LEDs_Output(EDUBASE_LED_ALL_ON);

    else

      EduBase_LEDs_Output(EDUBASE_LED_ALL_OFF);
    SysTick_Delay1ms(1000);
  }
}