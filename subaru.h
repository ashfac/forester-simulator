#ifndef SUBARU_H
#define SUBARU_H

#include "can.h"

namespace subaru {

namespace ecu {

constexpr can::protocol_t protocol = can::protocol::iso_15765_4_can_11bit_id_500kbaud;

namespace id {

constexpr can::can_id_t dlc = 0x000;
constexpr can::can_id_t ecm = 0x7E0;
constexpr can::can_id_t tcm = 0x7E1;

} // namespace id

} // namespace ecu

} // namespace subaru

#endif // SUBARU_H
