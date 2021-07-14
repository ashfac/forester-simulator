#include "canbus.h"

CanBus::CanBus()
{
}

void CanBus::attach(const ecu_ptr_t ecu)
{
    m_ecus.push_back(ecu);
}

void CanBus::transmit(const can_types::can_msg_t &message) const
{
    for (auto ecu : m_ecus) {
        ecu->transmit(message);
    }
}
