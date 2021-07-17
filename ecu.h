#ifndef ECU_H
#define ECU_H

#include <map>

#include "can.h"
#include "obd.h"
#include "canbus.h"

class CanBus;

class ECU : public std::enable_shared_from_this<ECU>
{
public:
    ECU();

    virtual void put(const can::protocol_t protocol, const can::msg_t &message) = 0;

protected:
    void transmit(const can::header_t header, const can::data_t &data) const;

    void set_obd_request_header(can::header_t header) { m_obd_request_header = header; }
    can::header_t get_obd_request_header() const { return m_obd_request_header; }
    can::header_t get_obd_response_header() const { return (m_obd_request_header | obd::response_header_mask); }

    can::data_t get_obd_pid(can::data_t &request) const;
    static obd::mode_t get_obd_mode(const can::data_t &request);
    static size_t get_obd_pid_request_size(const obd::mode_t mode);

protected:
    std::shared_ptr<CanBus> m_can_bus;
    can::protocol_t m_protocol;
    can::header_t m_obd_request_header;

    std::map<obd::pid_t, can::data_t> m_pids;
};

#endif // ECU_H
