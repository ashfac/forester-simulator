#ifndef ECM_H
#define ECM_H

#include <vector>

#include "ecu.h"
#include "subaru.h"

class ECM : public ECU
{
public:
    ECM(const std::shared_ptr<CanBus> can_bus);

    void put(const can::protocol_t protocol, const can::msg_t &message);

private:
    void init_obd_pids();
    void init_obd_mode_01_02_pids();

    static can::byte_t construct_obd_pid_byte(obd::pid_t pid_1, obd::pid_t pid_2 = 0, obd::pid_t pid_3 = 0, obd::pid_t pid_4 = 0,
                                              obd::pid_t pid_5 = 0, obd::pid_t pid_6 = 0, obd::pid_t pid_7 = 0, obd::pid_t pid_8 = 0);

private:
    std::vector<obd::pid_t> mode_01_02_pids;
};

#endif // ECM_H
