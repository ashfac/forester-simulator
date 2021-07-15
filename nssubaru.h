#ifndef NSSUBARU_H
#define NSSUBARU_H

#include "can_types.h"

namespace nssubaru {
constexpr can_types::can_bus_protocol_t CAN_BUS_PROTOCOL = can_types::can_bus_protocol::iso_15765_4_can_11bit_id_500kbaud;
constexpr can_types::can_id_t CAN_ID_ECM = 0x7E0;
constexpr can_types::can_id_t CAN_ID_TCM = 0x7E1;

constexpr can_types::can_id_t CAN_ID_BROADCAST = 0x7DF;

}

#endif // NSSUBARU_H
