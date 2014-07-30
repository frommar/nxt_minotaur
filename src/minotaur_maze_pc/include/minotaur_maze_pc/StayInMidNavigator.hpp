#ifndef MINOTAUR_STAY_IN_MID_NAVIGATOR_HPP
#define MINOTAUR_STAY_IN_MID_NAVIGATOR_HPP

#include <pthread.h>
#include "minotaur_maze_pc/MazeNavigator.hpp"

namespace minotaur
{
    enum MovementMode { WAITING, BEGIN_MOVE, MOVE, BEGIN_TURN, TURN };
    
    class StayInMidNavigator : public MazeNavigator
    {
    private:
        pthread_mutex_t mutex;
        pthread_cond_t condition;
        
        Direction currentDirection;
        Direction targetDirection;
        
        volatile MovementMode mode;
        volatile bool frontObstacle;
        
        float targetX, targetY;
        float targetTheta;
        float leftDistance, rightDistance;
        
        float startX, startY;
        float startTheta;
        
        void setTargetPosition(const nav_msgs::Odometry &p_odometry);
        bool reachedTargetPosition(const nav_msgs::Odometry &p_odometry);
        void setTargetTheta(const nav_msgs::Odometry &p_odometry);
        bool reachedTargetTheta(const nav_msgs::Odometry &p_odometry);
        
        void checkFrontObstacle(const robot_control_beagle::UltrasonicData &p_sensorData);
        void updateDistances(const robot_control_beagle::UltrasonicData &p_sensorData);
        
        void setMovementVelocity(const robot_control_beagle::UltrasonicData &p_sensorData);
        bool obstacleIsCloseEnough(const robot_control_beagle::UltrasonicData &p_sensorData);
        
        void setTurnVelocity(const robot_control_beagle::UltrasonicData &p_sensorData);
        
        bool isFrontSensor(int p_id);
        bool isMovingVertically();
        float getSensorDistanceThreshold();
        float getSensorOffset(int p_id);
        
        void stopMovement();
    public:
        StayInMidNavigator():mode(WAITING) { pthread_mutex_init(&mutex, NULL); pthread_cond_init(&condition, NULL); }
        ~StayInMidNavigator() { pthread_mutex_destroy(&mutex); pthread_cond_destroy(&condition); }
    
        void receivedOdometry(const nav_msgs::Odometry &p_odometry);
        void receivedUltrasonicData(const robot_control_beagle::UltrasonicData &p_sensorData) ;
        
        void moveToNextNode(Direction p_currentDirection);
        void turnRobotTo(Direction p_currentDirection, Direction p_newDirection);
        
        void shutdown();
    };
}

#endif
