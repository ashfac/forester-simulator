#ifndef OBD_H
#define OBD_H

#include "can.h"

namespace obd {

namespace mode {

enum mode {
    mode_unknown                         = 0x00,
    mode_01_live_data                    = 0x01,
    mode_02_freeze_frame                 = 0x02,
    mode_03_get_stored_trouble_codes     = 0x03,
    mode_04_clear_stored_trouble_codes   = 0x04,
    mode_05_oxygen_sensors_test_results  = 0x05,
    mode_06_onboard_systems_test_results = 0x06,
    mode_07_get_pending_trouble_codes    = 0x07,
    mode_08_control_onboard_systems      = 0x08,
    mode_09_vehicle_information          = 0x09,
    mode_0A_get_permanent_trouble_codes  = 0x0A
};

} // namespace mode

namespace mode_01_02 {

namespace pid {

enum pid : int {
    pid_00_get_supported_pids = 0x00

};

} // namespace pid

} // namespace mode_01_02

namespace mode_05 {

namespace pid {

enum pid : int {

};

} // namespace pid

} // namespace mode_05

using mode_t = mode::mode;
using pid_t = unsigned int;
using pid_data_t = can::data_t;

constexpr can::header_t header_invalid       = 0x000;
constexpr can::header_t header_broadcast     = 0x7DF;
constexpr can::header_t response_header_mask = 0x008;
constexpr can::byte_t response_positive_mask = static_cast<can::byte_t>( 0x40 );

constexpr size_t mode_01_requet_size = 1;
constexpr size_t mode_02_requet_size = 1;
constexpr size_t mode_03_requet_size = 0;
constexpr size_t mode_04_requet_size = 0;
constexpr size_t mode_05_requet_size = 2;
constexpr size_t mode_06_requet_size = 1;
constexpr size_t mode_07_requet_size = 1;
constexpr size_t mode_08_requet_size = 1;
constexpr size_t mode_09_requet_size = 1;
constexpr size_t mode_0A_requet_size = 1;

} // namespace obd

#endif // OBD_H
