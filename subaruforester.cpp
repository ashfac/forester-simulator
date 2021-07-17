
#include "subaruforester.h"

SubaruForester::SubaruForester() :
    m_can_bus(std::make_shared<CanBus>())
  , m_dlc (std::make_shared<DLC>( m_can_bus ))
  , m_ecm (std::make_shared<ECM>( m_can_bus ))
  , m_tcm (std::make_shared<TCM>( m_can_bus ))
{
    m_can_bus->attach( m_dlc );
    m_can_bus->attach( m_ecm );
    m_can_bus->attach( m_tcm );
}
