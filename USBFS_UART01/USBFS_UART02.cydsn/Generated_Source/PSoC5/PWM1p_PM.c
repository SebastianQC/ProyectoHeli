/*******************************************************************************
* File Name: PWM1p_PM.c
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

#include "PWM1p.h"

static PWM1p_backupStruct PWM1p_backup;


/*******************************************************************************
* Function Name: PWM1p_SaveConfig
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
*  PWM1p_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void PWM1p_SaveConfig(void) 
{

    #if(!PWM1p_UsingFixedFunction)
        #if(!PWM1p_PWMModeIsCenterAligned)
            PWM1p_backup.PWMPeriod = PWM1p_ReadPeriod();
        #endif /* (!PWM1p_PWMModeIsCenterAligned) */
        PWM1p_backup.PWMUdb = PWM1p_ReadCounter();
        #if (PWM1p_UseStatus)
            PWM1p_backup.InterruptMaskValue = PWM1p_STATUS_MASK;
        #endif /* (PWM1p_UseStatus) */

        #if(PWM1p_DeadBandMode == PWM1p__B_PWM__DBM_256_CLOCKS || \
            PWM1p_DeadBandMode == PWM1p__B_PWM__DBM_2_4_CLOCKS)
            PWM1p_backup.PWMdeadBandValue = PWM1p_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(PWM1p_KillModeMinTime)
             PWM1p_backup.PWMKillCounterPeriod = PWM1p_ReadKillTime();
        #endif /* (PWM1p_KillModeMinTime) */

        #if(PWM1p_UseControl)
            PWM1p_backup.PWMControlRegister = PWM1p_ReadControlRegister();
        #endif /* (PWM1p_UseControl) */
    #endif  /* (!PWM1p_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWM1p_RestoreConfig
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
*  PWM1p_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWM1p_RestoreConfig(void) 
{
        #if(!PWM1p_UsingFixedFunction)
            #if(!PWM1p_PWMModeIsCenterAligned)
                PWM1p_WritePeriod(PWM1p_backup.PWMPeriod);
            #endif /* (!PWM1p_PWMModeIsCenterAligned) */

            PWM1p_WriteCounter(PWM1p_backup.PWMUdb);

            #if (PWM1p_UseStatus)
                PWM1p_STATUS_MASK = PWM1p_backup.InterruptMaskValue;
            #endif /* (PWM1p_UseStatus) */

            #if(PWM1p_DeadBandMode == PWM1p__B_PWM__DBM_256_CLOCKS || \
                PWM1p_DeadBandMode == PWM1p__B_PWM__DBM_2_4_CLOCKS)
                PWM1p_WriteDeadTime(PWM1p_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(PWM1p_KillModeMinTime)
                PWM1p_WriteKillTime(PWM1p_backup.PWMKillCounterPeriod);
            #endif /* (PWM1p_KillModeMinTime) */

            #if(PWM1p_UseControl)
                PWM1p_WriteControlRegister(PWM1p_backup.PWMControlRegister);
            #endif /* (PWM1p_UseControl) */
        #endif  /* (!PWM1p_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWM1p_Sleep
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
*  PWM1p_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWM1p_Sleep(void) 
{
    #if(PWM1p_UseControl)
        if(PWM1p_CTRL_ENABLE == (PWM1p_CONTROL & PWM1p_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWM1p_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWM1p_backup.PWMEnableState = 0u;
        }
    #endif /* (PWM1p_UseControl) */

    /* Stop component */
    PWM1p_Stop();

    /* Save registers configuration */
    PWM1p_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM1p_Wakeup
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
*  PWM1p_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWM1p_Wakeup(void) 
{
     /* Restore registers values */
    PWM1p_RestoreConfig();

    if(PWM1p_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWM1p_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
