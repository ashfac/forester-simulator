#include "ecu.h"

ECU::ECU(const can_types::ecu_id_t ecu_id, const std::shared_ptr<CanBus> can_bus) :
    m_ecu_id(ecu_id)
  , m_can_bus(can_bus)
{
}

void ECU::transmit(const can_types::can_msg_t &message) const
{
    (void) message;
}
