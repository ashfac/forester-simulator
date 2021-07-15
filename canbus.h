#ifndef CANBUS_H
#define CANBUS_H

#include <memory>
#include <vector>
#include <cstddef>

#include "can.h"
#include "ecu.h"

class ECU;

class CanBus
{
private:
    using ecu_ptr_t = std::shared_ptr<const ECU>;

public:
    CanBus();

    void attach(const ecu_ptr_t &ecu);
    void detach(const ecu_ptr_t &ecu);

    void transmit(const ecu_ptr_t &sender, const can::can_msg_t &message) const;

private:
    std::vector<ecu_ptr_t> m_ecus;
    can::protocol_t m_protocol;
};

#endif // CANBUS_H
