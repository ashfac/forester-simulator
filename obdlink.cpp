#include "obdlink.h"

OBDLink::OBDLink(const transmit_on_can_bus_callback_t respond_can_callback,
                 const send_to_serial_callback_t respond_serial_callback,
                 const protocol_select_callback_t protocol_select_callback) :
    m_transmit_on_can_bus_callback(respond_can_callback)
  , m_send_to_serial_callback(respond_serial_callback)
  , m_protocol_select_callback(protocol_select_callback)
  , m_config(std::make_unique<OBDLinkConfig>())
{
    m_min_command_size = min_command_size();
    m_max_command_size = max_command_size();
}

void OBDLink::send(const std::string &data)
{
    echo(data);

    // don't copy over LF charachters
    std::copy_if(data.begin(), data.end(), std::back_inserter(m_request), [](char c) { return c != '\n'; } );

    size_t index;
    while ( (index = m_request.find_first_of("\r")) != std::string::npos ) {

        std::string request;

        if ( index == 0 ) {
            // CR: execute last request
            request = m_last_request;
        } else {
            request = m_request.substr(0, index);

            // convert request string to UPPERCASE charachters
            std::transform(request.begin(), request.end(), request.begin(),
                [](unsigned char c){ return std::toupper(c); });

            m_last_request = request;
        }

        // advance by the length of current request + size of CR charachter
        m_request = m_request.substr(index + 1);

        if ( is_command(request) ) {
           send_to_serial( handle_command(request) );
        } else {

        }

    }
}

void OBDLink::send(const can_types::can_msg_t &message)
{
    (void) message;
}

void OBDLink::echo(const std::string &data)
{
    if (m_config->get_echo()) {
        m_send_to_serial_callback(format_response(data));
    }
}

void OBDLink::send_to_serial(const std::string &response)
{
    if ( response != nsobdlink::response::pending ) {
        m_send_to_serial_callback(format_response(response + "\r>"));
    }
}

void OBDLink::transmit_on_can_bus(const can_types::can_msg_t &message)
{
    m_transmit_on_can_bus_callback(message);
}

std::string OBDLink::format_response(const std::string &str)
{
    std::string response = str;

    if (m_config->get_linefeeds()) {
        size_t index = 0;

        while ( (index = response.find('\r', index)) != std::string::npos ) {
            response.insert(++index, "\n");
        }
    }

    return response;
}

size_t OBDLink::min_command_size()
{
    size_t min_command_size = std::string::npos;

    for( auto &command : m_command_set ) {
        size_t command_size = command.first.length();

        if (command_size < min_command_size) {
            min_command_size = command_size;
        }
    }

    return min_command_size;
}

size_t OBDLink::max_command_size()
{
    size_t max_command_size = 0;

    for( auto &command : m_command_set ) {
        size_t command_size = command.first.length();

        if (command_size > max_command_size) {
            max_command_size = command_size;
        }
    }

    return max_command_size;
}

unsigned long OBDLink::hex_string_to_int(const std::string &str)
{
    if ( !str.empty() && (str.find_first_not_of("0123456789ABCDEF") == std::string::npos) ) {
        return std::stoul(str, 0, 16);
    }

    return -1;
}

std::vector<std::string> OBDLink::get_arguments(const std::string& str)
{
    std::vector<std::string> args;
    std::istringstream iss(str);

    for (std::string s; iss >> s; ) {
        args.push_back(s);
    }

    return args;
}

int OBDLink::get_integer_argument(const std::vector<std::string> &args)
{
    if ( args.size() == 1 ) {
        return hex_string_to_int(args.at(0));
    }

    return -1;
}

std::string OBDLink::set_on_off_config(const nsobdlink::config::boolean_config_t config_item, const std::vector<std::string> &args)
{
    int arg = get_integer_argument(args);

    if ( arg == 0 || arg == 1 ) {
        set_on_off_config(config_item, static_cast<bool>(arg));
        return nsobdlink::response::ok;
    }

    return nsobdlink::response::invalid;
}

void OBDLink::set_on_off_config(const nsobdlink::config::boolean_config_t config_item, const bool value)
{
    switch (config_item) {
    case nsobdlink::config::can_auto_formatting: m_config->set_can_auto_formatting(value); break;

    case nsobdlink::config::can_flow_control: m_config->set_can_flow_control(value); break;

    case nsobdlink::config::can_silent_mode: m_config->set_can_silent_mode(value); break;

    case nsobdlink::config::dlc: m_config->set_dlc(value); break;

    case nsobdlink::config::echo: m_config->set_echo(value); break;

    case nsobdlink::config::headers: m_config->set_headers(value); break;

    case nsobdlink::config::j1939_header_formatting: m_config->set_j1939_header_formatting(value); break;

    case nsobdlink::config::keyword_checking: m_config->set_keyword_checking(value); break;

    case nsobdlink::config::linefeeds: m_config->set_linefeeds(value); break;

    case nsobdlink::config::memory: m_config->set_memory(value); break;

    case nsobdlink::config::responses: m_config->set_responses(value); break;

    case nsobdlink::config::spaces: m_config->set_spaces(value); break;

    case nsobdlink::config::variable_dlc: m_config->set_variable_dlc(value); break;
    }
}

bool OBDLink::is_command(const std::string &request)
{
    if ( request.find("AT") != std::string::npos ||
         request.find("ST") != std::string::npos ) {
        return true;
    }

    return false;
}

