#include "dlc.h"

DLC::DLC(const std::shared_ptr<CanBus> can_bus, const can::can_id_t ecu_id, const can::protocol_t protocol) :
    ECU(can_bus, ecu_id, protocol)
{
}

void DLC::init(const dlc::device_t device, respond_callback_t callback)
{
    m_data_link_device = device;
    m_respond_callback = callback;

    switch (m_data_link_device) {

    case dlc::device::elm327:
        m_elm327 = std::make_unique<Elm327>(std::bind(&DLC::on_transmit, this, std::placeholders::_1),
                                            std::bind(&DLC::on_respond, this, std::placeholders::_1),
                                            std::bind(&DLC::on_select_protocol, this, std::placeholders::_1));
        break;

    case dlc::device::obdlink:
        m_obdlink = std::make_unique<OBDLink>(std::bind(&DLC::on_transmit, this, std::placeholders::_1),
                                              std::bind(&DLC::on_respond, this, std::placeholders::_1),
                                              std::bind(&DLC::on_select_protocol, this, std::placeholders::_1));
        break;

    default:
        break;
    }
}

void DLC::request(const std::string &request) const
{
    switch (m_data_link_device) {

    case dlc::device::transparent:
        break;

    case dlc::device::elm327:
        if(m_elm327) {
            m_elm327->request(request);
        }
        break;

    case dlc::device::obdlink:
        if(m_obdlink) {
            m_obdlink->request(request);
        }
        break;
    }
}

void DLC::put(const can::protocol_t protocol, const can::can_msg_t &message)
{
    if ( get_protocol() != protocol ) return;

    switch (m_data_link_device) {

    case dlc::device::transparent:
        break;

    case dlc::device::elm327:
        if(m_elm327) {
            m_elm327->put(message);
        }
        break;

    case dlc::device::obdlink:
        if(m_obdlink) {
            m_obdlink->put(message);
        }
        break;
    }
}

void DLC::on_respond(const std::string &response) const
{
    m_respond_callback( response );
}

void DLC::on_transmit(const can::can_msg_t &message) const
{
    get_can_bus()->transmit( shared_from_this(), get_protocol(), message );
}

void DLC::on_select_protocol(const elm::obd::protocol_t protocol)
{
    switch ( protocol ) {

    case elm::obd::protocol::protocol::iso_15765_4_can_11bit_id_500kbaud:
        set_protocol( can::protocol::iso_15765_4_can_11bit_id_500kbaud );
        break;

    case elm::obd::protocol::protocol::iso_15765_4_can_29bit_id_500kbaud:
        set_protocol( can::protocol::iso_15765_4_can_29bit_id_500kbaud );
        break;

    case elm::obd::protocol::protocol::iso_15765_4_can_11bit_id_250kbaud:
        set_protocol( can::protocol::iso_15765_4_can_11bit_id_250kbaud );
        break;

    case elm::obd::protocol::protocol::iso_15765_4_can_29bit_id_250kbaud:
        set_protocol( can::protocol::iso_15765_4_can_29bit_id_250kbaud );
        break;

    case elm::obd::protocol::protocol::sae_j1939_can_29bit_id_250kbaud:
        set_protocol( can::protocol::sae_j1939_can_29bit_id_250kbaud );
        break;

    case elm::obd::protocol::protocol::user1_can_11bit_id_125kbaud:
        set_protocol( can::protocol::user1_can_11bit_id_125kbaud );
        break;

    case elm::obd::protocol::protocol::user2_can_11bit_id_50kbaud:
        set_protocol( can::protocol::user2_can_11bit_id_50kbaud );
        break;

    default:
        set_protocol( can::protocol::unknown );
        break;
    }
}
