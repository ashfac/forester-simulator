#ifndef OBD_H
#define OBD_H

#include "can.h"

namespace obd {

using pid_t = unsigned int;

namespace mode {

enum mode {
    mode_unknown                         = 0x00,
    mode_01_live_data                    = 0x01,
    mode_02_freeze_frame                 = 0x02,
    mode_03_get_stored_trouble_codes     = 0x03,
    mode_04_clear_stored_trouble_codes   = 0x04,
    mode_05_oxygen_sensors_test_results  = 0x05,
    mode_06_onboard_systems_test_results = 0x06,
    mode_07_get_pending_trouble_codes    = 0x07,
    mode_08_control_onboard_systems      = 0x08,
    mode_09_vehicle_information          = 0x09,
    mode_0A_get_permanent_trouble_codes  = 0x0A
};

} // namespace mode

namespace mode_01_02 {

namespace pid {

enum pid : pid_t {
    pid_00_get_supported_pids_01_20          = 0x00,
    pid_01_monitor_status_since_dtc_cleared  = 0x01,
    pid_02_freeze_dtc                        = 0x02,
    pid_03_fuel_system_status                = 0x03,
    pid_04_calculated_engine_load            = 0x04,
    pid_05_engine_coolant_temperature        = 0x05,
    pid_06_short_term_fuel_trim_bank_1       = 0x06,
    pid_07_long_term_fuel_trim_bank_1        = 0x07,
    pid_08_short_term_fuel_trim_bank_2       = 0x08,
    pid_09_long_term_fuel_trim_bank_2        = 0x09,
    pid_0A_fuel_pressure                     = 0x0A,
    pid_0B_intake_manifold_absolute_pressure = 0x0B,
    pid_0C_engine_speed                      = 0x0C,
    pid_0D_vehicle_speed                     = 0x0D,
    pid_0E_timing_advance                    = 0x0E,
    pid_0F_intake_air_temperature            = 0x0F,
    pid_10_maf_sensor_air_flow_rate          = 0x10,
    pid_11_throttle_position                 = 0x11,
    pid_12_commanded_secondary_air_status    = 0x12,
    pid_13_oxygen_sensors_present            = 0x13,
    pid_14_oxygen_sensor_1                   = 0x14,
    pid_15_oxygen_sensor_2                   = 0x15,
    pid_16_oxygen_sensor_3                   = 0x16,
    pid_17_oxygen_sensor_4                   = 0x17,
    pid_18_oxygen_sensor_5                   = 0x18,
    pid_19_oxygen_sensor_6                   = 0x19,
    pid_1A_oxygen_sensor_7                   = 0x1A,
    pid_1B_oxygen_sensor_8                   = 0x1B,
    pid_1C_obd_standards_confirmance         = 0x1C,
    pid_1D_oxygen_sensors_present            = 0x1D,
    pid_1E_auxiliary_input_status            = 0x1E,
    pid_1F_run_time_since_engine_start       = 0x1F,

    pid_20_pids_supported_21_40                           = 0x20,
    pid_21_distance_traveled_with_money_light_on          = 0x21,
    pid_22_fuel_rail_pressure_relative_to_manifold_vacuum = 0x22,
    pid_23_fuel_rail_gauge_pressure_direct_injection      = 0x23,
    pid_24_oxygen_sensor_1_voltage                        = 0x24,
    pid_25_oxygen_sensor_2_voltage                        = 0x25,
    pid_26_oxygen_sensor_3_voltage                        = 0x26,
    pid_27_oxygen_sensor_4_voltage                        = 0x27,
    pid_28_oxygen_sensor_5_voltage                        = 0x28,
    pid_29_oxygen_sensor_6_voltage                        = 0x29,
    pid_2A_oxygen_sensor_7_voltage                        = 0x2A,
    pid_2B_oxygen_sensor_8_voltage                        = 0x2B,
    pid_2C_commanded_egr                                  = 0x2C,
    pid_2D_egr_error                                      = 0x2D,
    pid_2E_commanded_evaporative_purge                    = 0x2E,
    pid_2F_fuel_tank_level_input                          = 0x2F,
    pid_30_warm_ups_since_codes_cleared                   = 0x30,
    pid_31_distance_traveled_since_codes_cleared          = 0x31,
    pid_32_evap_system_vapor_pressure                     = 0x32,
    pid_33_absolute_barometric_pressure                   = 0x33,
    pid_34_oxygen_sensor_1_current                        = 0x34,
    pid_35_oxygen_sensor_2_current                        = 0x35,
    pid_36_oxygen_sensor_3_current                        = 0x36,
    pid_37_oxygen_sensor_4_current                        = 0x37,
    pid_38_oxygen_sensor_5_current                        = 0x38,
    pid_39_oxygen_sensor_6_current                        = 0x39,
    pid_3A_oxygen_sensor_7_current                        = 0x3A,
    pid_3B_oxygen_sensor_8_current                        = 0x3B,
    pid_3C_catalyst_temperature_bank_1_sensor_1           = 0x3C,
    pid_3D_catalyst_temperature_bank_2_sensor_1           = 0x3D,
    pid_3E_catalyst_temperature_bank_1_sensor_2           = 0x3E,
    pid_3F_catalyst_temperature_bank_2_sensor_2           = 0x3F,

