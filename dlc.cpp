#include "dlc.h"

DLC::DLC(const can_types::can_id_t can_id, const std::shared_ptr<CanBus> can_bus) :
    ECU(can_id, can_bus)
{
    set_can_bus_protocol(can_types::can_bus_protocol::protocol_unknown);
}

DLC::~DLC()
{
}

void DLC::connect(const data_link_device_t device, respond_serial_callback_t callback)
{
    m_data_link_device = device;
    m_respond_serial_callback = callback;

    switch (m_data_link_device) {

    case data_link_device_t::device_obdlink:
        m_obdlink = std::make_unique<OBDLink>(
                        std::bind(&DLC::on_respond_can, this, std::placeholders::_1),
                        std::bind(&DLC::on_respond_serial, this, std::placeholders::_1),
                        std::bind(&DLC::on_protocol_select, this, std::placeholders::_1)
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
        break;

    default:
        break;
    }
}

void DLC::send(const std::string &request) const
{
    switch (m_data_link_device) {

    case data_link_device_t::device_transparent:
        break;

    case data_link_device_t::device_obdlink:
        if(m_obdlink) {
            m_obdlink->send(request);
        }
        break;
    }
}

void DLC::send(const can_types::can_msg_t &message) const
{
    switch (m_data_link_device) {

    case data_link_device_t::device_transparent:
        break;

    case data_link_device_t::device_obdlink:
        if(m_obdlink) {
            m_obdlink->send(message);
        }
    }
}

void DLC::on_respond_serial(const std::string &response) const
{
    m_respond_serial_callback(response);
}

void DLC::on_respond_can(const can_types::can_msg_t &message) const
{
    get_can_bus()->transmit(shared_from_this(), message);
}

void DLC::on_protocol_select(const nsobdlink::config::obd_protocol_t protocol)
{
    switch (protocol) {
    case nsobdlink::config::obd_protocol::obd_protocol_6_iso_15765_4_can_11bit_id_500kbaud:
        set_can_bus_protocol(can_types::can_bus_protocol::iso_15765_4_can_11bit_id_500kbaud);
        break;

    case nsobdlink::config::obd_protocol::obd_protocol_7_iso_15765_4_can_29bit_id_500kbaud:
        set_can_bus_protocol(can_types::can_bus_protocol::iso_15765_4_can_29bit_id_500kbaud);
        break;

    case nsobdlink::config::obd_protocol::obd_protocol_8_iso_15765_4_can_11bit_id_250kbaud:
        set_can_bus_protocol(can_types::can_bus_protocol::iso_15765_4_can_11bit_id_250kbaud);
        break;

    case nsobdlink::config::obd_protocol::obd_protocol_9_iso_15765_4_can_29bit_id_250kbaud:
        set_can_bus_protocol(can_types::can_bus_protocol::iso_15765_4_can_29bit_id_250kbaud);
        break;

    case nsobdlink::config::obd_protocol::obd_protocol_A_sae_j1939_can_29bit_id_250kbaud:
        set_can_bus_protocol(can_types::can_bus_protocol::sae_j1939_can_29bit_id_250kbaud);
        break;

    case nsobdlink::config::obd_protocol::obd_protocol_B_user1_can_11bit_id_125kbaud:
        set_can_bus_protocol(can_types::can_bus_protocol::user1_can_11bit_id_125kbaud);
        break;

    case nsobdlink::config::obd_protocol::obd_protocol_C_user2_can_11bit_id_50kbaud:
        set_can_bus_protocol(can_types::can_bus_protocol::user2_can_11bit_id_50kbaud);
        break;

    default:
        set_can_bus_protocol(can_types::can_bus_protocol::protocol_unknown);
        break;
    }

    get_can_bus()->attach(shared_from_this());
}
