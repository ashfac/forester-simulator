#include "tcm.h"

TCM::TCM(const std::shared_ptr<CanBus> can_bus)
{
    m_can_bus = can_bus;
    m_protocol = subaru::can_bus_protocol;
    m_obd_request_header = subaru::tcm::obd_request_header;
}

void TCM::put(const can::protocol_t protocol, const can::msg_t &message)
{
    if ( m_protocol != protocol ) return;

    (void) message;
}
