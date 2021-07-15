#ifndef OBDLINK_H
#define OBDLINK_H

#include "elm327.h"

class OBDLink : public Elm327
{
public:
    OBDLink(const transmit_on_can_bus_callback_t respond_can_callback,
            const send_to_serial_callback_t respond_serial_callback,
            const protocol_select_callback_t protocol_select_callback);
};

#endif // OBDLINK_H
