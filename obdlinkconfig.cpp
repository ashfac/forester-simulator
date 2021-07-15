#include "obdlinkconfig.h"

OBDLinkConfig::OBDLinkConfig()
{
    load_defaults();
}

void OBDLinkConfig::load_defaults()
{
    m_adaptive_timing = nsobdlink::config::defaults::ADAPTIVE_TIMING;
    m_can_auto_formatting = nsobdlink::config::defaults::CAN_AUTO_FORMATTING;
    m_can_flow_control = nsobdlink::config::defaults::CAN_FLOW_CONTROL;
    m_can_silent_mode = nsobdlink::config::defaults::CAN_SILENT_MODE;
    m_dlc = nsobdlink::config::defaults::DLC;
    m_echo = nsobdlink::config::defaults::ECHO;
    m_headers = nsobdlink::config::defaults::HEADERS;
    m_j1939_header_formatting = nsobdlink::config::defaults::J1939_HEADER_FORMATTING;
    m_keyword_checking = nsobdlink::config::defaults::KEYWORD_CHECKING;
    m_linefeeds = nsobdlink::config::defaults::LINEFEEDS;
    m_memory = nsobdlink::config::defaults::MEMORY;
    m_responses = nsobdlink::config::defaults::RESPONSES;
    m_spaces = nsobdlink::config::defaults::SPACES;
    m_variable_dlc = nsobdlink::config::defaults::VARIABLE_DLC;
    m_obd_protocol = nsobdlink::config::defaults::OBD_PROTOCOL;
    m_obd_request_header = nsobdlink::config::defaults::OBD_HEADER_BROADCAST;
}

bool OBDLinkConfig::set_obd_protocol(nsobdlink::config::obd_protocol_t arg)
{
    if ( arg >= nsobdlink::config::OBD_PROTOCOL_MIN &&
         arg <= nsobdlink::config::OBD_PROTOCOL_MAX ) {
        m_obd_protocol = arg;
        return true;
    }

    return false;
}
