#include "elm327.h"

Elm327::Elm327(const transmit_on_can_bus_callback_t respond_can_callback,
               const send_to_serial_callback_t respond_serial_callback,
               const protocol_select_callback_t protocol_select_callback) :
    m_transmit_on_can_bus_callback(respond_can_callback)
  , m_send_to_serial_callback(respond_serial_callback)
  , m_protocol_select_callback(protocol_select_callback)
  , m_config(std::make_unique<Elm327Config>())
{
    m_min_command_size = min_command_size();
    m_max_command_size = max_command_size();
}

void Elm327::send(const std::string &data)
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

void Elm327::send(const can::can_msg_t &message)
{
    (void) message;
}

void Elm327::echo(const std::string &data)
{
    if (m_config->get_echo()) {
        m_send_to_serial_callback(format_response(data));
    }
}

void Elm327::send_to_serial(const std::string &response)
{
    if ( response != elm::response_pending ) {
        m_send_to_serial_callback(format_response(response + "\r>"));
    }
}

void Elm327::transmit_on_can_bus(const can::can_msg_t &message)
{
    m_transmit_on_can_bus_callback(message);
}

std::string Elm327::format_response(const std::string &str)
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

size_t Elm327::min_command_size()
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

size_t Elm327::max_command_size()
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

unsigned long Elm327::hex_string_to_int(const std::string &str)
{
    if ( !str.empty() && (str.find_first_not_of("0123456789ABCDEF") == std::string::npos) ) {
        return std::stoul(str, 0, 16);
    }

    return -1;
}

std::vector<std::string> Elm327::get_arguments(const std::string& str)
{
    std::vector<std::string> args;
    std::istringstream iss(str);

    for (std::string s; iss >> s; ) {
        args.push_back(s);
    }

    return args;
}

int Elm327::get_integer_argument(const std::vector<std::string> &args)
{
    if ( args.size() == 1 ) {
        return hex_string_to_int(args.at(0));
    }

    return -1;
}

std::string Elm327::set_config(const elm::config::config_t config_item, const std::vector<std::string> &args)
{
    int arg = get_integer_argument(args);

    if ( arg == 0 || arg == 1 ) {
        set_config(config_item, static_cast<bool>(arg));
        return elm::response_ok;
    }

    return elm::response_invalid;
}

void Elm327::set_config(const elm::config::config_t config_item, const bool value)
{
    switch (config_item) {
    case elm::config::can_auto_formatting: m_config->set_can_auto_formatting(value); break;

    case elm::config::can_flow_control: m_config->set_can_flow_control(value); break;

    case elm::config::can_silent_mode: m_config->set_can_silent_mode(value); break;

    case elm::config::dlc: m_config->set_dlc(value); break;

    case elm::config::echo: m_config->set_echo(value); break;

    case elm::config::headers: m_config->set_headers(value); break;

    case elm::config::j1939_header_formatting: m_config->set_j1939_header_formatting(value); break;

    case elm::config::keyword_checking: m_config->set_keyword_checking(value); break;

    case elm::config::linefeeds: m_config->set_linefeeds(value); break;

    case elm::config::memory: m_config->set_memory(value); break;

    case elm::config::responses: m_config->set_responses(value); break;

    case elm::config::spaces: m_config->set_spaces(value); break;

    case elm::config::variable_dlc: m_config->set_variable_dlc(value); break;
    }
}

bool Elm327::is_command(const std::string &request)
{
    if ( request.find("AT") != std::string::npos ||
         request.find("ST") != std::string::npos ) {
        return true;
    }

    return false;
}

std::string Elm327::handle_command(const std::string &request)
{
    std::string response = elm::response_invalid;

    if ( request.length() >= m_min_command_size ) {

        size_t command_size;
        std::string command;

        // if the command and arguments are already separated by SPACE, pick command
        if ( (command_size = request.find_first_of(" ")) != std::string::npos ) {
            command = request.substr(0, command_size);
            response = handle_command(command, request.substr(command_size));

        } else {
            for (command_size = std::min(m_max_command_size, request.size()); command_size >= m_min_command_size; --command_size) {
                command = request.substr(0, command_size);
                response = handle_command(command, request.substr(command_size));

                // if the command has been handled, exit loop
                if (response != elm::response_invalid) {
                    break;
                }
            }
        }
    }

    return response;
}

