#ifndef OBDLINK_H
#define OBDLINK_H

#include <string>
#include <sstream>
#include <functional>
#include <vector>
#include <map>

#include "obdlinkconfig.h"
#include "canbus.h"

class OBDLink
{
public:
    using transmit_on_can_bus_callback_t = std::function<void(const can_types::can_msg_t&)>;
    using send_to_serial_callback_t = std::function<void(const std::string&)>;
    using protocol_select_callback_t = std::function<void(const nsobdlink::config::obd_protocol_t)>;

private:
    using command_handler_t = std::function<std::string(const std::vector<std::string> &)>;
    using command_set_t = std::map<std::string, command_handler_t>;

public:
    OBDLink(const transmit_on_can_bus_callback_t respond_can_callback,
            const send_to_serial_callback_t respond_serial_callback,
            const protocol_select_callback_t protocol_select_callback);

    void send(const std::string &data);
    void send(const can_types::can_msg_t &message);

private:
    void echo(const std::string &data);
    void send_to_serial(const std::string &response);
    void transmit_on_can_bus(const can_types::can_msg_t &message);

    std::string format_response(const std::string &str);

    size_t min_command_size();
    size_t max_command_size();

    static unsigned long hex_string_to_int(const std::string &str);

    static std::vector<std::string> get_arguments(const std::string& str);
    static int get_integer_argument(const std::vector<std::string> &args);
    std::string set_on_off_config(const nsobdlink::config::boolean_config_t config_item, const std::vector<std::string> &args);
    void set_on_off_config(const nsobdlink::config::boolean_config_t config_item, const bool value);


    bool is_command(const std::string &request);
    std::string handle_command(const std::string &request);
    std::string handle_command(const std::string &request, const std::string &args);
    void handle_can_message(const std::string &request);

    // AT commands, in alphabetic order
    std::string handle_AT_at(const std::vector<std::string> &args);
    std::string handle_ATAL(const std::vector<std::string> &args);
    std::string handle_ATAR(const std::vector<std::string> &args);
    std::string handle_ATAT(const std::vector<std::string> &args);
    std::string handle_ATBD(const std::vector<std::string> &args);
    std::string handle_ATBI(const std::vector<std::string> &args);
    std::string handle_ATBRD(const std::vector<std::string> &args);
    std::string handle_ATBRT(const std::vector<std::string> &args);
    std::string handle_ATCAF(const std::vector<std::string> &args);
    std::string handle_ATCEA(const std::vector<std::string> &args);
    std::string handle_ATCF(const std::vector<std::string> &args);
    std::string handle_ATCFC(const std::vector<std::string> &args);
    std::string handle_ATCM(const std::vector<std::string> &args);
    std::string handle_ATCP(const std::vector<std::string> &args);
    std::string handle_ATCRA(const std::vector<std::string> &args);
    std::string handle_ATCS(const std::vector<std::string> &args);
    std::string handle_ATCSM(const std::vector<std::string> &args);
    std::string handle_ATCV(const std::vector<std::string> &args);
    std::string handle_ATD(const std::vector<std::string> &args);
    std::string handle_ATDM1(const std::vector<std::string> &args);
    std::string handle_ATDP(const std::vector<std::string> &args);
    std::string handle_ATDPN(const std::vector<std::string> &args);
    std::string handle_ATE(const std::vector<std::string> &args);
    std::string handle_ATFC(const std::vector<std::string> &args);
    std::string handle_ATFE(const std::vector<std::string> &args);
    std::string handle_ATFI(const std::vector<std::string> &args);
    std::string handle_ATH(const std::vector<std::string> &args);
    std::string handle_ATI(const std::vector<std::string> &args);
    std::string handle_ATIB(const std::vector<std::string> &args);
    std::string handle_ATIFR(const std::vector<std::string> &args);
    std::string handle_ATIGN(const std::vector<std::string> &args);
    std::string handle_ATIIA(const std::vector<std::string> &args);
    std::string handle_ATJE(const std::vector<std::string> &args);
    std::string handle_ATJHF(const std::vector<std::string> &args);
    std::string handle_ATJS(const std::vector<std::string> &args);
    std::string handle_ATJTM1(const std::vector<std::string> &args);
    std::string handle_ATJTM5(const std::vector<std::string> &args);
    std::string handle_ATKW(const std::vector<std::string> &args);
    std::string handle_ATL(const std::vector<std::string> &args);
    std::string handle_ATLP(const std::vector<std::string> &args);
    std::string handle_ATM(const std::vector<std::string> &args);
    std::string handle_ATMA(const std::vector<std::string> &args);
    std::string handle_ATMP(const std::vector<std::string> &args);
    std::string handle_ATMR(const std::vector<std::string> &args);
    std::string handle_ATMT(const std::vector<std::string> &args);
    std::string handle_ATNL(const std::vector<std::string> &args);
    std::string handle_ATPB(const std::vector<std::string> &args);
    std::string handle_ATPC(const std::vector<std::string> &args);
    std::string handle_ATPP(const std::vector<std::string> &args);
    std::string handle_ATPPS(const std::vector<std::string> &args);
    std::string handle_ATR(const std::vector<std::string> &args);
    std::string handle_ATRA(const std::vector<std::string> &args);
    std::string handle_ATRD(const std::vector<std::string> &args);
    std::string handle_ATRTR(const std::vector<std::string> &args);
    std::string handle_ATRV(const std::vector<std::string> &args);
    std::string handle_ATS(const std::vector<std::string> &args);
    std::string handle_ATSD(const std::vector<std::string> &args);
    std::string handle_ATSH(const std::vector<std::string> &args);
    std::string handle_ATSI(const std::vector<std::string> &args);
    std::string handle_ATSP(const std::vector<std::string> &args);
    std::string handle_ATSR(const std::vector<std::string> &args);
    std::string handle_ATSS(const std::vector<std::string> &args);
    std::string handle_ATST(const std::vector<std::string> &args);
    std::string handle_ATSW(const std::vector<std::string> &args);
    std::string handle_ATTA(const std::vector<std::string> &args);
    std::string handle_ATTP(const std::vector<std::string> &args);
    std::string handle_ATV(const std::vector<std::string> &args);
    std::string handle_ATWM(const std::vector<std::string> &args);
    std::string handle_ATWS(const std::vector<std::string> &args);
    std::string handle_ATZ(const std::vector<std::string> &args);


