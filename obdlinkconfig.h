#ifndef OBDLINKCONFIG_H
#define OBDLINKCONFIG_H

#include <string>

namespace nsobdlink {

namespace versions {
const std::string elm_device_id = "ELM327 v1.4b";

}

typedef enum on_off : int {
    off = 0,
    on = 1,
    invalid = -1
} on_off_t;

typedef enum adaptive_timing : int {
    adaptive_timing_off = 0,
    adaptive_timing_auto_1 = 1,
    adaptive_timing_auto_2 = 2
} adaptive_timing_t;

namespace response_t {
const std::string ok = "OK";
const std::string invalid = "?";
const std::string pending = "!pending!";
}

}

class OBDLinkConfig
{
public:
    OBDLinkConfig();
    void load_defaults();

    nsobdlink::on_off_t get_echo() const { return m_echo; }
    void set_echo(const nsobdlink::on_off_t echo) { m_echo = echo; }

private:

    nsobdlink::adaptive_timing_t m_adaptive_timing;
    nsobdlink::on_off_t m_can_auto_formatting;
    nsobdlink::on_off_t m_can_flow_control;
    nsobdlink::on_off_t m_can_silent_mode;
    nsobdlink::on_off_t m_dlc;
    nsobdlink::on_off_t m_echo;
    nsobdlink::on_off_t m_headers;

    nsobdlink::on_off_t m_j1939_header_formatting;
    nsobdlink::on_off_t m_keyword_checking;
    nsobdlink::on_off_t m_linefeeds;
    nsobdlink::on_off_t m_memory;
    nsobdlink::on_off_t m_responses;
    nsobdlink::on_off_t m_spaces;
    nsobdlink::on_off_t m_variable_dlc;

};

#endif // OBDLINKCONFIG_H
