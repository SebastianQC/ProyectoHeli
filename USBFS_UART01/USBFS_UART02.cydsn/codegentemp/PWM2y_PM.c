/*******************************************************************************
* File Name: PWM2y_PM.c
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

#include "PWM2y.h"

static PWM2y_backupStruct PWM2y_backup;


/*******************************************************************************
* Function Name: PWM2y_SaveConfig
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
*  PWM2y_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void PWM2y_SaveConfig(void) 
{

    #if(!PWM2y_UsingFixedFunction)
        #if(!PWM2y_PWMModeIsCenterAligned)
            PWM2y_backup.PWMPeriod = PWM2y_ReadPeriod();
        #endif /* (!PWM2y_PWMModeIsCenterAligned) */
        PWM2y_backup.PWMUdb = PWM2y_ReadCounter();
        #if (PWM2y_UseStatus)
            PWM2y_backup.InterruptMaskValue = PWM2y_STATUS_MASK;
        #endif /* (PWM2y_UseStatus) */

        #if(PWM2y_DeadBandMode == PWM2y__B_PWM__DBM_256_CLOCKS || \
            PWM2y_DeadBandMode == PWM2y__B_PWM__DBM_2_4_CLOCKS)
            PWM2y_backup.PWMdeadBandValue = PWM2y_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(PWM2y_KillModeMinTime)
             PWM2y_backup.PWMKillCounterPeriod = PWM2y_ReadKillTime();
        #endif /* (PWM2y_KillModeMinTime) */

        #if(PWM2y_UseControl)
            PWM2y_backup.PWMControlRegister = PWM2y_ReadControlRegister();
        #endif /* (PWM2y_UseControl) */
    #endif  /* (!PWM2y_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWM2y_RestoreConfig
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
*  PWM2y_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWM2y_RestoreConfig(void) 
{
        #if(!PWM2y_UsingFixedFunction)
            #if(!PWM2y_PWMModeIsCenterAligned)
                PWM2y_WritePeriod(PWM2y_backup.PWMPeriod);
            #endif /* (!PWM2y_PWMModeIsCenterAligned) */

            PWM2y_WriteCounter(PWM2y_backup.PWMUdb);

            #if (PWM2y_UseStatus)
                PWM2y_STATUS_MASK = PWM2y_backup.InterruptMaskValue;
            #endif /* (PWM2y_UseStatus) */

            #if(PWM2y_DeadBandMode == PWM2y__B_PWM__DBM_256_CLOCKS || \
                PWM2y_DeadBandMode == PWM2y__B_PWM__DBM_2_4_CLOCKS)
                PWM2y_WriteDeadTime(PWM2y_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(PWM2y_KillModeMinTime)
                PWM2y_WriteKillTime(PWM2y_backup.PWMKillCounterPeriod);
            #endif /* (PWM2y_KillModeMinTime) */

            #if(PWM2y_UseControl)
                PWM2y_WriteControlRegister(PWM2y_backup.PWMControlRegister);
            #endif /* (PWM2y_UseControl) */
        #endif  /* (!PWM2y_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWM2y_Sleep
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
*  PWM2y_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWM2y_Sleep(void) 
{
    #if(PWM2y_UseControl)
        if(PWM2y_CTRL_ENABLE == (PWM2y_CONTROL & PWM2y_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWM2y_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWM2y_backup.PWMEnableState = 0u;
        }
    #endif /* (PWM2y_UseControl) */

    /* Stop component */
    PWM2y_Stop();

    /* Save registers configuration */
    PWM2y_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM2y_Wakeup
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
*  PWM2y_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWM2y_Wakeup(void) 
{
     /* Restore registers values */
    PWM2y_RestoreConfig();

    if(PWM2y_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWM2y_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
