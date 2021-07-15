#ifndef DATALINKCONNECTOR_H
#define DATALINKCONNECTOR_H

#include "elm327.h"
#include "obdlink.h"
#include "ecu.h"

namespace dlc {

namespace device {

enum device : int {
    transparent,
    elm327,
    obdlink
};

} // namespace device

using device_t = dlc::device::device;

} // namespace dlc

class DLC : public ECU
{
private:
    using respond_can_callback_t = std::function<void(const can::can_msg_t&)>;
    using respond_serial_callback_t = std::function<void(const std::string&)>;
    using protocol_select_callback_t = std::function<void(const elm::obd::protocol_t)>;

public:
    DLC(const can::can_id_t can_id, const std::shared_ptr<CanBus> can_bus);

    void connect(const dlc::device_t device, respond_serial_callback_t callback);
    void disconnect();

    void send(const std::string &request) const;
    void send(const can::can_msg_t &message) const;

private:
    void on_respond_serial(const std::string &response) const;
    void on_respond_can(const can::can_msg_t &message) const;
    void on_protocol_select(const elm::obd::protocol_t protocol);

    std::unique_ptr<Elm327> m_elm327 = nullptr;
    std::unique_ptr<OBDLink> m_obdlink = nullptr;

    dlc::device_t m_data_link_device;

    respond_serial_callback_t m_respond_serial_callback = nullptr;
};

#endif // DATALINKCONNECTOR_H
