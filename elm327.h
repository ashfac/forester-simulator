#ifndef ELM327_H
#define ELM327_H

#include <string>
#include <sstream>
#include <functional>
#include <vector>
#include <map>

#include "elm.h"
#include "elm327config.h"
#include "canbus.h"

class Elm327
{
public:
    using transmit_callback_t = std::function<void(const can::can_msg_t&)>;
    using respond_callback_t = std::function<void(const std::string&)>;
    using protocol_select_callback_t = std::function<void(const elm::obd::protocol_t)>;

private:
    using command_handler_t = std::function<std::string(const std::vector<std::string> &)>;
    using command_set_t = std::map<std::string, command_handler_t>;

public:
    Elm327(const transmit_callback_t transmit_callback,
           const respond_callback_t respond_callback,
           const protocol_select_callback_t protocol_select_callback);

    void request(const std::string &data);
    void put(const can::can_msg_t &message);

private:
    void echo(const std::string &data);
    void respond(const std::string &response);
    void transmit(const can::can_msg_t &message);

    bool is_command(const std::string &request) const;
    std::string handle_command(const std::string &request);
    std::string handle_command(const std::string &command, const std::string &args);
    std::string handle_obd_request(const std::string &request);

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

    void reset_device();
    void load_defaults();
    void set_can_bus_mode(can::mode_t mode);

    std::string set_config(const elm::config::config_t config_item, const std::vector<std::string> &args);
    void set_config(const elm::config::config_t config_item, const bool value);
    bool set_obd_protocol(const elm::obd::protocol_t obd_protocol);

    size_t min_command_size();
    size_t max_command_size();

    std::string compose_serial_message(const std::string &str);

