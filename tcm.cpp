#include "tcm.h"

TCM::TCM(const std::shared_ptr<CanBus> can_bus, const can::can_id_t ecu_id, const can::protocol_t protocol) :
    ECU(can_bus, ecu_id, protocol)
{
}

void TCM::put(const can::protocol_t protocol, can::can_msg_t message)
{
    if ( get_protocol() != protocol ) return;
}