std::string OBDLink::handle_command(const std::string &command)
{
    std::string response = nsobdlink::response::invalid;

    if ( command.length() >= m_min_command_size ) {

        size_t command_size;
        std::string command;

        // if the command and arguments are already separated by SPACE, pick command
        if ( (command_size = command.find_first_of(" ")) != std::string::npos ) {
            command = command.substr(0, command_size);
            response = handle_command(command, command.substr(command_size));

        } else {
            for (command_size = std::min(m_max_command_size, command.size()); command_size >= m_min_command_size; --command_size) {
                command = command.substr(0, command_size);
                response = handle_command(command, command.substr(command_size));

                // if the command has been handled, exit loop
                if (response != nsobdlink::response::invalid) {
                    break;
                }
            }
        }
    }

    return response;
}

std::string OBDLink::handle_command(const std::string &command, const std::string &args)
{
    try {
        const command_handler_t command_handler = m_command_set.at(command);
        return command_handler(get_arguments(args));
    } catch ( const std::out_of_range& oor) { }

    return nsobdlink::response::invalid;
}

void OBDLink::handle_can_message(const std::string &request)
{
    can_types::can_msg_t message;

    transmit_on_can_bus(message);
}

std::string OBDLink::handle_AT_at(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATAL(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATAR(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATAT(const std::vector<std::string> &args)
{
    int timing = get_integer_argument(args);

    if ( timing >= 0 && timing <= 2 ) {
        m_config->set_adaptive_timing(static_cast<nsobdlink::config::adaptive_timing_t>(timing));
        return nsobdlink::response::ok;
    }

    return nsobdlink::response::invalid;
}

std::string OBDLink::handle_ATBD(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATBI(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATBRD(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATBRT(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATCAF(const std::vector<std::string> &args)
{
    return set_on_off_config(nsobdlink::config::can_auto_formatting, args);
}

std::string OBDLink::handle_ATCEA(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATCF(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATCFC(const std::vector<std::string> &args)
{
    return set_on_off_config(nsobdlink::config::can_flow_control, args);
}

std::string OBDLink::handle_ATCM(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATCP(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATCRA(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATCS(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATCSM(const std::vector<std::string> &args)
{
    return set_on_off_config(nsobdlink::config::can_silent_mode, args);
}

std::string OBDLink::handle_ATCV(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATD(const std::vector<std::string> &args)
{
    if ( args.size() == 1 ) {
        return set_on_off_config(nsobdlink::config::dlc, args);

    } else if ( args.size() == 0 ) {
        m_config->load_defaults();
        return handle_ATI(args);
    }

    return nsobdlink::response::invalid;
}

std::string OBDLink::handle_ATDM1(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATDP(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATDPN(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATE(const std::vector<std::string> &args)
{
    return set_on_off_config(nsobdlink::config::echo, args);
}

std::string OBDLink::handle_ATFC(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATFE(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATFI(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATH(const std::vector<std::string> &args)
{
    return set_on_off_config(nsobdlink::config::headers, args);
}

std::string OBDLink::handle_ATI(const std::vector<std::string> &args)
{
    if ( args.size() == 0 ) {
        return nsobdlink::versions::elm_device_id;
    }

    return nsobdlink::response::invalid;
}

std::string OBDLink::handle_ATIB(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATIFR(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATIGN(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATIIA(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATJE(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATJHF(const std::vector<std::string> &args)
{
    return set_on_off_config(nsobdlink::config::j1939_header_formatting, args);
}

std::string OBDLink::handle_ATJS(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATJTM1(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATJTM5(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATKW(const std::vector<std::string> &args)
{
    return set_on_off_config(nsobdlink::config::keyword_checking, args);
}

std::string OBDLink::handle_ATL(const std::vector<std::string> &args)
{
    return set_on_off_config(nsobdlink::config::linefeeds, args);
}

std::string OBDLink::handle_ATLP(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATM(const std::vector<std::string> &args)
{
    return set_on_off_config(nsobdlink::config::memory, args);
}

std::string OBDLink::handle_ATMA(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATMP(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATMR(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATMT(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATNL(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATPB(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATPC(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATPP(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATPPS(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATR(const std::vector<std::string> &args)
{
    return set_on_off_config(nsobdlink::config::responses, args);
}

std::string OBDLink::handle_ATRA(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATRD(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATRTR(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATRV(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATS(const std::vector<std::string> &args)
{
    return set_on_off_config(nsobdlink::config::spaces, args);
}

std::string OBDLink::handle_ATSD(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATSH(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATSI(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATSP(const std::vector<std::string> &args)
{
    if ( m_config->set_obd_protocol(static_cast<nsobdlink::config::obd_protocol_t>(get_integer_argument(args))) ) {
        return nsobdlink::response::ok;
    }

    return nsobdlink::response::invalid;
}

std::string OBDLink::handle_ATSR(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATSS(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATST(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATSW(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATTA(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATTP(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATV(const std::vector<std::string> &args)
{
    return set_on_off_config(nsobdlink::config::variable_dlc, args);
}

std::string OBDLink::handle_ATWM(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATWS(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATZ(const std::vector<std::string> &args)
{
    return handle_ATD(args);
}

std::string OBDLink::command_not_implemented(const std::vector<std::string> &args)
{
    (void) args;
    return nsobdlink::response::invalid;
}
