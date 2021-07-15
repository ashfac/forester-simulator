#ifndef TCM_H
#define TCM_H

#include "ecu.h"

class TCM : public ECU
{
public:
    TCM(const can::can_id_t can_id, const std::shared_ptr<CanBus> can_bus);
};

#endif // TCM_H
