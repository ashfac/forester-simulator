#ifndef ECU_H
#define ECU_H

#include "can.h"
#include "obd.h"
#include "canbus.h"

class CanBus;

class ECU : public std::enable_shared_from_this<ECU>
{
public:
    ECU(const std::shared_ptr<CanBus> can_bus, const can::can_id_t ecu_id, const can::protocol_t protocol);

    virtual void put(const can::protocol_t protocol, can::can_msg_t message) = 0;

protected:
    void transmit(const can::can_msg_t &message) const;

    const std::shared_ptr<CanBus> get_can_bus() const { return m_can_bus; }
    can::can_id_t get_ecu_id() const { return m_ecu_id; }

    can::protocol_t get_protocol() const { return m_protocol; }
    void set_protocol(const can::protocol_t  protocol) { m_protocol = protocol; };

private:
    std::shared_ptr<CanBus> m_can_bus;
    can::can_id_t m_ecu_id;
    can::protocol_t m_protocol;
};

#endif // ECU_H
