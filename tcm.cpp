#include "tcm.h"

TCM::TCM(const can_types::can_id_t can_id, const std::shared_ptr<CanBus> can_bus) :
    ECU(can_id, can_bus)
{
    set_can_bus_protocol(nssubaru::CAN_BUS_PROTOCOL);
}
