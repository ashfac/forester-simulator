#include "obdlink.h"

OBDLink::OBDLink(const respond_can_callback_t respond_can_callback, const respond_serial_callback_t respond_serial_callback) :
    m_respond_can_callback(respond_can_callback)
  , m_respond_serial_callback(respond_serial_callback)
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
        std::string response = "?";

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
                    if (response != "?") {
                        break;
                    }
                }
            }
        }

        respond(response);
    }
}

void OBDLink::send(const can_types::can_msg_t &message)
{
    (void) message;
}

void OBDLink::echo(const std::string &data)
{
    if (m_config->get_echo()) {
        m_respond_serial_callback(data);
    }
}

void OBDLink::respond(const std::string &response)
{
    if ( response != nsobdlink::response_t::pending ) {
        m_respond_serial_callback(response + "\r\r>");
    }
}

void OBDLink::respond(const can_types::can_msg_t &message)
{
    (void) message;
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

int OBDLink::string_to_number(const std::string &str)
{
    if ( !str.empty() && (str.find_first_not_of("0123456789ABCDEF") == std::string::npos) ) {
        return std::stoi(str);
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
        return string_to_number(args.at(0));
    }

    return -1;
}

nsobdlink::on_off_t OBDLink::get_on_off_argument(const std::vector<std::string> &args)
{
    switch ( get_integer_argument(args) ) {
        case 0: return nsobdlink::on_off_t::off;
        case 1: return nsobdlink::on_off_t::on;
        default: return nsobdlink::on_off_t::invalid;
    }
}

std::string OBDLink::command_not_implemented(const std::vector<std::string> &args)
{
    (void) args;
    return "?";
}

std::string OBDLink::handle_command(const std::string &command, const std::string &args)
{
    try {
        const command_handler_t command_handler = m_command_set.at(command);
        return command_handler(get_arguments(args));
    } catch ( const std::out_of_range& oor) { }

    return "?";
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
    return command_not_implemented(args);
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
    return command_not_implemented(args);
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
    return command_not_implemented(args);
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
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATCV(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATD(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
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
    nsobdlink::on_off_t arg;

    if ( ( arg = get_on_off_argument(args) ) != nsobdlink::on_off_t::invalid ) {
        m_config->set_echo(arg);
        return nsobdlink::response_t::ok;
    }

    return nsobdlink::response_t::invalid;
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
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATI(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
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
    return command_not_implemented(args);
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
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATL(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATLP(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
}

std::string OBDLink::handle_ATM(const std::vector<std::string> &args)
{
    return command_not_implemented(args);
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
    return command_not_implemented(args);
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
    return command_not_implemented(args);
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
    return command_not_implemented(args);
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
    return command_not_implemented(args);
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
    if ( args.size() == 0 ) {
        m_config->load_defaults();
        return nsobdlink::versions::elm_device_id;
    }

    return nsobdlink::response_t::invalid;
}
