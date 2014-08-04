#include "robot_control/RobotController.hpp"
#include "minotaur_common/MotorVelocity.hpp"
#include "minotaur_common/Math.hpp"

namespace minotaur
{
    RobotController::RobotController()
    : odometry(), velocity(), wheelTrack(0.0f), pidController()
    {
        initOdometry(odometry);
    }
    
    IPIDController& RobotController::getPIDController()
    {
        return pidController;
    }
    
    float RobotController::getWheelTrack() const
    {
        return wheelTrack;
    }
    
    const nav_msgs::Odometry& RobotController::getOdometry()
    {
        return odometry;
    }

    void RobotController::setVelocity(const geometry_msgs::Twist& p_velocity)
    {
        velocity = p_velocity;
        calculateMotorVelocity();
    }
    
    void RobotController::setWheelTrack(const float p_wheelTrack)
    {
        wheelTrack = p_wheelTrack;
        calculateMotorVelocity();
    }

    void RobotController::calculateMotorVelocity()
    {
        MotorVelocity targetVelocity;
        
        //to get the formula see kinematic of two wheeled robots
        double theta = getTheta(odometry);
        float linearVelocity = getLinearVelocity(velocity, theta);
        float angularVelocity = getAngularVelocity(velocity);
        ROS_INFO("Setting v=%.2f; w=%.2f; theta=%.2f", linearVelocity, angularVelocity, theta);
        targetVelocity.leftMPS = linearVelocity - (angularVelocity * wheelTrack) / 2;
        targetVelocity.rightMPS = linearVelocity + (angularVelocity * wheelTrack) / 2;
        
        pidController.setVelocity(targetVelocity);
    }
    
    void RobotController::setPose(const geometry_msgs::PoseWithCovariance& p_pose)
    {
        odometry.pose = p_pose;
    }
    
    void RobotController::step(const int p_samplingIntervallMsec)
    {
        pidController.step(p_samplingIntervallMsec);
        deadReckoning(p_samplingIntervallMsec);
    }
    
    void RobotController::deadReckoning(const int p_samplingIntervallMsec)
    {
        float intervalSec = msecToSec(p_samplingIntervallMsec);
        
        float theta = getTheta(odometry);
        
        // get measured Velocity from PID and transform to Twist
        geometry_msgs::Twist nextVelocity = getMeasuredVelocity(theta);
        ROS_INFO("DeadReck: theta=%.2f; angVel=%.2f; nextAngVel=%.2f", theta, getAngularVelocity(odometry), getAngularVelocity(nextVelocity));
        //calculate new pose via deadReckoning
        odometry.pose.pose.position.x += ((odometry.twist.twist.linear.x + nextVelocity.linear.x) / 2) * intervalSec;
        odometry.pose.pose.position.y += ((odometry.twist.twist.linear.y + nextVelocity.linear.y) / 2) * intervalSec;
        setTheta(odometry, theta + ((getAngularVelocity(odometry) + getAngularVelocity(nextVelocity)) / 2 ) * intervalSec);
        
        // set next measured velocity
        odometry.twist.twist = nextVelocity;
    }
    
    geometry_msgs::Twist RobotController::getMeasuredVelocity(const float p_theta)
    {
        geometry_msgs::Twist result;
        MotorVelocity motorVel = pidController.getMeasuredVelocity();
        //to get the formula see kinematic of two wheeled robots
        float linearVelocity = (motorVel.rightMPS + motorVel.leftMPS) / 2;
        float angularVelocity = (motorVel.rightMPS - motorVel.leftMPS) / wheelTrack;
        
        initTwist(result);
        setLinearVelocity(result, p_theta, linearVelocity);
        setAngularVelocity(result, angularVelocity);
        
        return result;
    }
}