    pid_40_pids_supported_41_60                                 = 0x40,
    pid_41_monitor_status_this_drive_cycle                      = 0x41,
    pid_42_control_module_voltage                               = 0x42,
    pid_43_absolute_load_value                                  = 0x43,
    pid_44_commanded_air_fuel_equivalence_ratio                 = 0x44,
    pid_45_relative_throttle_position                           = 0x45,
    pid_46_ambient_air_temperature                              = 0x46,
    pid_47_absolute_throttle_position_b                         = 0x47,
    pid_48_absolute_throttle_position_c                         = 0x48,
    pid_49_accelerator_pedal_position_d                         = 0x49,
    pid_4A_accelerator_pedal_position_e                         = 0x4A,
    pid_4B_accelerator_pedal_position_f                         = 0x4B,
    pid_4C_commanded_throttle_actuator                          = 0x4C,
    pid_4D_time_run_with_mil_on                                 = 0x4D,
    pid_4E_time_since_trouble_codes_cleared                     = 0x4E,
    pid_4F_max_afr__oxygen_sensor_voltage_current__map          = 0x4F,
    pid_50_max_maf_air_flow_rate                                = 0x50,
    pid_51_fuel_type                                            = 0x51,
    pid_52_ethanol_fuel                                         = 0x52,
    pid_53_absolute_evap_system_vapor_pressure                  = 0x53,
    pid_54_evap_system_vapor_pressure                           = 0x54,
    pid_55_short_term_secondary_oxygen_sensor_trim_bank_1_and_3 = 0x55,
    pid_56_long_term_secondary_oxygen_sensor_trim_bank_1_and_3  = 0x56,
    pid_57_short_term_secondary_oxygen_sensor_trim_bank_2_and_4 = 0x57,
    pid_58_long_term_secondary_oxygen_sensor_trim_bank_2_and_4  = 0x58,
    pid_59_fuel_rail_absolute_pressure                          = 0x59,
    pid_5A_relative_accelerator_pedal_position                  = 0x5A,
    pid_5B_hybrid_battery_pack_remaining_life                   = 0x5B,
    pid_5C_engine_oil_temperature                               = 0x5C,
    pid_5D_fuel_injection_timing                                = 0x5D,
    pid_5E_engine_fuel_rate                                     = 0x5E,
    pid_5F_emission_requirements_to_which_vehicle_is_designed   = 0x5F,

    pid_60_pids_supported_61_80                                  = 0x60,
    pid_61_driver_demand_engine_percent_torque                   = 0x61,
    pid_62_actual_engine_percent_torque                          = 0x62,
    pid_63_engine_reference_torque                               = 0x63,
    pid_64_engine_percent_torque_data                            = 0x64,
    pid_65_auxiliary_input_output_supported                      = 0x65,
    pid_66_mass_air_flow_sensor                                  = 0x66,
    pid_67_engine_coolant_temperature                            = 0x67,
    pid_68_intake_air_temperature_sensor                         = 0x68,
    pid_69_commanded_egr_and_egr_error                           = 0x69,
    pid_6A_diesel_intake_air_flow_commanded_control_and_position = 0x6A,
    pid_6B_exhaust_gas_recirculation_temperature                 = 0x6B,
    pid_6C_throttle_actuator_commanded_control_and_position      = 0x6C,
    pid_6D_fuel_pressure_control_system                          = 0x6D,
    pid_6E_injection_pressure_control_system                     = 0x6E,
    pid_6F_turbocharger_compressor_inlet_pressure                = 0x6F,
    pid_70_boost_pressure_control                                = 0x70,
    pid_71_variable_geometry_turbo_vgt_control                   = 0x71,
    pid_72_wastegate_control                                     = 0x72,
    pid_73_exhaust_pressure                                      = 0x73,
    pid_74_turbocharger_rpm                                      = 0x74,
    pid_75_turbocharger_temperature                              = 0x75,
    pid_76_turbocharger_temperature                              = 0x76,
    pid_77_charge_air_cooler_temperature_cact                    = 0x77,
    pid_78_exhaust_gas_temperature_egt_bank_1                    = 0x78,
    pid_79_exhaust_gas_temperature_egt_bank_2                    = 0x79,
    pid_7A_diesel_particulate_filter_dpf_differential_pressure   = 0x7A,
    pid_7B_diesel_particulate_filter_dpf                         = 0x7B,
    pid_7C_diesel_particulate_filter_dpf_temperature             = 0x7C,
    pid_7D_nox_nte_not_to_exceed_control_area_status             = 0x7D,
    pid_7E_pm_nte_not_to_exceed_control_area_status              = 0x7E,
    pid_7F_engine_run_time                                       = 0x7F,

