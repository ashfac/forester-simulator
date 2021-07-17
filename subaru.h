#ifndef SUBARU_H
#define SUBARU_H

#include "can.h"
#include "obd.h"

namespace subaru {

namespace ecm {

constexpr can::header_t obd_request_header  = 0x7E0;

} // namespace ecm

namespace tcm {

constexpr can::header_t obd_request_header  = 0x7E1;

} // namespace tcm

constexpr can::protocol_t can_bus_protocol = can::protocol::iso_15765_4_can_11bit_id_500kbaud;

} // namespace subaru

#endif // SUBARU_H
