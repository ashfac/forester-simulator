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
    using receive_callback_t = std::function<void(const std::string&)>;

public:
    DLC(const can_types::ecu_id_t ecu_id, const std::shared_ptr<CanBus> can_bus);
    ~DLC();

    void connect(const data_link_device_t device, receive_callback_t callback);
    void disconnect();

    void send(const std::string &data);

private:
    void decode(const std::string &data);
    void encode(const std::string &data);

    void on_decoded(const can_types::can_msg_t &message) const;
    void on_encoded(const std::string &data) const;

    OBDLink *m_obdlink = nullptr;

    data_link_device_t m_data_link_device;
    receive_callback_t m_receive_callback = nullptr;

};

#endif // DATALINKCONNECTOR_H
