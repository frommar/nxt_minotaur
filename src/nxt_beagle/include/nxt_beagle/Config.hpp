/*
 * Author: Fabian Meyer
 */

#ifndef NXT_BEAGLE_CONFIG_HPP
#define NXT_BEAGLE_CONFIG_HPP

#include <string>

#define NXT_BEAGLE_MAJOR_VERSION 0
#define NXT_BEAGLE_MINOR_VERSION 1
#define NXT_BEAGLE_PATCH_VERSION 0
#define NXT_BEAGLE_VERSION 0.1.0

#define NXT_PORT1 1
#define NXT_PORT2 2
#define NXT_PORT3 3
#define NXT_PORT4 4

#define MINOTAUR_ROS_NS "/minotaur"

#define HERACLES_NAME "Heracles"
#define THESEUS_NAME "Theseus"

#define NXT_POWER_TOPIC MINOTAUR_ROS_NS "/cmd_pow"
#define NXT_ULTRA_SENSOR_TOPIC MINOTAUR_ROS_NS "/measure_ultrasensor"
#define NXT_SET_ROBOT_VELOCITY_TOPIC MINOTAUR_ROS_NS "/cmd_rvel"
#define NXT_TARGET_ROBOT_VELOCITY_TOPIC MINOTAUR_ROS_NS "/target_rvel"
#define NXT_TARGET_MOTOR_VELOCITY_TOPIC MINOTAUR_ROS_NS "/target_mvel"
#define NXT_MEASURE_ROBOT_VELOCITY_TOPIC MINOTAUR_ROS_NS "/measure_rvel"
#define NXT_MEASURE_MOTOR_VELOCITY_TOPIC MINOTAUR_ROS_NS"/measure_mvel"
#define NXT_SET_SAMPLING_INTERVAL_TOPIC MINOTAUR_ROS_NS "/set_sampling_interval"
#define NXT_SET_PID_PARAMETER MINOTAUR_ROS_NS "/set_pid_param"
#define NXT_CLEAR_SENSOR_TOPIC MINOTAUR_ROS_NS "/clear_sensor"
#define NXT_SET_MODEL_TOPIC MINOTAUR_ROS_NS "/set_model"

#define NXT_GET_TICKS_SRV MINOTAUR_ROS_NS "/get_ticks"
#define NXT_GET_ULTRASONIC_SRV MINOTAUR_ROS_NS "/get_ultrasonic"
#define NXT_ADD_ULTRASONIC_SRV MINOTAUR_ROS_NS "/add_ultrasonic"

inline std::string PARAM_NAME(const std::string& p_name)
{ return (MINOTAUR_ROS_NS "/") + p_name; }

inline std::string PARAM_WHEEL_TRACK(const std::string& p_name)
{ return PARAM_NAME(p_name) + "/wheel_track"; }

inline std::string PARAM_WHEEL_CIRCUMFERENCE(const std::string& p_name)
{ return PARAM_NAME(p_name) + "/wheel_circumference"; }

inline std::string PARAM_KP(const std::string& p_name)
{ return PARAM_NAME(p_name) + "/kp"; }

inline std::string PARAM_KI(const std::string& p_name)
{ return PARAM_NAME(p_name) + "/ki"; }

inline std::string PARAM_KD(const std::string& p_name)
{ return PARAM_NAME(p_name) + "/kd"; }

inline std::string PARAM_SAMPLING_INTERVAL(const std::string& p_name)
{ return PARAM_NAME(p_name) + "/sampling_interval"; }

#endif
