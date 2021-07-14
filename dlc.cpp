#include "dlc.h"

DLC::DLC(const can_types::ecu_id_t ecu_id, const std::shared_ptr<CanBus> can_bus) :
    ECU(ecu_id, can_bus)
{
}

DLC::~DLC()
{
}

void DLC::connect(const data_link_device_t device, receive_callback_t callback)
{
    m_data_link_device = device;
    m_receive_callback = callback;

    switch (m_data_link_device) {

    case data_link_device_t::device_obdlink:
        m_obdlink = new OBDLink(
                        std::bind(&DLC::on_decoded, this, std::placeholders::_1),
                        std::bind(&DLC::on_encoded, this, std::placeholders::_1)
                    );
        break;

    default:
        break;
    }
}

void DLC::disconnect()
{
    switch (m_data_link_device) {

    case data_link_device_t::device_obdlink:
        delete m_obdlink;
        m_obdlink = nullptr;
        break;

    default:
        break;
    }
}

void DLC::send(const std::string &data)
{
    decode(data);
}

void DLC::decode(const std::string &data)
{
    switch (m_data_link_device) {

    case data_link_device_t::device_transparent:
        break;

    case data_link_device_t::device_obdlink:
        if(m_obdlink) {
            m_obdlink->send(data);
        }
        break;
    }
}

void DLC::encode(const std::string &data)
{
    switch (m_data_link_device) {

    case data_link_device_t::device_transparent:
        break;

    case data_link_device_t::device_obdlink:
        if(m_obdlink) {
            m_obdlink->send(data);
        }
    }
}

void DLC::on_decoded(const can_types::can_msg_t &message) const
{
    getCanBus()->transmit(message);
}

void DLC::on_encoded(const std::string &data) const
{
    m_receive_callback(data);
}
