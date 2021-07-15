#ifndef CAN_TYPES_H
#define CAN_TYPES_H

#include <vector>
#include <cstddef>

namespace can_types {

typedef enum can_bus_protocol: int {
    iso_15765_4_can_11bit_id_500kbaud,
    iso_15765_4_can_29bit_id_500kbaud,
    iso_15765_4_can_11bit_id_250kbaud,
    iso_15765_4_can_29bit_id_250kbaud,
    sae_j1939_can_29bit_id_250kbaud,
    user1_can_11bit_id_125kbaud,
    user2_can_11bit_id_50kbaud,
    protocol_unknown = 0xFF,
} can_bus_protocol_t;

using can_id_t = unsigned int;
using can_data_t = std::vector<std::byte>;

using can_msg_t = std::pair<can_id_t, can_data_t>;

};

#endif // CAN_TYPES_H
