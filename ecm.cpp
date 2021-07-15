#include "ecm.h"

ECM::ECM(const can::can_id_t can_id, const std::shared_ptr<CanBus> can_bus) :
    ECU(can_id, can_bus)
{
    set_can_protocol(subaru::CAN_PROTOCOL);
}
