#ifndef SUBARU_H
#define SUBARU_H

#include "can.h"

namespace subaru {
constexpr can::protocol_t CAN_PROTOCOL = can::protocol::iso_15765_4_can_11bit_id_500kbaud;
constexpr can::can_id_t CAN_ID_ECM = 0x7E0;
constexpr can::can_id_t CAN_ID_TCM = 0x7E1;

constexpr can::can_id_t CAN_ID_BROADCAST = 0x7DF;

}

#endif // SUBARU_H