std::string Elm327::handle_command(const std::string &command, const std::string &args)
{
    try {
        const command_handler_t command_handler = m_command_set.at(command);
        return command_handler(get_arguments(args));
    } catch ( const std::out_of_range& oor) { }

    return elm::response_invalid;
}

void Elm327::handle_can_message(const std::string &request)
{
    can::can_msg_t message;

    transmit_on_can_bus(message);
}

std::string Elm327::handle_AT_at(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATAL(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATAR(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATAT(const std::vector<std::string> &args)
{
    int timing = get_integer_argument(args);

    if ( timing >= 0 && timing <= 2 ) {
        m_config->set_adaptive_timing(static_cast<elm::config::adaptive_timing_t>(timing));
        return elm::response_ok;
    }

    return elm::response_invalid;
}

std::string Elm327::handle_ATBD(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATBI(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATBRD(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATBRT(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATCAF(const std::vector<std::string> &args)
{
    return set_config(elm::config::can_auto_formatting, args);
}

std::string Elm327::handle_ATCEA(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATCF(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATCFC(const std::vector<std::string> &args)
{
    return set_config(elm::config::can_flow_control, args);
}

std::string Elm327::handle_ATCM(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATCP(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATCRA(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATCS(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATCSM(const std::vector<std::string> &args)
{
    return set_config(elm::config::can_silent_mode, args);
}

std::string Elm327::handle_ATCV(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATD(const std::vector<std::string> &args)
{
    if ( args.size() == 1 ) {
        return set_config(elm::config::dlc, args);

    } else if ( args.size() == 0 ) {
        m_config->load_defaults();
        return handle_ATI(args);
    }

    return elm::response_invalid;
}

std::string Elm327::handle_ATDM1(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATDP(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATDPN(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATE(const std::vector<std::string> &args)
{
    return set_config(elm::config::echo, args);
}

std::string Elm327::handle_ATFC(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATFE(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATFI(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATH(const std::vector<std::string> &args)
{
    return set_config(elm::config::headers, args);
}

std::string Elm327::handle_ATI(const std::vector<std::string> &args)
{
    if ( args.size() == 0 ) {
        return elm::device_id;
    }

    return elm::response_invalid;
}

std::string Elm327::handle_ATIB(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATIFR(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATIGN(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATIIA(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATJE(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATJHF(const std::vector<std::string> &args)
{
    return set_config(elm::config::j1939_header_formatting, args);
}

std::string Elm327::handle_ATJS(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATJTM1(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATJTM5(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATKW(const std::vector<std::string> &args)
{
    return set_config(elm::config::keyword_checking, args);
}

std::string Elm327::handle_ATL(const std::vector<std::string> &args)
{
    return set_config(elm::config::linefeeds, args);
}

std::string Elm327::handle_ATLP(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATM(const std::vector<std::string> &args)
{
    return set_config(elm::config::memory, args);
}

std::string Elm327::handle_ATMA(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATMP(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATMR(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATMT(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATNL(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATPB(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATPC(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATPP(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATPPS(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATR(const std::vector<std::string> &args)
{
    return set_config(elm::config::responses, args);
}

std::string Elm327::handle_ATRA(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATRD(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATRTR(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATRV(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATS(const std::vector<std::string> &args)
{
    return set_config(elm::config::spaces, args);
}

std::string Elm327::handle_ATSD(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATSH(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATSI(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATSP(const std::vector<std::string> &args)
{
    if ( m_config->set_obd_protocol(static_cast<elm::obd::protocol_t>(get_integer_argument(args))) ) {
        return elm::response_ok;
    }

    return elm::response_invalid;
}

std::string Elm327::handle_ATSR(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATSS(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATST(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATSW(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATTA(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATTP(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATV(const std::vector<std::string> &args)
{
    return set_config(elm::config::variable_dlc, args);
}

std::string Elm327::handle_ATWM(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATWS(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string Elm327::handle_ATZ(const std::vector<std::string> &args)
{
    return handle_ATD(args);
}

std::string Elm327::command_not_implemented(const std::vector<std::string> &args)
{
    (void) args;
    return elm::response_invalid;
}
