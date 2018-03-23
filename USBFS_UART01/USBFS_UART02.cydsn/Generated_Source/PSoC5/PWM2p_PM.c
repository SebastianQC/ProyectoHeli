/*******************************************************************************
* File Name: PWM2p_PM.c
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

#include "PWM2p.h"

static PWM2p_backupStruct PWM2p_backup;


/*******************************************************************************
* Function Name: PWM2p_SaveConfig
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
*  PWM2p_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void PWM2p_SaveConfig(void) 
{

    #if(!PWM2p_UsingFixedFunction)
        #if(!PWM2p_PWMModeIsCenterAligned)
            PWM2p_backup.PWMPeriod = PWM2p_ReadPeriod();
        #endif /* (!PWM2p_PWMModeIsCenterAligned) */
        PWM2p_backup.PWMUdb = PWM2p_ReadCounter();
        #if (PWM2p_UseStatus)
            PWM2p_backup.InterruptMaskValue = PWM2p_STATUS_MASK;
        #endif /* (PWM2p_UseStatus) */

        #if(PWM2p_DeadBandMode == PWM2p__B_PWM__DBM_256_CLOCKS || \
            PWM2p_DeadBandMode == PWM2p__B_PWM__DBM_2_4_CLOCKS)
            PWM2p_backup.PWMdeadBandValue = PWM2p_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(PWM2p_KillModeMinTime)
             PWM2p_backup.PWMKillCounterPeriod = PWM2p_ReadKillTime();
        #endif /* (PWM2p_KillModeMinTime) */

        #if(PWM2p_UseControl)
            PWM2p_backup.PWMControlRegister = PWM2p_ReadControlRegister();
        #endif /* (PWM2p_UseControl) */
    #endif  /* (!PWM2p_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWM2p_RestoreConfig
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
*  PWM2p_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWM2p_RestoreConfig(void) 
{
        #if(!PWM2p_UsingFixedFunction)
            #if(!PWM2p_PWMModeIsCenterAligned)
                PWM2p_WritePeriod(PWM2p_backup.PWMPeriod);
            #endif /* (!PWM2p_PWMModeIsCenterAligned) */

            PWM2p_WriteCounter(PWM2p_backup.PWMUdb);

            #if (PWM2p_UseStatus)
                PWM2p_STATUS_MASK = PWM2p_backup.InterruptMaskValue;
            #endif /* (PWM2p_UseStatus) */

            #if(PWM2p_DeadBandMode == PWM2p__B_PWM__DBM_256_CLOCKS || \
                PWM2p_DeadBandMode == PWM2p__B_PWM__DBM_2_4_CLOCKS)
                PWM2p_WriteDeadTime(PWM2p_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(PWM2p_KillModeMinTime)
                PWM2p_WriteKillTime(PWM2p_backup.PWMKillCounterPeriod);
            #endif /* (PWM2p_KillModeMinTime) */

            #if(PWM2p_UseControl)
                PWM2p_WriteControlRegister(PWM2p_backup.PWMControlRegister);
            #endif /* (PWM2p_UseControl) */
        #endif  /* (!PWM2p_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWM2p_Sleep
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
*  PWM2p_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWM2p_Sleep(void) 
{
    #if(PWM2p_UseControl)
        if(PWM2p_CTRL_ENABLE == (PWM2p_CONTROL & PWM2p_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWM2p_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWM2p_backup.PWMEnableState = 0u;
        }
    #endif /* (PWM2p_UseControl) */

    /* Stop component */
    PWM2p_Stop();

    /* Save registers configuration */
    PWM2p_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM2p_Wakeup
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
*  PWM2p_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWM2p_Wakeup(void) 
{
     /* Restore registers values */
    PWM2p_RestoreConfig();

    if(PWM2p_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWM2p_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
