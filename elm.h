#ifndef ELM_H
#define ELM_H

#include <string>

#include "can.h"

namespace elm {

const std::string device_id = "ELM327 v1.4b";

namespace response {

const std::string ok = "OK";
const std::string invalid = "?";
const std::string pending = "!pending!";

} // namespace response

namespace obd {

namespace protocol {

enum protocol : int {
    automatic = 0x00,
    sae_j1850_pwm_41_6kbaud = 0x01,
    sae_j1850_vpw_10_4kbaud = 0x02,
    iso_9141_2_5baud_init_10_4kbaud = 0x03,
    iso_14230_kwp_5baud_init_10_4kbaud = 0x04,
    iso_14230_kwp_fast_init_10_4kbaud = 0x05,
    iso_15765_4_can_11bit_id_500kbaud = 0x06,
    iso_15765_4_can_29bit_id_500kbaud = 0x07,
    iso_15765_4_can_11bit_id_250kbaud = 0x08,
    iso_15765_4_can_29bit_id_250kbaud = 0x09,
    sae_j1939_can_29bit_id_250kbaud = 0x0A,
    user1_can_11bit_id_125kbaud = 0x0B,
    user2_can_11bit_id_50kbaud = 0x0C,
    unknown = 0xFF
};

constexpr protocol MIN = protocol::automatic;
constexpr protocol MAX = protocol::user2_can_11bit_id_50kbaud;

} // namespace protocol

using protocol_t = elm::obd::protocol::protocol;
constexpr can::can_id_t BROADCAST_HEADER_STANDARD = 0x7DF;

} // namespace obd

namespace config {

typedef enum config : int {
    can_auto_formatting,
    can_flow_control,
    can_silent_mode,
    dlc,
    echo,
    headers,
    j1939_header_formatting,
    keyword_checking,
    linefeeds,
    memory,
    responses,
    spaces,
    variable_dlc
} config_t;

namespace adaptive_timing {

enum adaptive_timing : int {
    off = 0,
    auto_1 = 1,
    auto_2 = 2
};

} // namespace adaptive_timing

using adaptive_timing_t = elm::config::adaptive_timing::adaptive_timing;

namespace defaults {
constexpr bool CAN_AUTO_FORMATTING = true;
constexpr bool CAN_FLOW_CONTROL = true;
constexpr bool CAN_SILENT_MODE = true;
constexpr bool DLC = false;
constexpr bool ECHO = true;
constexpr bool HEADERS = true;
constexpr bool J1939_HEADER_FORMATTING = true;
constexpr bool KEYWORD_CHECKING = true;
constexpr bool LINEFEEDS = false;
constexpr bool MEMORY = true;
constexpr bool RESPONSES = true;
constexpr bool SPACES = true;
constexpr bool VARIABLE_DLC = true;

constexpr adaptive_timing_t ADAPTIVE_TIMING = adaptive_timing::auto_1;
constexpr elm::obd::protocol_t OBD_PROTOCOL = elm::obd::protocol::unknown;

}


} // namespace config

} // namespace elm

#endif // ELM_H
