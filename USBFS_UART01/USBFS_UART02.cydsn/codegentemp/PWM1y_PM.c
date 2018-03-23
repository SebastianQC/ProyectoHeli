/*******************************************************************************
* File Name: PWM1y_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "PWM1y.h"

static PWM1y_backupStruct PWM1y_backup;


/*******************************************************************************
* Function Name: PWM1y_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM1y_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void PWM1y_SaveConfig(void) 
{

    #if(!PWM1y_UsingFixedFunction)
        #if(!PWM1y_PWMModeIsCenterAligned)
            PWM1y_backup.PWMPeriod = PWM1y_ReadPeriod();
        #endif /* (!PWM1y_PWMModeIsCenterAligned) */
        PWM1y_backup.PWMUdb = PWM1y_ReadCounter();
        #if (PWM1y_UseStatus)
            PWM1y_backup.InterruptMaskValue = PWM1y_STATUS_MASK;
        #endif /* (PWM1y_UseStatus) */

        #if(PWM1y_DeadBandMode == PWM1y__B_PWM__DBM_256_CLOCKS || \
            PWM1y_DeadBandMode == PWM1y__B_PWM__DBM_2_4_CLOCKS)
            PWM1y_backup.PWMdeadBandValue = PWM1y_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(PWM1y_KillModeMinTime)
             PWM1y_backup.PWMKillCounterPeriod = PWM1y_ReadKillTime();
        #endif /* (PWM1y_KillModeMinTime) */

        #if(PWM1y_UseControl)
            PWM1y_backup.PWMControlRegister = PWM1y_ReadControlRegister();
        #endif /* (PWM1y_UseControl) */
    #endif  /* (!PWM1y_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWM1y_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM1y_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWM1y_RestoreConfig(void) 
{
        #if(!PWM1y_UsingFixedFunction)
            #if(!PWM1y_PWMModeIsCenterAligned)
                PWM1y_WritePeriod(PWM1y_backup.PWMPeriod);
            #endif /* (!PWM1y_PWMModeIsCenterAligned) */

            PWM1y_WriteCounter(PWM1y_backup.PWMUdb);

            #if (PWM1y_UseStatus)
                PWM1y_STATUS_MASK = PWM1y_backup.InterruptMaskValue;
            #endif /* (PWM1y_UseStatus) */

            #if(PWM1y_DeadBandMode == PWM1y__B_PWM__DBM_256_CLOCKS || \
                PWM1y_DeadBandMode == PWM1y__B_PWM__DBM_2_4_CLOCKS)
                PWM1y_WriteDeadTime(PWM1y_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(PWM1y_KillModeMinTime)
                PWM1y_WriteKillTime(PWM1y_backup.PWMKillCounterPeriod);
            #endif /* (PWM1y_KillModeMinTime) */

            #if(PWM1y_UseControl)
                PWM1y_WriteControlRegister(PWM1y_backup.PWMControlRegister);
            #endif /* (PWM1y_UseControl) */
        #endif  /* (!PWM1y_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWM1y_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM1y_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWM1y_Sleep(void) 
{
    #if(PWM1y_UseControl)
        if(PWM1y_CTRL_ENABLE == (PWM1y_CONTROL & PWM1y_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWM1y_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWM1y_backup.PWMEnableState = 0u;
        }
    #endif /* (PWM1y_UseControl) */

    /* Stop component */
    PWM1y_Stop();

    /* Save registers configuration */
    PWM1y_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM1y_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM1y_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWM1y_Wakeup(void) 
{
     /* Restore registers values */
    PWM1y_RestoreConfig();

    if(PWM1y_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWM1y_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
