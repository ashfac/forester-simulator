#include "tcm.h"

TCM::TCM(const can_types::ecu_id_t ecu_id, const std::shared_ptr<CanBus> can_bus) :
    ECU(ecu_id, can_bus)
{
}
