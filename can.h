#ifndef CAN_H
#define CAN_H

#include <vector>
#include <cstddef>

namespace can {

using can_id_t = unsigned int;
using can_data_t = std::vector<std::byte>;
using can_msg_t = std::pair<can_id_t, can_data_t>;

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

using protocol_t = can::protocol::protocol;

} // namespace can

#endif // CAN_H
