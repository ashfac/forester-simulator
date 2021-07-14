#ifndef ECU_H
#define ECU_H

#include "can_types.h"
#include "canbus.h"

class CanBus;

class ECU
{
public:
    ECU(const can_types::ecu_id_t ecu_id, const std::shared_ptr<CanBus> can_bus);
    virtual ~ECU() { }

    void transmit(const can_types::can_msg_t &message) const;

protected:
    const std::shared_ptr<CanBus> getCanBus() const { return m_can_bus; }
    can_types::ecu_id_t get_ecu_id() const { return m_ecu_id; }

private:
    can_types::ecu_id_t m_ecu_id;
    std::shared_ptr<CanBus> m_can_bus;
};

#endif // ECU_H
