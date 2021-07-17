#include <algorithm>

#include "canbus.h"

CanBus::CanBus()
{
}

void CanBus::attach(const std::shared_ptr<ECU> &ecu)
{
    m_ecus.push_back(ecu);
}

void CanBus::detach(const std::shared_ptr<ECU> &ecu)
{
    auto it = std::find(m_ecus.begin(), m_ecus.end(), ecu);

    if ( it != m_ecus.end() ) {
        m_ecus.erase(it);
    }
}

void CanBus::transmit(const std::shared_ptr<const ECU> &sender, const can::protocol_t protocol, const can::msg_t &message) const
{
    for ( auto &ecu : m_ecus ) {
        if ( ecu != sender ) {
            ecu->put(protocol, message);
        }
    }
}
