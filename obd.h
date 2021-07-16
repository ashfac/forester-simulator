#ifndef OBD_H
#define OBD_H

#include "can.h"

namespace obd {

namespace request {

constexpr size_t min_size = 2;

} // namespace request

namespace id {

constexpr can::can_id_t broadcast = 0x7DF;

} // namespace id

namespace mode {

constexpr can::can_byte_t mode_01_live_data                    = static_cast<can::can_byte_t>( 0x01 );
constexpr can::can_byte_t mode_02_freeze_frame                 = static_cast<can::can_byte_t>( 0x02 );
constexpr can::can_byte_t mode_03_get_stored_trouble_codes     = static_cast<can::can_byte_t>( 0x03 );
constexpr can::can_byte_t mode_04_clear_stored_trouble_codes   = static_cast<can::can_byte_t>( 0x04 );
constexpr can::can_byte_t mode_05_oxygen_sensors_test_results  = static_cast<can::can_byte_t>( 0x05 );
constexpr can::can_byte_t mode_06_onboard_systems_test_results = static_cast<can::can_byte_t>( 0x06 );
constexpr can::can_byte_t mode_07_get_pending_trouble_codes    = static_cast<can::can_byte_t>( 0x07 );
constexpr can::can_byte_t mode_08_control_onboard_systems      = static_cast<can::can_byte_t>( 0x08 );
constexpr can::can_byte_t mode_09_vehicle_information          = static_cast<can::can_byte_t>( 0x09 );
constexpr can::can_byte_t mode_0A_get_permanent_trouble_codes  = static_cast<can::can_byte_t>( 0x0A );

} // namespace mode

} // namespace obd

#endif // OBD_H
