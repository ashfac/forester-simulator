#include "elm327config.h"

Elm327Config::Elm327Config()
{
    load_defaults();
}

void Elm327Config::load_defaults()
{
    m_can_auto_formatting = elm::config::defaults::CAN_AUTO_FORMATTING;
    m_can_flow_control = elm::config::defaults::CAN_FLOW_CONTROL;
    m_can_silent_mode = elm::config::defaults::CAN_SILENT_MODE;
    m_dlc = elm::config::defaults::DLC;
    m_echo = elm::config::defaults::ECHO;
    m_headers = elm::config::defaults::HEADERS;
    m_j1939_header_formatting = elm::config::defaults::J1939_HEADER_FORMATTING;
    m_keyword_checking = elm::config::defaults::KEYWORD_CHECKING;
    m_linefeeds = elm::config::defaults::LINEFEEDS;
    m_memory = elm::config::defaults::MEMORY;
    m_responses = elm::config::defaults::RESPONSES;
    m_spaces = elm::config::defaults::SPACES;
    m_variable_dlc = elm::config::defaults::VARIABLE_DLC;
    m_adaptive_timing = elm::config::defaults::ADAPTIVE_TIMING;
    m_obd_protocol = elm::config::defaults::OBD_PROTOCOL;
    m_obd_request_header = elm::obd::BROADCAST_HEADER_STANDARD;
}

bool Elm327Config::set_obd_protocol(elm::obd::protocol_t arg)
{
    if ( arg >= elm::obd::protocol::MIN && arg <= elm::obd::protocol::MAX ) {
        m_obd_protocol = arg;
        return true;
    }

    return false;
}
