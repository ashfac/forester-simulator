#include "ecm.h"

ECM::ECM(const std::shared_ptr<CanBus> can_bus)
{
    m_can_bus = can_bus;
    m_protocol = subaru::can_bus_protocol;
    m_obd_request_header = subaru::ecm::obd_request_header;

    m_pids[0x0100] = can::data_t { 0x00, 0x88, 0x1F, 0x00, 0x00, 0x00 };
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

