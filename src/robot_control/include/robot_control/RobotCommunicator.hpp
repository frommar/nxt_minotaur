#ifndef ROBOT_CONTROL_ROBOT_COMMUNICATOR_HPP
#define ROBOT_CONTROL_ROBOT_COMMUNICATOR_HPP

#include <pthread.h>
#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <move_base_msgs/MoveBaseActionFeedback.h>
#include <nxt/NXTControl.hpp>
#include "robot_control/RobotController.hpp"
#include "minotaur_common/PIDParameter.h"
#include "minotaur_common/RobotSettings.hpp"

namespace minotaur
{
    /**
     * \brief Gathers and publish Odometry data
     * 
     */
    class RobotCommunicator
    {
    private:
        nxt::Brick *brick;
        nxt::Motor leftMotor;
        nxt::Motor rightMotor;
        RobotController robotController;
        
        int statusMsec;
        
        ros::Publisher odometryPub;
        
        ros::Subscriber setPIDParamSub;
        ros::Subscriber cmdVelSub;
        
        tf::TransformBroadcaster *odomBroadcaster;
        
        pthread_mutex_t robotMutex;
        
	/**
	 * Sets a new velocity for the motors
	 * @param p_msg contains linear- and angular velocity
	 */
        void processSetVelocityMsg(const geometry_msgs::Twist& p_msg);
	/**
	 * Sets new values for the PID-Parameters
	 * @param p_msg New values of the PID-Parameters
	 */
        void processPIDParamMsg(const minotaur_common::PIDParameter &p_msg);
        void checkBrickStatus(const int p_samplingIntervalMsec);
    public:
        RobotCommunicator(nxt::Brick *p_brick);
        ~RobotCommunicator();
        
        void init(ros::NodeHandle &p_handle);
        void setTransformBroadcaster(tf::TransformBroadcaster *p_odomBroadcaster);
        
	/**
	 * Creates a TransformStamped and an Odometry message and publish them
	 */
        void publish();
	/**
	 * Calls the PID-Controller and the calculation for deadReckoning
	 * @param p_samplingIntervalMsec duration of one sampling interval in MSec
	 */
        void stepController(const int p_samplingIntervalMsec);
		
        void applySettings(const RobotSettings &p_settings);
        void applySettings(const MouseSensorSettings &p_settings);
		
        void shutdown();
    };
}

#endif
