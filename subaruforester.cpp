
#include "subaruforester.h"

SubaruForester::SubaruForester() :
    m_can_bus(std::make_shared<CanBus>())
  , m_ecus {
          { can_types::ecu_id_t::ecu_id_dlc, std::make_shared<DLC>(can_types::ecu_id_t::ecu_id_dlc, m_can_bus) }
        , { can_types::ecu_id_t::ecu_id_ecm, std::make_shared<ECM>(can_types::ecu_id_t::ecu_id_ecm, m_can_bus) }
        , { can_types::ecu_id_t::ecu_id_tcm, std::make_shared<TCM>(can_types::ecu_id_t::ecu_id_tcm, m_can_bus) }
        }
{
}

const std::shared_ptr<CanBus> SubaruForester::get_can_bus() const
{
    return m_can_bus;
}

const std::shared_ptr<DLC> SubaruForester::get_dlc() const
{
    return std::dynamic_pointer_cast<DLC>(m_ecus.at(can_types::ecu_id_t::ecu_id_dlc));
}

const std::shared_ptr<ECM> SubaruForester::get_ecm() const
{
    return std::dynamic_pointer_cast<ECM>(m_ecus.at(can_types::ecu_id_t::ecu_id_dlc));
}

const std::shared_ptr<TCM> SubaruForester::get_tcm() const
{
    return std::dynamic_pointer_cast<TCM>(m_ecus.at(can_types::ecu_id_t::ecu_id_dlc));
}
