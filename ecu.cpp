#include "ecu.h"

ECU::ECU()
{
}

void ECU::transmit(const can::header_t header, const can::data_t &data) const
{
    can::data_t msg_data (8);
    can::data_t::const_iterator begin = data.begin();
    size_t remaining = data.end() - begin;

    if ( remaining > 7 ) {
        // handle extended length message
    }

    while ( remaining > 0 ) {
        size_t len = ( remaining < 7) ? remaining : 7;

        msg_data[0] = static_cast<can::byte_t>( len );
        std::copy( begin, begin + len, msg_data.begin() + 1 );
        std::fill( msg_data.begin() + len + 1, msg_data.end(), static_cast<can::byte_t>( 0 ) );

        m_can_bus->transmit( shared_from_this(), m_protocol, std::make_pair(header, msg_data));

        begin += len;
        remaining = data.end() - begin;
    }
}

can::data_t ECU::get_obd_pid(can::data_t &request) const
{
    can::data_t response = can::data_t();

    if ( request.size() ) {
        obd::mode_t mode = get_obd_mode(request);

        request.erase(request.begin());

        if ( request.size() == get_obd_pid_request_size(mode) ) {
            unsigned int key = mode;

            for ( auto b : request ) {
                key <<= 8;
                key |= static_cast<unsigned int>( b );
            }

            try {
                response = m_pids.at(key);
            } catch ( const std::out_of_range& oor) { }
        }
    }

    return response;
}

obd::mode_t ECU::get_obd_mode(const can::data_t &request)
{
    return ( (request.size()) ? static_cast<obd::mode_t>(request.at(0)) : obd::mode::mode_unknown);
}

size_t ECU::get_obd_pid_request_size(const obd::mode_t mode)
{
    switch (mode) {
    case obd::mode::mode_01_live_data: return obd::mode_01_requet_size;
    case obd::mode::mode_02_freeze_frame: return obd::mode_02_requet_size;
    case obd::mode::mode_03_get_stored_trouble_codes: return obd::mode_03_requet_size;
    case obd::mode::mode_04_clear_stored_trouble_codes: return obd::mode_04_requet_size;
    case obd::mode::mode_05_oxygen_sensors_test_results: return obd::mode_05_requet_size;
    case obd::mode::mode_06_onboard_systems_test_results: return obd::mode_06_requet_size;
    case obd::mode::mode_07_get_pending_trouble_codes: return obd::mode_07_requet_size;
    case obd::mode::mode_08_control_onboard_systems: return obd::mode_08_requet_size;
    case obd::mode::mode_09_vehicle_information:  return obd::mode_09_requet_size;
    case obd::mode::mode_0A_get_permanent_trouble_codes: return obd::mode_0A_requet_size;
    default: return 0;
    }
}
