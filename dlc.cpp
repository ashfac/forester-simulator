#include "dlc.h"

DLC::DLC(const can::can_id_t can_id, const std::shared_ptr<CanBus> can_bus) :
    ECU(can_id, can_bus)
{
    set_can_protocol(can::protocol::unknown);
}

void DLC::connect(const dlc::device_t device, respond_serial_callback_t callback)
{
    m_data_link_device = device;
    m_respond_serial_callback = callback;

    switch (m_data_link_device) {

    case dlc::device::elm327:
        m_elm327 = std::make_unique<Elm327>(std::bind(&DLC::on_respond_can, this, std::placeholders::_1),
                                            std::bind(&DLC::on_respond_serial, this, std::placeholders::_1),
                                            std::bind(&DLC::on_protocol_select, this, std::placeholders::_1));
        break;

    case dlc::device::obdlink:
        m_obdlink = std::make_unique<OBDLink>(std::bind(&DLC::on_respond_can, this, std::placeholders::_1),
                                              std::bind(&DLC::on_respond_serial, this, std::placeholders::_1),
                                              std::bind(&DLC::on_protocol_select, this, std::placeholders::_1));
        break;

    default:
        break;
    }
}

void DLC::disconnect()
{
    switch (m_data_link_device) {

    case dlc::device::elm327:
        break;

    case dlc::device::obdlink:
        break;

    default:
        break;
    }
}

void DLC::send(const std::string &request) const
{
    switch (m_data_link_device) {

    case dlc::device::transparent:
        break;

    case dlc::device::elm327:
        if(m_elm327) {
            m_elm327->send(request);
        }
        break;

    case dlc::device::obdlink:
        if(m_obdlink) {
            m_obdlink->send(request);
        }
        break;
    }
}

void DLC::send(const can::can_msg_t &message) const
{
    switch (m_data_link_device) {

    case dlc::device::transparent:
        break;

    case dlc::device::elm327:
        if(m_elm327) {
            m_elm327->send(message);
        }
        break;

    case dlc::device::obdlink:
        if(m_obdlink) {
            m_obdlink->send(message);
        }
        break;
    }
}

void DLC::on_respond_serial(const std::string &response) const
{
    m_respond_serial_callback(response);
}

void DLC::on_respond_can(const can::can_msg_t &message) const
{
    get_can_bus()->transmit(shared_from_this(), message);
}

void DLC::on_protocol_select(const elm::obd::protocol_t protocol)
{
    switch (protocol) {
    case elm::obd::protocol::protocol::iso_15765_4_can_11bit_id_500kbaud:
        set_can_protocol(can::protocol::iso_15765_4_can_11bit_id_500kbaud);
        break;

    case elm::obd::protocol::protocol::iso_15765_4_can_29bit_id_500kbaud:
        set_can_protocol(can::protocol::iso_15765_4_can_29bit_id_500kbaud);
        break;

    case elm::obd::protocol::protocol::iso_15765_4_can_11bit_id_250kbaud:
        set_can_protocol(can::protocol::iso_15765_4_can_11bit_id_250kbaud);
        break;

    case elm::obd::protocol::protocol::iso_15765_4_can_29bit_id_250kbaud:
        set_can_protocol(can::protocol::iso_15765_4_can_29bit_id_250kbaud);
        break;

    case elm::obd::protocol::protocol::sae_j1939_can_29bit_id_250kbaud:
        set_can_protocol(can::protocol::sae_j1939_can_29bit_id_250kbaud);
        break;

    case elm::obd::protocol::protocol::user1_can_11bit_id_125kbaud:
        set_can_protocol(can::protocol::user1_can_11bit_id_125kbaud);
        break;

    case elm::obd::protocol::protocol::user2_can_11bit_id_50kbaud:
        set_can_protocol(can::protocol::user2_can_11bit_id_50kbaud);
        break;

    default:
        set_can_protocol(can::protocol::unknown);
        break;
    }

    get_can_bus()->attach(shared_from_this());
}
