/*
 * Author: Fabian Meyer 
 */

#include <pthread.h>
#include <ros/ros.h>
#include <termios.h>
#include <signal.h>
#include <exception>
#include "nxt_beagle/PIDController.hpp"
#include "nxt_beagle/nxtPower.h"
#include "nxt_beagle/nxtTicks.h"
#include "nxt_beagle/Config.hpp"

#define SAMPLE_INTERVALL 100
#define KEYCODE_R 0x43
#define KEYCODE_L 0x44
#define KEYCODE_U 0x41
#define KEYCODE_D 0x42
#define KEYCODE_Q 0x71

minotaur::PIDController pidController;
pthread_t thread;
struct termios oldtio, currtio;
pthread_mutex_t motorMutex = PTHREAD_MUTEX_INITIALIZER;
bool run = true;
ros::Publisher powerPublisher;
ros::ServiceClient tickClient;

void signalHandler(int sig);
void initPIDController(ros::NodeHandle p_handle);
int startThread();
void initKeyHandling();
void *pidThread(void *arg);
void eventLoop();
void setMotor(const float p_leftmps, const float p_rightmps);

int main(int argc, char **argv)
{
    ros::init(argc, argv, "pidTeleop");
    ros::NodeHandle handle;
    
    signal(SIGINT, signalHandler);
    
    initPIDController(handle);
    startThread();
    
    initKeyHandling();
    eventLoop();
    
    pthread_join(thread, NULL);
    
    ros::shutdown();
    return 0;
}

void signalHandler(int sig)
{
    run = false;
}

void initPIDController(ros::NodeHandle p_handle)
{
    ROS_INFO("Initializing PIDController...");
    
    powerPublisher = p_handle.advertise<nxt_beagle::nxtPower>(NXT_POWER_TOPIC, 1000);
    tickClient = p_handle.serviceClient<nxt_beagle::nxtTicks>(NXT_GET_TICKS_SRV);
    
    // publisher and client of pid-controller have to be
    // set before using "step()"
    pidController.setMotorPublisher(&powerPublisher);
    pidController.setMotorClient(&tickClient);
}

int startThread()
{
    ROS_INFO("Starting thread...");
    if(pthread_create(&thread, NULL, pidThread, NULL))
    {
        ROS_ERROR("Could not create Thread");
        return -1;
    }
    
    return 0;
}

void *pidThread(void *arg)
{
    ros::Time begin, end;
    double sleeptime;
    
    ROS_INFO("Thread is running.");
    
    while(run)
    {
        begin = ros::Time::now();
        pthread_mutex_lock(&motorMutex);
        
        // catch exceptions to prevent the thread from terminating
        try
        {
            pidController.step(SAMPLE_INTERVALL);
        }
        catch(std::exception& e)
        {
            ROS_ERROR("===Step encountered an exception==");
            ROS_ERROR("%s", e.what());
            ROS_ERROR("==================================");
        }
        
        pthread_mutex_unlock(&motorMutex);
        end = ros::Time::now();
        
        // sleep the remaining time of SAMPLE_INTERVALL 
        sleeptime = SAMPLE_INTERVALL - (end.toSec() - begin.toSec());
        if(sleeptime > 0)
            ros::Duration(sleeptime).sleep();
    }
    
    ROS_INFO("Thread terminated.");
}

void initKeyHandling()
{
    //save for restore
    tcgetattr(0, &oldtio);
    
    // put the console in raw mode to catch key events
    tcgetattr(0, &currtio);
    currtio.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &currtio);

    puts("Reading from keyboard");
    puts("---------------------------");
    puts("Use arrow keys to move Bender.");
}

void eventLoop()
{
    char keycode;
    ros::Rate loop_rate(60);
    
    ROS_INFO("Starting Eventloop...");
    
    while(run)
    {
        // read key events
        if(read(0, &keycode, 1) < 0)
        {
            perror("read():");
            exit(-1);
        }
        
        switch(keycode)
        {
            case KEYCODE_L:
                ROS_DEBUG("LEFT");
                setMotor(-0.3f, 0.3f);
                break;
            case KEYCODE_R:
                ROS_DEBUG("RIGHT");
                setMotor(0.3f, -0.3f);
                break;
            case KEYCODE_U:
                ROS_DEBUG("UP");
                setMotor(0.3f, 0.3f);
                break;
            case KEYCODE_D:
                ROS_DEBUG("DOWN");
                setMotor(-0.3f, -0.3f);
                break;
            case KEYCODE_Q:
                ROS_DEBUG("STOP");
                setMotor(0,0);
                break;
        }
            
        loop_rate.sleep();
    }
    
    tcsetattr(0, TCSANOW, &oldtio);
    ROS_INFO("Eventloop terminated.");
}

void setMotor(const float p_leftmps, const float p_rightmps)
{
    minotaur::MotorVelocity velocity(p_leftmps, p_rightmps);
    pthread_mutex_lock(&motorMutex);
    
    // set velocity for left and right motor
    pidController.setVelocity(velocity);
    
    pthread_mutex_unlock(&motorMutex);
}