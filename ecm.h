#ifndef ECM_H
#define ECM_H

#include "ecu.h"

class ECM : public ECU
{
public:
    ECM(const can_types::ecu_id_t ecu_id, const std::shared_ptr<CanBus> can_bus);
};

#endif // ECM_H
