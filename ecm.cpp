#include "ecm.h"

ECM::ECM(const std::shared_ptr<CanBus> can_bus)
{
    m_can_bus = can_bus;
    m_protocol = subaru::can_bus_protocol;
    m_obd_request_header = subaru::ecm::obd_request_header;

    init_obd_pids();
}

void ECM::put(const can::protocol_t protocol, const can::msg_t &message)
{
    if ( (m_protocol != protocol) || (message.second.empty()) ) return;

    can::header_t header = message.first;
    can::data_t data = message.second;
    size_t data_size = static_cast<size_t>( data.at(0) );
    can::data_t response;

    if ( header == obd::header_broadcast || header == get_obd_request_header() ) {
        if ( data_size < 7 ) {
            data.erase( data.begin() );
            data.resize( data_size );
            response = get_obd_pid( data );
        } else {
            // expect more messages to come
        }
    } else {
        // handle non-obd messages
    }

    if ( !( response.empty() ) ) {
        transmit( get_obd_response_header(), response );
    }
}

void ECM::init_obd_pids()
{
    init_obd_mode_01_02_pids();
}

void ECM::init_obd_mode_01_02_pids()
{
    can::data_t pids;

    // PIDs [0..20]
    pids = { static_cast<can::byte_t>(0x41), static_cast<can::byte_t>(0x00) };

    // PIDs [01..08]
    pids.push_back( construct_obd_pid_byte( obd::mode_01_02::pid::pid_01_monitor_status_since_dtc_cleared,
                                            obd::mode_01_02::pid::pid_03_fuel_system_status,
                                            obd::mode_01_02::pid::pid_04_calculated_engine_load,
                                            obd::mode_01_02::pid::pid_05_engine_coolant_temperature,
                                            obd::mode_01_02::pid::pid_06_short_term_fuel_trim_bank_1,
                                            obd::mode_01_02::pid::pid_07_long_term_fuel_trim_bank_1 ));

    // PIDs [09..10]
    pids.push_back( construct_obd_pid_byte( obd::mode_01_02::pid::pid_0B_intake_manifold_absolute_pressure,
                                            obd::mode_01_02::pid::pid_0C_engine_speed,
                                            obd::mode_01_02::pid::pid_0D_vehicle_speed,
                                            obd::mode_01_02::pid::pid_0E_timing_advance,
                                            obd::mode_01_02::pid::pid_0F_intake_air_temperature,
                                            obd::mode_01_02::pid::pid_10_maf_sensor_air_flow_rate ));

    // PIDs [11..18]
    pids.push_back( construct_obd_pid_byte( obd::mode_01_02::pid::pid_11_throttle_position,
                                            obd::mode_01_02::pid::pid_13_oxygen_sensors_present,
                                            obd::mode_01_02::pid::pid_15_oxygen_sensor_2 ));

    // PIDs [19..20]
    pids.push_back( construct_obd_pid_byte( obd::mode_01_02::pid::pid_1C_obd_standards_confirmance,
                                            obd::mode_01_02::pid::pid_1F_run_time_since_engine_start,
                                            obd::mode_01_02::pid::pid_20_pids_supported_21_40 ));


    m_pids[0x0100] = pids;
}

can::byte_t ECM::construct_obd_pid_byte(obd::pid_t pid_1, obd::pid_t pid_2, obd::pid_t pid_3, obd::pid_t pid_4,
                                        obd::pid_t pid_5, obd::pid_t pid_6, obd::pid_t pid_7, obd::pid_t pid_8)
{
    unsigned int pid = ( 1 << ( 7 - ( ( pid_1 - 1 ) & 0x07 ) ) );
    pid |= ( pid_2 ) ? ( 1 << ( 7 - ( ( pid_2 - 1 ) & 0x07 ) ) ) : 0;
    pid |= ( pid_3 ) ? ( 1 << ( 7 - ( ( pid_3 - 1 ) & 0x07 ) ) ) : 0;
    pid |= ( pid_4 ) ? ( 1 << ( 7 - ( ( pid_4 - 1 ) & 0x07 ) ) ) : 0;
    pid |= ( pid_5 ) ? ( 1 << ( 7 - ( ( pid_5 - 1 ) & 0x07 ) ) ) : 0;
    pid |= ( pid_6 ) ? ( 1 << ( 7 - ( ( pid_6 - 1 ) & 0x07 ) ) ) : 0;
    pid |= ( pid_7 ) ? ( 1 << ( 7 - ( ( pid_7 - 1 ) & 0x07 ) ) ) : 0;
    pid |= ( pid_8 ) ? ( 1 << ( 7 - ( ( pid_8 - 1 ) & 0x07 ) ) ) : 0;

    return static_cast<can::byte_t>( pid );
}
