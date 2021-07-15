
#include "subaruforester.h"

SubaruForester::SubaruForester() :
    m_can_bus(std::make_shared<CanBus>())
  , m_ecus {
          { nssubaru::CAN_ID_BROADCAST, std::make_shared<DLC>(nssubaru::CAN_ID_BROADCAST, m_can_bus) }
        , { nssubaru::CAN_ID_ECM, std::make_shared<ECM>(nssubaru::CAN_ID_ECM, m_can_bus) }
        , { nssubaru::CAN_ID_TCM, std::make_shared<TCM>(nssubaru::CAN_ID_TCM, m_can_bus) }
        }
{
}

const std::shared_ptr<CanBus> SubaruForester::get_can_bus() const
{
    return m_can_bus;
}

const std::shared_ptr<DLC> SubaruForester::get_dlc() const
{
    return std::dynamic_pointer_cast<DLC>(m_ecus.at(nssubaru::CAN_ID_BROADCAST));
}

const std::shared_ptr<ECM> SubaruForester::get_ecm() const
{
    return std::dynamic_pointer_cast<ECM>(m_ecus.at(nssubaru::CAN_ID_ECM));
}

const std::shared_ptr<TCM> SubaruForester::get_tcm() const
{
    return std::dynamic_pointer_cast<TCM>(m_ecus.at(nssubaru::CAN_ID_TCM));
}
