#include "obdlink.h"

OBDLink::OBDLink(const transmit_callback_t respond_can_callback,
                 const respond_callback_t respond_serial_callback,
                 const protocol_select_callback_t protocol_select_callback) :
    Elm327(respond_can_callback, respond_serial_callback, protocol_select_callback)
{
}

bool OBDLink::is_command(const std::string &request) const
{
    if ( request.find("AT") != std::string::npos ||
         request.find("ST") != std::string::npos ) {
        return true;
    }

    return false;
}

