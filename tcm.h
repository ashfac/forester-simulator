#ifndef TCM_H
#define TCM_H

#include "ecu.h"
#include "subaru.h"

class TCM : public ECU
{
public:
    TCM(const std::shared_ptr<CanBus> can_bus);

    void put(const can::protocol_t protocol, const can::msg_t &message);
};

#endif // TCM_H
