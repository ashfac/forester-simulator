#include "elm327.h"

Elm327::Elm327( const transmit_callback_t transmit_callback,
                const respond_callback_t respond_callback,
                const protocol_select_callback_t protocol_select_callback ) :
    m_transmit_callback( transmit_callback )
  , m_respond_callback( respond_callback )
  , m_protocol_select_callback( protocol_select_callback )
  , m_config( std::make_unique<Elm327Config>() )
  , m_can_bus_mode( can::mode::disconnected )
{
    m_min_command_size = min_command_size();
    m_max_command_size = max_command_size();
}

void Elm327::request( const std::string &data )
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
            respond( handle_command(request) );
        } else {
            respond( handle_obd_request(request) );
        }
    }
}

void Elm327::put(const can::can_msg_t &message)
{
    (void) message;
}

void Elm327::echo(const std::string &data)
{
    if (m_config->get_echo()) {
        m_respond_callback(compose_serial_message(data));
    }
}

void Elm327::respond(const std::string &response)
{
    if ( response != elm::response::pending ) {
        m_respond_callback(compose_serial_message(response + "\r>"));
    }
}

void Elm327::transmit(const can::can_msg_t &message)
{
    if ( m_can_bus_mode == can::mode::connected ) {
        m_transmit_callback(message);
    }
}

bool Elm327::is_command(const std::string &request) const
{
    if ( request.find("AT") == 0 ) {
        return true;
    }

    return false;
}

std::string Elm327::handle_command(const std::string &request)
{
    if ( request.length() >= m_min_command_size ) {

        std::string command;

        // remove spaces betwen AT and the command, for example, "AT I" becomes "ATI"
        if ( request.find_first_of(' ') == 2 ) {
            size_t index = request.find_first_not_of(' ', 2);

            if ( index != std::string::npos ) {
                std::copy_if(request.begin(), request.begin() + index, std::back_inserter(command), [](char c) { return c != ' '; } );
                command.append(request, index, std::string::npos);
            }
        } else {
            command = request;
        }

        size_t command_size = command.find_first_of(' ');

        // if the command and arguments are already separated by SPACE, select command
        if ( command_size != std::string::npos ) {
            return (handle_command(command.substr(0, command_size), command.substr(command_size)));

        } else {
            for (command_size = std::min(m_max_command_size, command.size()); command_size >= m_min_command_size; --command_size) {

                std::string response = handle_command(command.substr(0, command_size), command.substr(command_size));

                if (response != elm::response::invalid) {
                    return response;
                }
            }
        }
    }

    return elm::response::invalid;
}

std::string Elm327::handle_command(const std::string &command, const std::string &args)
{
    try {
        const command_handler_t command_handler = m_command_set.at(command);
        return command_handler(split_string(args));
    } catch ( const std::out_of_range& oor) { }

    return elm::response::invalid;
}

std::string Elm327::handle_obd_request(const std::string &request)
{
    std::string req;
    // remove spaces from request
    std::copy_if(request.begin(), request.end(), std::back_inserter(req), [](char c) { return c != ' '; } );

    if ( ( req.length() > 0 ) && ( req.length() % 2 ) ) {
        req.resize(req.length() - 1);
    }

    if ( req.length() < 2 ) {
        return  elm::response::invalid;
    }

    // convert all bytes to a vector
    can::can_data_t data_bytes;

    for ( size_t i = 0; i < req.length(); i += 2 ) {
        int data_byte = hex_string_to_int( req.substr(i, 2) );

        if ( data_byte < 0 ) {
            return  elm::response::invalid;
        }

        data_bytes.push_back( static_cast<std::byte>(data_byte) );
    }

    // create individual CAN messages and transmit on CAN bus
    can::can_id_t msg_header = m_config->get_obd_request_header();
    can::can_data_t msg_data (8);

    can::can_data_t::iterator begin = data_bytes.begin();
    size_t remaining;

    while ( (remaining = data_bytes.end() - begin) > 0 ) {
        size_t len = ( remaining < 7) ? remaining : 7;

        msg_data[0] = static_cast<can::can_byte_t>( len );
        std::copy( begin, begin + len, msg_data.begin() + 1 );
        std::fill( msg_data.begin() + len, msg_data.end(), static_cast<can::can_byte_t>( 0 ) );

        transmit(std::make_pair(msg_header, msg_data));

        begin += len;
    }

    return elm::response::pending;
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
        return elm::response::ok;
    }

    return elm::response::invalid;
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
        load_defaults();
        return elm::device_id;
    }

    return elm::response::invalid;
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

    return elm::response::invalid;
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
    bool response = set_obd_protocol( static_cast<elm::obd::protocol_t>(get_integer_argument(args)) );

    return ( ( response ) ? ( elm::response::ok ) : ( elm::response::invalid ) );
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
    if ( args.size() == 0 ) {
        reset_device();
        return elm::response::ok;
    }

    return elm::response::invalid;
}

std::string Elm327::command_not_implemented(const std::vector<std::string> &args)
{
    (void) args;
    return elm::response::invalid;
}

void Elm327::reset_device()
{
    load_defaults();
}

void Elm327::load_defaults()
{
    set_can_bus_mode( can::mode::disconnected );
    set_obd_protocol( elm::obd::protocol::unknown );
    m_config->load_defaults();
}

void Elm327::set_can_bus_mode(can::mode_t mode)
{
    m_can_bus_mode = mode;
}

std::string Elm327::set_config(const elm::config::config_t config_item, const std::vector<std::string> &args)
{
    int arg = get_integer_argument(args);

    if ( arg == 0 || arg == 1 ) {
        set_config(config_item, static_cast<bool>(arg));
        return elm::response::ok;
    }

    return elm::response::invalid;
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

bool Elm327::set_obd_protocol(const elm::obd::protocol_t obd_protocol)
{
    if ( m_config->set_obd_protocol( obd_protocol )) {
        set_can_bus_mode( (obd_protocol == elm::obd::protocol::unknown) ? can::mode::disconnected : can::mode::connected );
        m_protocol_select_callback( obd_protocol );
        return true;
    }

    return false;
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

std::string Elm327::compose_serial_message(const std::string &str)
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

std::vector<std::string> Elm327::split_string(const std::string& str)
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

int Elm327::hex_string_to_int(const std::string &str)
{
    if ( !str.empty() && (str.find_first_not_of("0123456789ABCDEF") == std::string::npos) ) {
        return std::stoi(str, 0, 16);
    }

    return -1;
}

