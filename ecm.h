#ifndef ECM_H
#define ECM_H

#include "ecu.h"

class ECM : public ECU
{
public:
    ECM(const std::shared_ptr<CanBus> can_bus, const can::can_id_t ecu_id, const can::protocol_t protocol);

    void put(const can::protocol_t protocol, can::can_msg_t message);

private:
    void handle_obd_request(can::can_data_t &data);

    void handle_mode_01_live_data(can::can_data_t &data);
    void handle_mode_02_freeze_frame(can::can_data_t &data);
    void handle_mode_03_get_stored_trouble_codes(can::can_data_t &data);
    void handle_mode_04_clear_stored_trouble_codes(can::can_data_t &data);
    void handle_mode_05_oxygen_sensors_test_results(can::can_data_t &data);
    void handle_mode_06_onboard_systems_test_results(can::can_data_t &data);
    void handle_mode_07_get_pending_trouble_codes(can::can_data_t &data);
    void handle_mode_08_control_onboard_systems(can::can_data_t &data);
    void handle_mode_09_vehicle_information(can::can_data_t &data);
    void handle_mode_0A_get_permanent_trouble_codes(can::can_data_t &data);

};

#endif // ECM_H