    std::string command_not_implemented(const std::vector<std::string> &args);

private:
    const command_set_t m_command_set = {
          { "AT@", std::bind(&OBDLink::handle_AT_at, this, std::placeholders::_1) }
        , { "ATAL", std::bind(&OBDLink::handle_ATAL, this, std::placeholders::_1) }
        , { "ATAR", std::bind(&OBDLink::handle_ATAR, this, std::placeholders::_1) }
        , { "ATAT", std::bind(&OBDLink::handle_ATAT, this, std::placeholders::_1) }
        , { "ATBD", std::bind(&OBDLink::handle_ATBD, this, std::placeholders::_1) }
        , { "ATBI", std::bind(&OBDLink::handle_ATBI, this, std::placeholders::_1) }
        , { "ATBRD", std::bind(&OBDLink::handle_ATBRD, this, std::placeholders::_1) }
        , { "ATBRT", std::bind(&OBDLink::handle_ATBRT, this, std::placeholders::_1) }
        , { "ATCAF", std::bind(&OBDLink::handle_ATCAF, this, std::placeholders::_1) }
        , { "ATCEA", std::bind(&OBDLink::handle_ATCEA, this, std::placeholders::_1) }
        , { "ATCF", std::bind(&OBDLink::handle_ATCF, this, std::placeholders::_1) }
        , { "ATCFC", std::bind(&OBDLink::handle_ATCFC, this, std::placeholders::_1) }
        , { "ATCM", std::bind(&OBDLink::handle_ATCM, this, std::placeholders::_1) }
        , { "ATCP", std::bind(&OBDLink::handle_ATCP, this, std::placeholders::_1) }
        , { "ATCRA", std::bind(&OBDLink::handle_ATCRA, this, std::placeholders::_1) }
        , { "ATCS", std::bind(&OBDLink::handle_ATCS, this, std::placeholders::_1) }
        , { "ATCSM", std::bind(&OBDLink::handle_ATCSM, this, std::placeholders::_1) }
        , { "ATCV", std::bind(&OBDLink::handle_ATCV, this, std::placeholders::_1) }
        , { "ATD", std::bind(&OBDLink::handle_ATD, this, std::placeholders::_1) }
        , { "ATDM1", std::bind(&OBDLink::handle_ATDM1, this, std::placeholders::_1) }
        , { "ATDP", std::bind(&OBDLink::handle_ATDP, this, std::placeholders::_1) }
        , { "ATDPN", std::bind(&OBDLink::handle_ATDPN, this, std::placeholders::_1) }
        , { "ATE", std::bind(&OBDLink::handle_ATE, this, std::placeholders::_1) }
        , { "ATFC", std::bind(&OBDLink::handle_ATFC, this, std::placeholders::_1) }
        , { "ATFE", std::bind(&OBDLink::handle_ATFE, this, std::placeholders::_1) }
        , { "ATFI", std::bind(&OBDLink::handle_ATFI, this, std::placeholders::_1) }
        , { "ATH", std::bind(&OBDLink::handle_ATH, this, std::placeholders::_1) }
        , { "ATI", std::bind(&OBDLink::handle_ATI, this, std::placeholders::_1) }
        , { "ATIB", std::bind(&OBDLink::handle_ATIB, this, std::placeholders::_1) }
        , { "ATIFR", std::bind(&OBDLink::handle_ATIFR, this, std::placeholders::_1) }
        , { "ATIGN", std::bind(&OBDLink::handle_ATIGN, this, std::placeholders::_1) }
        , { "ATIIA", std::bind(&OBDLink::handle_ATIIA, this, std::placeholders::_1) }
        , { "ATJE", std::bind(&OBDLink::handle_ATJE, this, std::placeholders::_1) }
        , { "ATJHF", std::bind(&OBDLink::handle_ATJHF, this, std::placeholders::_1) }
        , { "ATJS", std::bind(&OBDLink::handle_ATJS, this, std::placeholders::_1) }
        , { "ATJTM1", std::bind(&OBDLink::handle_ATJTM1, this, std::placeholders::_1) }
        , { "ATJTM5", std::bind(&OBDLink::handle_ATJTM5, this, std::placeholders::_1) }
        , { "ATKW", std::bind(&OBDLink::handle_ATKW, this, std::placeholders::_1) }
        , { "ATL", std::bind(&OBDLink::handle_ATL, this, std::placeholders::_1) }
        , { "ATLP", std::bind(&OBDLink::handle_ATLP, this, std::placeholders::_1) }
        , { "ATM", std::bind(&OBDLink::handle_ATM, this, std::placeholders::_1) }
        , { "ATMA", std::bind(&OBDLink::handle_ATMA, this, std::placeholders::_1) }
        , { "ATMP", std::bind(&OBDLink::handle_ATMP, this, std::placeholders::_1) }
        , { "ATMR", std::bind(&OBDLink::handle_ATMR, this, std::placeholders::_1) }
        , { "ATMT", std::bind(&OBDLink::handle_ATMT, this, std::placeholders::_1) }
        , { "ATNL", std::bind(&OBDLink::handle_ATNL, this, std::placeholders::_1) }
        , { "ATPB", std::bind(&OBDLink::handle_ATPB, this, std::placeholders::_1) }
        , { "ATPC", std::bind(&OBDLink::handle_ATPC, this, std::placeholders::_1) }
        , { "ATPP", std::bind(&OBDLink::handle_ATPP, this, std::placeholders::_1) }
        , { "ATPPS", std::bind(&OBDLink::handle_ATPPS, this, std::placeholders::_1) }
        , { "ATR", std::bind(&OBDLink::handle_ATR, this, std::placeholders::_1) }
        , { "ATRA", std::bind(&OBDLink::handle_ATRA, this, std::placeholders::_1) }
        , { "ATRD", std::bind(&OBDLink::handle_ATRD, this, std::placeholders::_1) }
        , { "ATRTR", std::bind(&OBDLink::handle_ATRTR, this, std::placeholders::_1) }
        , { "ATRV", std::bind(&OBDLink::handle_ATRV, this, std::placeholders::_1) }
        , { "ATS", std::bind(&OBDLink::handle_ATS, this, std::placeholders::_1) }
        , { "ATD", std::bind(&OBDLink::handle_ATSD, this, std::placeholders::_1) }
        , { "ATH", std::bind(&OBDLink::handle_ATSH, this, std::placeholders::_1) }
        , { "ATSI", std::bind(&OBDLink::handle_ATSI, this, std::placeholders::_1) }
        , { "ATSP", std::bind(&OBDLink::handle_ATSP, this, std::placeholders::_1) }
        , { "ATSR", std::bind(&OBDLink::handle_ATSR, this, std::placeholders::_1) }
        , { "ATSS", std::bind(&OBDLink::handle_ATSS, this, std::placeholders::_1) }
        , { "ATST", std::bind(&OBDLink::handle_ATST, this, std::placeholders::_1) }
        , { "ATSW", std::bind(&OBDLink::handle_ATSW, this, std::placeholders::_1) }
        , { "ATTA", std::bind(&OBDLink::handle_ATTA, this, std::placeholders::_1) }
        , { "ATTP", std::bind(&OBDLink::handle_ATTP, this, std::placeholders::_1) }
        , { "ATV", std::bind(&OBDLink::handle_ATV, this, std::placeholders::_1) }
        , { "ATWM", std::bind(&OBDLink::handle_ATWM, this, std::placeholders::_1) }
        , { "ATWS", std::bind(&OBDLink::handle_ATWS, this, std::placeholders::_1) }
        , { "ATZ", std::bind(&OBDLink::handle_ATZ, this, std::placeholders::_1) }
    };

    const transmit_on_can_bus_callback_t m_transmit_on_can_bus_callback = nullptr;
    const send_to_serial_callback_t m_send_to_serial_callback = nullptr;
    const protocol_select_callback_t m_protocol_select_callback = nullptr;

    size_t m_min_command_size;
    size_t m_max_command_size;

    std::unique_ptr<OBDLinkConfig> m_config;

    std::string m_request;
    std::string m_last_request;

};

#endif // OBDLINK_H
