#ifndef DATALINKCONNECTOR_H
#define DATALINKCONNECTOR_H

#include "obdlink.h"
#include "ecu.h"

class DLC : public ECU
{
public:
    using data_link_device_t = enum {
        device_transparent,
        device_obdlink
    } ;

private:
    using respond_can_callback_t = std::function<void(const can_types::can_msg_t&)>;
    using respond_serial_callback_t = std::function<void(const std::string&)>;
    using protocol_select_callback_t = std::function<void(const nsobdlink::config::obd_protocol_t)>;

public:
    DLC(const can_types::can_id_t can_id, const std::shared_ptr<CanBus> can_bus);
    ~DLC();

    void connect(const data_link_device_t device, respond_serial_callback_t callback);
    void disconnect();

    void send(const std::string &request) const;
    void send(const can_types::can_msg_t &message) const;

private:
    void on_respond_serial(const std::string &response) const;
    void on_respond_can(const can_types::can_msg_t &message) const;
    void on_protocol_select(const nsobdlink::config::obd_protocol_t protocol);

    std::unique_ptr<OBDLink> m_obdlink = nullptr;

    data_link_device_t m_data_link_device;

    respond_serial_callback_t m_respond_serial_callback = nullptr;
};

#endif // DATALINKCONNECTOR_H
