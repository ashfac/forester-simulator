#ifndef ECU_H
#define ECU_H

#include "subaru.h"
#include "can.h"
#include "canbus.h"

class CanBus;

class ECU : public std::enable_shared_from_this<ECU>
{
public:
    ECU(const can::can_id_t ecu_id, const std::shared_ptr<CanBus> can_bus);
    virtual ~ECU() { }

    void transmit(const can::can_msg_t &message) const;

    can::protocol_t get_can_bus_protocol() const { return m_can_bus_protocol; }

protected:
    const std::shared_ptr<CanBus> get_can_bus() const { return m_can_bus; }
    can::can_id_t get_ecu_id() const { return m_can_id; }
    void set_can_protocol(const can::protocol_t  protocol) { m_can_bus_protocol = protocol; };

private:
    can::can_id_t m_can_id;
    std::shared_ptr<CanBus> m_can_bus;

    can::protocol_t m_can_bus_protocol;
};

#endif // ECU_H
