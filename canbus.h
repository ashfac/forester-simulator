#ifndef CANBUS_H
#define CANBUS_H

#include <memory>
#include <vector>
#include <cstddef>

#include "can_types.h"
#include "ecu.h"

class ECU;

class CanBus
{
private:
    using ecu_ptr_t = std::shared_ptr<ECU>;

public:
    CanBus();

    void attach(const ecu_ptr_t);

    void transmit(const can_types::can_msg_t &message) const;

private:
    std::vector<ecu_ptr_t> m_ecus;
};

#endif // CANBUS_H
