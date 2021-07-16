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

public:
    CanBus();

    void attach(const std::shared_ptr<const ECU> &ecu);
    void detach(const std::shared_ptr<const ECU> &ecu);

    void transmit(const std::shared_ptr<const ECU> &sender, const can::protocol_t protocol, const can::can_msg_t &message) const;

private:
    std::vector<std::shared_ptr<const ECU>> m_ecus;
};

#endif // CANBUS_H
