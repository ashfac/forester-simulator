#ifndef ECM_H
#define ECM_H

#include "ecu.h"
#include "subaru.h"

class ECM : public ECU
{
public:
    ECM(const std::shared_ptr<CanBus> can_bus);

    void put(const can::protocol_t protocol, const can::msg_t &message);

};

#endif // ECM_H
