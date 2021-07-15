#ifndef NSOBDLINK_H
#define NSOBDLINK_H

#include <string>

#include "can_types.h"

namespace nsobdlink {

namespace versions {
const std::string elm_device_id = "ELM327 v1.4b";

}

namespace response {
const std::string ok = "OK";
const std::string invalid = "?";
const std::string pending = "!pending!";
}

namespace config {
typedef enum boolean_config : int {
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
} boolean_config_t;

typedef enum adaptive_timing : int {
    adaptive_timing_off = 0,
    adaptive_timing_auto_1 = 1,
    adaptive_timing_auto_2 = 2
} adaptive_timing_t;

typedef enum obd_protocol : int {
    obd_protocol_0_automatic = 0x00,
    obd_protocol_1_sae_j1850_pwm_41_6kbaud = 0x01,
    obd_protocol_2_sae_j1850_vpw_10_4kbaud = 0x02,
    obd_protocol_3_iso_9141_2_5baud_init_10_4kbaud = 0x03,
    obd_protocol_4_iso_14230_kwp_5baud_init_10_4kbaud = 0x04,
    obd_protocol_5_iso_14230_kwp_fast_init_10_4kbaud = 0x05,
    obd_protocol_6_iso_15765_4_can_11bit_id_500kbaud = 0x06,
    obd_protocol_7_iso_15765_4_can_29bit_id_500kbaud = 0x07,
    obd_protocol_8_iso_15765_4_can_11bit_id_250kbaud = 0x08,
    obd_protocol_9_iso_15765_4_can_29bit_id_250kbaud = 0x09,
    obd_protocol_A_sae_j1939_can_29bit_id_250kbaud = 0x0A,
    obd_protocol_B_user1_can_11bit_id_125kbaud = 0x0B,
    obd_protocol_C_user2_can_11bit_id_50kbaud = 0x0C,
    obd_protocol_unknown = 0xFF
} obd_protocol_t;

constexpr obd_protocol_t OBD_PROTOCOL_MIN = obd_protocol::obd_protocol_0_automatic;
constexpr obd_protocol_t OBD_PROTOCOL_MAX = obd_protocol::obd_protocol_C_user2_can_11bit_id_50kbaud;

namespace defaults {
constexpr adaptive_timing_t ADAPTIVE_TIMING = adaptive_timing::adaptive_timing_auto_1;
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
constexpr obd_protocol_t OBD_PROTOCOL = obd_protocol::obd_protocol_0_automatic;

constexpr can_types::can_id_t OBD_HEADER_BROADCAST = 0x7DF;
}

}


}

#endif // NSOBDLINK_H
