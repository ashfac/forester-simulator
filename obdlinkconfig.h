#ifndef OBDLINKCONFIG_H
#define OBDLINKCONFIG_H

#include "nsobdlink.h"
#include "can_types.h"

class OBDLinkConfig
{
public:
    OBDLinkConfig();
    void load_defaults();

    nsobdlink::config::adaptive_timing_t get_adaptive_timing() const { return m_adaptive_timing; }
    void set_adaptive_timing(const nsobdlink::config::adaptive_timing_t arg) { m_adaptive_timing = arg; }

    bool get_can_auto_formatting() const { return m_can_auto_formatting; }
    void set_can_auto_formatting(const bool arg) { m_can_auto_formatting = arg; }

    bool get_can_flow_control() const { return m_can_flow_control; }
    void set_can_flow_control(const bool arg) { m_can_flow_control = arg; }

    bool get_can_silent_mode() const { return m_can_silent_mode; }
    void set_can_silent_mode(const bool arg) { m_can_silent_mode = arg; }

    bool get_dlc() const { return m_dlc; }
    void set_dlc(const bool arg) { m_dlc = arg; }

    bool get_echo() const { return m_echo; }
    void set_echo(const bool arg) { m_echo = arg; }

    bool get_headers() const { return m_headers; }
    void set_headers(const bool arg) { m_headers = arg; }

    bool get_j1939_header_formatting() const { return m_j1939_header_formatting; }
    void set_j1939_header_formatting(const bool arg) { m_j1939_header_formatting = arg; }

    bool get_keyword_checking() const { return m_keyword_checking; }
    void set_keyword_checking(const bool arg) { m_keyword_checking = arg; }

    bool get_linefeeds() const { return m_linefeeds; }
    void set_linefeeds(const bool arg) { m_linefeeds = arg; }

    bool get_memory() const { return m_memory; }
    void set_memory(const bool arg) { m_memory = arg; }

    bool get_responses() const { return m_responses; }
    void set_responses(const bool arg) { m_responses = arg; }

    bool get_spaces() const { return m_spaces; }
    void set_spaces(const bool arg) { m_spaces = arg; }

    bool get_variable_dlc() const { return m_variable_dlc; }
    void set_variable_dlc(const bool arg) { m_variable_dlc = arg; }

    nsobdlink::config::obd_protocol_t get_obd_protocol() const { return m_obd_protocol; }
    bool set_obd_protocol(nsobdlink::config::obd_protocol_t arg);

    can_types::can_id_t get_obd_request_header() const { return m_obd_request_header; }
    void set_obd_request_header(can_types::can_id_t arg) { m_obd_request_header = arg; }

private:
    nsobdlink::config::adaptive_timing_t m_adaptive_timing;
    bool m_can_auto_formatting;
    bool m_can_flow_control;
    bool m_can_silent_mode;
    bool m_dlc;
    bool m_echo;
    bool m_headers;

    bool m_j1939_header_formatting;
    bool m_keyword_checking;
    bool m_linefeeds;
    bool m_memory;
    bool m_responses;
    bool m_spaces;
    bool m_variable_dlc;

    nsobdlink::config::obd_protocol_t m_obd_protocol;
    can_types::can_id_t m_obd_request_header;

private:

};

#endif // OBDLINKCONFIG_H
