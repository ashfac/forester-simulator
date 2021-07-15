#include "obdlink.h"

OBDLink::OBDLink(const transmit_on_can_bus_callback_t respond_can_callback,
                 const send_to_serial_callback_t respond_serial_callback,
                 const protocol_select_callback_t protocol_select_callback) :
    Elm327(respond_can_callback, respond_serial_callback, protocol_select_callback)
{
}