    pid_80_pids_supported_81_A0                                   = 0x80,
    pid_81_engine_run_time_for_auxiliary_emissions_control_device = 0x81,
    pid_82_engine_run_time_for_auxiliary_emissions_control_device = 0x82,
    pid_83_nox_sensor                                             = 0x83,
    pid_84_manifold_surface_temperature                           = 0x84,
    pid_85_nox_reagent_system                                     = 0x85,
    pid_86_particulate_matter_pm_sensor                           = 0x86,
    pid_87_intake_manifold_absolute_pressure                      = 0x87,
    pid_88_scr_induce_system                                      = 0x88,
    pid_89_run_time_for_auxiliary_emissions_control_device_11_15  = 0x89,
    pid_8A_run_time_for_auxiliary_emissions_control_device_16_20  = 0x8A,
    pid_8B_diesel_aftertreatment                                  = 0x8B,
    pid_8C_o2_sensor_wide_range                                   = 0x8C,
    pid_8D_throttle_position_G                                    = 0x8D,
    pid_8E_engine_friction_percent_torque                         = 0x8E,
    pid_8F_pm_sensor_bank_1_and_2                                 = 0x8F,
    pid_90_wwh_obd_vehicle_obd_system_information                 = 0x90,
    pid_91_wwh_obd_vehicle_obd_system_information                 = 0x91,
    pid_92_fuel_system_control                                    = 0x92,
    pid_93_wwh_obd_vehicle_obd_counters_support                   = 0x93,
    pid_94_nox_warning_and_inducement_system                      = 0x94,
    pid_98_exhaust_gas_temperature_sensor                         = 0x98,
    pid_99_exhaust_gas_temperature_sensor                         = 0x99,
    pid_9A_hybrid_ev_vehicle_system_data_battery_voltage          = 0x9A,
    pid_9B_diesel_exhaust_fluid_sensor_data                       = 0x9B,
    pid_9C_o2_sensor_data                                         = 0x9C,
    pid_9D_engine_fuel_rate_g_s                                   = 0x9D,
    pid_9E_engine_exhaust_flow_rate_kg_h                          = 0x9E,
    pid_9F_fuel_system_percentage_use                             = 0x9F,

    pid_A0_pids_supported_A1_C0        = 0xA0,
    pid_A1_nox_sensor_corrected_data   = 0xA1,
    pid_A2_cylinder_fuel_rate          = 0xA2,
    pid_A3_evap_system_vapor_pressure  = 0xA3,
    pid_A4_transmission_actual_gear    = 0xA4,
    pid_A5_diesel_exhaust_fluid_dosing = 0xA5,
    pid_A6_odometer                    = 0xA6,

    pid_C0_pids_supported_C1_E0        = 0xC0
};

} // namespace pid

} // namespace mode_01_02

namespace mode_05 {

namespace pid {

enum pid : int {

};

} // namespace pid

} // namespace mode_05

using mode_t = mode::mode;

constexpr can::header_t header_invalid       = 0x000;
constexpr can::header_t header_broadcast     = 0x7DF;
constexpr can::header_t response_header_mask = 0x008;
constexpr can::byte_t response_positive_mask = static_cast<can::byte_t>( 0x40 );

constexpr size_t mode_01_requet_size = 1;
constexpr size_t mode_02_requet_size = 1;
constexpr size_t mode_03_requet_size = 0;
constexpr size_t mode_04_requet_size = 0;
constexpr size_t mode_05_requet_size = 2;
constexpr size_t mode_06_requet_size = 1;
constexpr size_t mode_07_requet_size = 1;
constexpr size_t mode_08_requet_size = 1;
constexpr size_t mode_09_requet_size = 1;
constexpr size_t mode_0A_requet_size = 1;

} // namespace obd

#endif // OBD_H
