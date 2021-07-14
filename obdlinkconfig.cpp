#include "obdlinkconfig.h"

OBDLinkConfig::OBDLinkConfig()
{
    load_defaults();
}

void OBDLinkConfig::load_defaults()
{
    m_adaptive_timing = nsobdlink::adaptive_timing_t::adaptive_timing_auto_1;
    m_can_auto_formatting = nsobdlink::on_off_t::on;
    m_can_flow_control = nsobdlink::on_off_t::on;
    m_can_silent_mode = nsobdlink::on_off_t::on;
    m_dlc = nsobdlink::on_off_t::on;
    m_echo = nsobdlink::on_off_t::on;
    m_headers = nsobdlink::on_off_t::on;
    m_j1939_header_formatting = nsobdlink::on_off_t::on;
    m_keyword_checking = nsobdlink::on_off_t::on;
    m_linefeeds = nsobdlink::on_off_t::on;
    m_memory = nsobdlink::on_off_t::on;
    m_responses = nsobdlink::on_off_t::on;
    m_spaces = nsobdlink::on_off_t::on;
    m_variable_dlc = nsobdlink::on_off_t::on;
}
