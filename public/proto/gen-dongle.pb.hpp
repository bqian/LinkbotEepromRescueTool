#ifndef barobo_Dongle_INTERFACE
#define barobo_Dongle_INTERFACE

#include "rpc/def.hpp"
#include "dongle.pb.h"

RPCDEF_HPP(
        // the interface we're defining and it's version triplet
        (barobo, Dongle), (0, 1, 2),
        // all methods
        (getFirmwareVersion)
        (getFirmwareVersionString)
        (reboot)
        (setRadioMode)
        (setComputerId)
        (transmitUnicast)
        (transmitRadioBroadcast)
        ,
        // all broadcasts
        (receiveTransmission)
        )

#endif
