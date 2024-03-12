/*******************************************************************************
 * File: get.c
 * Created: 2021-10-06
 * 
 * Authors: 
 *   Tyler Matijevich
 * 
 * License:
 *   This file get.c is part of the UserLog project 
 *   released under the MIT license agreement.
 ******************************************************************************/

#include "Main.h"

/* Get ArEventLog severity from an event ID */
uint8_t UserLogGetSeverity(int32_t Event)
{
    /* Return bit 31-30. See 32-bit event ID in ArEventLog help */
    return (uint8_t)(Event >> 30 & 0x3);
}

/* Get ArEventLog facility from an event ID */
uint16_t UserLogGetFacility(int32_t Event)
{
    /* Return bit 27-16. See 32-bit event ID in ArEventLog help */
    return (uint16_t)(Event >> 16 & 0xFFF);
}

/* Get ArEventLog code from an event ID */
uint16_t UserLogGetCode(int32_t Event)
{
    /* Return bit 15-0. See 32-bit event ID in ArEventLog help */
    return (uint16_t)(Event & 0xFFFF);
}
