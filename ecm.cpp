#include "ecm.h"

ECM::ECM(const std::shared_ptr<CanBus> can_bus, const can::can_id_t ecu_id, const can::protocol_t protocol) :
    ECU(can_bus, ecu_id, protocol)
{
}

void ECM::put(const can::protocol_t protocol, can::can_msg_t message)
{
    if ( (get_protocol() != protocol) || (message.second.empty()) ) return;

    can::can_id_t message_id = message.first;
    can::can_data_t &data = message.second;
    size_t data_size = static_cast<size_t>( data.at(0) );

    if ( message_id == obd::id::broadcast || message_id == get_ecu_id() ) {

        data.resize( data_size );
        data.erase( data.begin() );
        handle_obd_request( data );

    } else {
        // handle non-obd messages
    }
}

void ECM::handle_obd_request(can::can_data_t &data)
{
    if ( data.size() < obd::request::min_size ) return;

    can::can_byte_t mode = data.at( 0 );
    data.erase(data.begin());

    switch ( mode ) {

    case obd::mode::mode_01_live_data: {
        handle_mode_01_live_data( data );
        break;
    }

    case obd::mode::mode_02_freeze_frame: {
        handle_mode_02_freeze_frame( data );
        break;
    }

    case obd::mode::mode_03_get_stored_trouble_codes: {
        handle_mode_03_get_stored_trouble_codes( data );
        break;
    }

    case obd::mode::mode_04_clear_stored_trouble_codes: {
        handle_mode_04_clear_stored_trouble_codes( data );
        break;
    }

    case obd::mode::mode_05_oxygen_sensors_test_results: {
        handle_mode_05_oxygen_sensors_test_results( data );
        break;
    }

    case obd::mode::mode_06_onboard_systems_test_results: {
        handle_mode_06_onboard_systems_test_results( data );
        break;
    }

    case obd::mode::mode_07_get_pending_trouble_codes: {
        handle_mode_08_control_onboard_systems( data );
        break;
    }

    case obd::mode::mode_08_control_onboard_systems: {
        handle_mode_08_control_onboard_systems( data );
        break;
    }

    case obd::mode::mode_09_vehicle_information: {
        handle_mode_09_vehicle_information( data );
        break;
    }

    case obd::mode::mode_0A_get_permanent_trouble_codes: {
        handle_mode_0A_get_permanent_trouble_codes( data );
        break;
    }

    default:
        break;
    }
}

void ECM::handle_mode_01_live_data(can::can_data_t &data)
{

}

void ECM::handle_mode_02_freeze_frame(can::can_data_t &data)
{

}

void ECM::handle_mode_03_get_stored_trouble_codes(can::can_data_t &data)
{

}

void ECM::handle_mode_04_clear_stored_trouble_codes(can::can_data_t &data)
{

}

void ECM::handle_mode_05_oxygen_sensors_test_results(can::can_data_t &data)
{

}

void ECM::handle_mode_06_onboard_systems_test_results(can::can_data_t &data)
{

}

void ECM::handle_mode_07_get_pending_trouble_codes(can::can_data_t &data)
{

}

void ECM::handle_mode_08_control_onboard_systems(can::can_data_t &data)
{

}

void ECM::handle_mode_09_vehicle_information(can::can_data_t &data)
{

}

void ECM::handle_mode_0A_get_permanent_trouble_codes(can::can_data_t &data)
{

}
