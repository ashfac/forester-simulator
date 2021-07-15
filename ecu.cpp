#include "ecu.h"

ECU::ECU(const can::can_id_t can_id, const std::shared_ptr<CanBus> can_bus) :
    m_can_id(can_id)
  , m_can_bus(can_bus)
{
}

void ECU::transmit(const can::can_msg_t &message) const
{
    get_can_bus()->attach(shared_from_this());

    (void) message;
}
