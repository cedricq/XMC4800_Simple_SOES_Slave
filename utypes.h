/*
 * Licensed under the GNU General Public License version 2 with exceptions. See
 * LICENSE file in the project root for full license information
 */

 /** \file
 * \brief
 * Header file for Application types.
 *
 * Part of the application, host declarations of application types mapped against
 * ServiceData Objects and ProcessData Objects.
 */

#ifndef __utypes_h__
#define __utypes_h__

#include <cc.h>

CC_PACKED_BEGIN
typedef struct
{
   // current IMU data
   int16_t accelerometerX0; // raw data
   int16_t accelerometerY0;
   int16_t accelerometerZ0;

   int16_t gyroscopeX0; // raw data.
   int16_t gyroscopeY0;
   int16_t gyroscopeZ0;

   int16_t temperature0; // Celsius degrees
   uint16_t timestamp0; //deca µs (cyclic timestamp returns to 0 every 65535 *10 µs)

   // previous IMU data
   int16_t accelerometerX1;
   int16_t accelerometerY1;
   int16_t accelerometerZ1;

   int16_t gyroscopeX1;
   int16_t gyroscopeY1;
   int16_t gyroscopeZ1;

   int16_t temperature1;
   uint16_t timestamp1;
}CC_PACKED imu;
CC_PACKED_END


CC_PACKED_BEGIN
typedef struct
{
   uint16_t watchdogCounter;

   imu ankleIMU;
   imu soleIMU;
   imu tibiaIMU;
   imu thighIMU;

   uint32_t forceSensor0;
   uint32_t forceSensor1;
   uint32_t forceSensor2;
   uint32_t forceSensor3;
   uint16_t forceSensorTimestamp;

   uint16_t boardStatus;
}CC_PACKED _Rbuffer;
CC_PACKED_END

CC_PACKED_BEGIN
typedef struct
{
   uint16_t watchdogCounter;
}CC_PACKED _Wbuffer;
CC_PACKED_END

CC_PACKED_BEGIN
typedef struct
{
   uint32_t reset_counter;
}CC_PACKED _Cbuffer;
CC_PACKED_END

#endif
