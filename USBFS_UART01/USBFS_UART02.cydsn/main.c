/*******************************************************************************
* File Name: main.c
*
* Version: 2.0
*
* Description:
*   The component is enumerated as a Virtual Com port. Receives data from the 
*   hyper terminal, then sends back the received data.
*   For PSoC3/PSoC5LP, the LCD shows the line settings.
*
* Related Document:
*  Universal Serial Bus Specification Revision 2.0
*  Universal Serial Bus Class Definitions for Communications Devices
*  Revision 1.2
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation. All rights reserved.
* This software is owned by Cypress Semiconductor Corporation and is protected
* by and subject to worldwide patent and copyright laws and treaties.
* Therefore, you may use this software only as provided in the license agreement
* accompanying the software package from which you obtained this software.
* CYPRESS AND ITS SUPPLIERS MAKE NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* WITH REGARD TO THIS SOFTWARE, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT,
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*******************************************************************************/

#include <project.h>
#include "stdio.h"
#include <string.h>


#if defined (__GNUC__)
    /* Add an explicit reference to the floating point printf library */
    /* to allow usage of the floating point conversion specifiers. */
    /* This is not linked in by default with the newlib-nano library. */
    asm (".global _printf_float");
#endif

#define USBFS_DEVICE    (0u)

/* The buffer size is equal to the maximum packet size of the IN and OUT bulk
* endpoints.
*/
#define USBUART_BUFFER_SIZE (64u)





/*******************************************************************************
* Function Name: main
********************************************************************************
*
* Summary:
*  The main function performs the following actions:
*   1. Waits until VBUS becomes valid and starts the USBFS component which is
*      enumerated as virtual Com port.
*   2. Waits until the device is enumerated by the host.
*   3. Waits for data coming from the hyper terminal and sends it back.
*   4. PSoC3/PSoC5LP: the LCD shows the line settings.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/

void enviar(float input,float input2){
    char msg[10];
    char msg2[10];
    char msg3[10];
    char TAB[4] = "; ";
    char TAB2[4] = "\n\r";
    input = input*1000;
    input2 = input2*1000;
    int vv = input;
    int vv1 = input2;
    // Verify if we have to put a tab or endl
     if (0u != USBUART_GetConfiguration())
        {
                /* Wait until component is ready to send data to host. */
                if (0u == USBUART_CDCIsReady())
                {
                }
                /* Send data back to host. */
                    sprintf(msg, "%d", vv);
                    sprintf(msg2, "%d", vv1);
                    strcat(msg,TAB);
                    strcat(msg,msg2);
                    strcat(msg,TAB2);
                    USBUART_PutString(msg);
                    
                    /* If the last sent packet is exactly the maximum packet 
                    *  size, it is followed by a zero-length packet to assure
                    *  that the end of the segment is properly identified by 
                    *  the terminal.
                    */
               
                
                /* Wait until component is ready to send data to PC. */
                    if (0u == USBUART_CDCIsReady())
                    {
                    }
        }
}
int main(void)
{
    CyGlobalIntEnable;
    int output;
    //Buffer del I2C
    //uint8 buffer[1];
    int cnt1 = 0;
    float ang1 = 0;
    float a = 184;
    //Iniciar componentes
    
    LCD_Start();
    //EZ_Start();
    ADC_DelSig_1_Start();
    PWM1p_Start();
    PWM2p_Start();
    PWM1y_Start();
    PWM2y_Start();
    QD_Start();
    QD_SetCounter(0);
    
    //Conversion del ADC
    ADC_DelSig_1_StartConvert();
    
    //LCD Valores YAW Y PITCH
    
    LCD_Position(0u,0u);
    LCD_PrintString("Pitch");
    LCD_Position(0u,7u);
    LCD_PrintString("Yaw");
    /* Start USBFS operation with 5-V operation. */
    USBUART_Start(USBFS_DEVICE, USBUART_5V_OPERATION);
    
    for(;;)
    {
        if(ADC_DelSig_1_IsEndConversion(ADC_DelSig_1_RETURN_STATUS))
        {
            output = ADC_DelSig_1_GetResult8();
            if(output<0){
                output=0;
            }
            int pwm = 125 + output;
            int pwm2 = 125 + output-42;
            PWM1p_WriteCompare(pwm);
            PWM1y_WriteCompare(pwm2);
            LCD_Position(1u,0u);
            LCD_PrintNumber(pwm);
            LCD_Position(1u,7u);
            LCD_PrintNumber(pwm2); 
        }
       
        cnt1 = QD_GetCounter();
        ang1 = 6.2832*cnt1/(2*900.036624);
        
        if(SR_Read()){
            QD_SetCounter(0);
        }
        if(SR2_Read() || SR3_Read()){
            a = 220;
        }
        enviar(a,ang1);
        a = 184;
        CyDelay(20u);
    }
}


/* [] END OF FILE */