    static std::vector<std::string> split_string(const std::string& str);
    static int get_integer_argument(const std::vector<std::string> &args);
    static int hex_string_to_int(const std::string &str);

private:
    const command_set_t m_command_set = {
          { "AT@", std::bind(&Elm327::handle_AT_at, this, std::placeholders::_1) }
        , { "ATAL", std::bind(&Elm327::handle_ATAL, this, std::placeholders::_1) }
        , { "ATAR", std::bind(&Elm327::handle_ATAR, this, std::placeholders::_1) }
        , { "ATAT", std::bind(&Elm327::handle_ATAT, this, std::placeholders::_1) }
        , { "ATBD", std::bind(&Elm327::handle_ATBD, this, std::placeholders::_1) }
        , { "ATBI", std::bind(&Elm327::handle_ATBI, this, std::placeholders::_1) }
        , { "ATBRD", std::bind(&Elm327::handle_ATBRD, this, std::placeholders::_1) }
        , { "ATBRT", std::bind(&Elm327::handle_ATBRT, this, std::placeholders::_1) }
        , { "ATCAF", std::bind(&Elm327::handle_ATCAF, this, std::placeholders::_1) }
        , { "ATCEA", std::bind(&Elm327::handle_ATCEA, this, std::placeholders::_1) }
        , { "ATCF", std::bind(&Elm327::handle_ATCF, this, std::placeholders::_1) }
        , { "ATCFC", std::bind(&Elm327::handle_ATCFC, this, std::placeholders::_1) }
        , { "ATCM", std::bind(&Elm327::handle_ATCM, this, std::placeholders::_1) }
        , { "ATCP", std::bind(&Elm327::handle_ATCP, this, std::placeholders::_1) }
        , { "ATCRA", std::bind(&Elm327::handle_ATCRA, this, std::placeholders::_1) }
        , { "ATCS", std::bind(&Elm327::handle_ATCS, this, std::placeholders::_1) }
        , { "ATCSM", std::bind(&Elm327::handle_ATCSM, this, std::placeholders::_1) }
        , { "ATCV", std::bind(&Elm327::handle_ATCV, this, std::placeholders::_1) }
        , { "ATD", std::bind(&Elm327::handle_ATD, this, std::placeholders::_1) }
        , { "ATDM1", std::bind(&Elm327::handle_ATDM1, this, std::placeholders::_1) }
        , { "ATDP", std::bind(&Elm327::handle_ATDP, this, std::placeholders::_1) }
        , { "ATDPN", std::bind(&Elm327::handle_ATDPN, this, std::placeholders::_1) }
        , { "ATE", std::bind(&Elm327::handle_ATE, this, std::placeholders::_1) }
        , { "ATFC", std::bind(&Elm327::handle_ATFC, this, std::placeholders::_1) }
        , { "ATFE", std::bind(&Elm327::handle_ATFE, this, std::placeholders::_1) }
        , { "ATFI", std::bind(&Elm327::handle_ATFI, this, std::placeholders::_1) }
        , { "ATH", std::bind(&Elm327::handle_ATH, this, std::placeholders::_1) }
        , { "ATI", std::bind(&Elm327::handle_ATI, this, std::placeholders::_1) }
        , { "ATIB", std::bind(&Elm327::handle_ATIB, this, std::placeholders::_1) }
        , { "ATIFR", std::bind(&Elm327::handle_ATIFR, this, std::placeholders::_1) }
        , { "ATIGN", std::bind(&Elm327::handle_ATIGN, this, std::placeholders::_1) }
        , { "ATIIA", std::bind(&Elm327::handle_ATIIA, this, std::placeholders::_1) }
        , { "ATJE", std::bind(&Elm327::handle_ATJE, this, std::placeholders::_1) }
        , { "ATJHF", std::bind(&Elm327::handle_ATJHF, this, std::placeholders::_1) }
        , { "ATJS", std::bind(&Elm327::handle_ATJS, this, std::placeholders::_1) }
        , { "ATJTM1", std::bind(&Elm327::handle_ATJTM1, this, std::placeholders::_1) }
        , { "ATJTM5", std::bind(&Elm327::handle_ATJTM5, this, std::placeholders::_1) }
        , { "ATKW", std::bind(&Elm327::handle_ATKW, this, std::placeholders::_1) }
        , { "ATL", std::bind(&Elm327::handle_ATL, this, std::placeholders::_1) }
        , { "ATLP", std::bind(&Elm327::handle_ATLP, this, std::placeholders::_1) }
        , { "ATM", std::bind(&Elm327::handle_ATM, this, std::placeholders::_1) }
        , { "ATMA", std::bind(&Elm327::handle_ATMA, this, std::placeholders::_1) }
        , { "ATMP", std::bind(&Elm327::handle_ATMP, this, std::placeholders::_1) }
        , { "ATMR", std::bind(&Elm327::handle_ATMR, this, std::placeholders::_1) }
        , { "ATMT", std::bind(&Elm327::handle_ATMT, this, std::placeholders::_1) }
        , { "ATNL", std::bind(&Elm327::handle_ATNL, this, std::placeholders::_1) }
        , { "ATPB", std::bind(&Elm327::handle_ATPB, this, std::placeholders::_1) }
        , { "ATPC", std::bind(&Elm327::handle_ATPC, this, std::placeholders::_1) }
        , { "ATPP", std::bind(&Elm327::handle_ATPP, this, std::placeholders::_1) }
        , { "ATPPS", std::bind(&Elm327::handle_ATPPS, this, std::placeholders::_1) }
        , { "ATR", std::bind(&Elm327::handle_ATR, this, std::placeholders::_1) }
        , { "ATRA", std::bind(&Elm327::handle_ATRA, this, std::placeholders::_1) }
        , { "ATRD", std::bind(&Elm327::handle_ATRD, this, std::placeholders::_1) }
        , { "ATRTR", std::bind(&Elm327::handle_ATRTR, this, std::placeholders::_1) }
        , { "ATRV", std::bind(&Elm327::handle_ATRV, this, std::placeholders::_1) }
        , { "ATS", std::bind(&Elm327::handle_ATS, this, std::placeholders::_1) }
        , { "ATD", std::bind(&Elm327::handle_ATSD, this, std::placeholders::_1) }
        , { "ATH", std::bind(&Elm327::handle_ATSH, this, std::placeholders::_1) }
        , { "ATSI", std::bind(&Elm327::handle_ATSI, this, std::placeholders::_1) }
        , { "ATSP", std::bind(&Elm327::handle_ATSP, this, std::placeholders::_1) }
        , { "ATSR", std::bind(&Elm327::handle_ATSR, this, std::placeholders::_1) }
        , { "ATSS", std::bind(&Elm327::handle_ATSS, this, std::placeholders::_1) }
        , { "ATST", std::bind(&Elm327::handle_ATST, this, std::placeholders::_1) }
        , { "ATSW", std::bind(&Elm327::handle_ATSW, this, std::placeholders::_1) }
        , { "ATTA", std::bind(&Elm327::handle_ATTA, this, std::placeholders::_1) }
        , { "ATTP", std::bind(&Elm327::handle_ATTP, this, std::placeholders::_1) }
        , { "ATV", std::bind(&Elm327::handle_ATV, this, std::placeholders::_1) }
        , { "ATWM", std::bind(&Elm327::handle_ATWM, this, std::placeholders::_1) }
        , { "ATWS", std::bind(&Elm327::handle_ATWS, this, std::placeholders::_1) }
        , { "ATZ", std::bind(&Elm327::handle_ATZ, this, std::placeholders::_1) }
    };

    const transmit_callback_t m_transmit_callback = nullptr;
    const respond_callback_t m_respond_callback = nullptr;
    const protocol_select_callback_t m_protocol_select_callback = nullptr;

    size_t m_min_command_size;
    size_t m_max_command_size;

    std::unique_ptr<Elm327Config> m_config;

    std::string m_request;
    std::string m_last_request;

    can::mode_t m_can_bus_mode;
};

#endif // ELM327_H
