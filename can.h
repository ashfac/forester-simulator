#ifndef CAN_H
#define CAN_H

#include <vector>
#include <cstddef>

namespace can {

namespace mode {

enum mode : int {
    disconnected,
    connected,
    listen_only
};

} // namespace mode

namespace protocol {

enum protocol: int {
    iso_15765_4_can_11bit_id_500kbaud,
    iso_15765_4_can_29bit_id_500kbaud,
    iso_15765_4_can_11bit_id_250kbaud,
    iso_15765_4_can_29bit_id_250kbaud,
    sae_j1939_can_29bit_id_250kbaud,
    user1_can_11bit_id_125kbaud,
    user2_can_11bit_id_50kbaud,
    unknown = 0xFF,
};

} // namespace protocol

using header_t = unsigned int;
using byte_t = std::byte;
using data_t = std::vector<byte_t>;
using msg_t = std::pair<header_t, data_t>;
using mode_t = can::mode::mode;
using protocol_t = can::protocol::protocol;

} // namespace can

#endif // CAN_H
