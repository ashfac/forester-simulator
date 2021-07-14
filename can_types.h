#ifndef CAN_TYPES_H
#define CAN_TYPES_H

#include <vector>
#include <cstddef>

namespace can_types {

    typedef enum : unsigned int {
        ecu_id_dlc = 0x7DF,
        ecu_id_ecm = 0x7E0,
        ecu_id_tcm = 0x7E1
    } ecu_id_t;

using can_data_t = std::vector<std::byte>;

using can_msg_t = std::pair<ecu_id_t, can_data_t>;

};

#endif // CAN_TYPES_H
