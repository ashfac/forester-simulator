#ifndef TCM_H
#define TCM_H

#include "ecu.h"

class TCM : public ECU
{
public:
    TCM(const std::shared_ptr<CanBus> can_bus, const can::can_id_t ecu_id, const can::protocol_t protocol);

    void put(const can::protocol_t protocol, can::can_msg_t message);
};

#endif // TCM_H
