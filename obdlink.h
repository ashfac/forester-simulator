#ifndef OBDLINK_H
#define OBDLINK_H

#include "elm327.h"

class OBDLink : public Elm327
{
public:
    OBDLink(const transmit_callback_t respond_can_callback,
            const respond_callback_t respond_serial_callback,
            const protocol_select_callback_t protocol_select_callback);

    bool is_command(const std::string &request) const;

};

#endif // OBDLINK_H
