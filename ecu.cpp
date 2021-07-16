#include "ecu.h"

ECU::ECU(const std::shared_ptr<CanBus> can_bus, const can::can_id_t ecu_id, const can::protocol_t protocol) :
    m_can_bus( can_bus )
  , m_ecu_id( ecu_id )
  , m_protocol( protocol )
{
}

void ECU::transmit(const can::can_msg_t &message) const
{
    m_can_bus->transmit( shared_from_this(), m_protocol, message );
}
