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
    using respond_callback_t = std::function<void(const std::string&)>;
    using protocol_select_callback_t = std::function<void(const elm::obd::protocol_t)>;

public:
    DLC(const std::shared_ptr<CanBus> can_bus);
    void init(const dlc::device_t device, respond_callback_t callback);

    void request(const std::string &request) const;
    void put(const can::protocol_t protocol, const can::msg_t &message);

private:
    void on_respond(const std::string &response) const;
    void on_transmit(const can::header_t header, const can::data_t &data) const;
    void on_select_protocol(const elm::obd::protocol_t protocol);

    std::unique_ptr<Elm327> m_elm327 = nullptr;
    std::unique_ptr<OBDLink> m_obdlink = nullptr;

    dlc::device_t m_data_link_device;

    respond_callback_t m_respond_callback = nullptr;
};

#endif // DATALINKCONNECTOR_H
