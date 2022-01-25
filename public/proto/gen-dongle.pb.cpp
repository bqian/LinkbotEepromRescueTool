#include "gen-dongle.pb.hpp"
#include "rpc/def.hpp"

RPCDEF_CPP((barobo, Dongle),
		(getFirmwareVersion)
		(getFirmwareVersionString)
		(reboot)
        (setRadioMode)
        (setComputerId)
        (transmitUnicast)
        (transmitRadioBroadcast)
        ,
        (receiveTransmission)
        )
