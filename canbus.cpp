#include <algorithm>

#include "canbus.h"

CanBus::CanBus()
{
}

void CanBus::attach(const ecu_ptr_t &ecu)
{
    // first ecu determines the CAN bus protocol
    if ( m_ecus.size() == 0 ) {
        m_protocol = ecu->get_can_bus_protocol();
    }

    m_ecus.push_back(ecu);
}

void CanBus::detach(const ecu_ptr_t &ecu)
{
    std::vector<ecu_ptr_t>::iterator it = std::find(m_ecus.begin(), m_ecus.end(), ecu);

    if ( it != m_ecus.end() ) {
        m_ecus.erase(it);
    }
}

void CanBus::transmit(const ecu_ptr_t &sender, const can::can_msg_t &message) const
{
    for (auto& ecu : m_ecus) {
        if (ecu != sender) {
            ecu->transmit(message);
        }
    }
}
