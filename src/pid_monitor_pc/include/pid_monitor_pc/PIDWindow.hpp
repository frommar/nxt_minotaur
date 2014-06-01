#ifndef PID_WINDOW_HPP_
#define PID_WINDOW_HPP_

#include <QtGui/QMainWindow>
#include <QPoint>
#include <qwt_plot_curve.h>
#include <qwt_plot_layout.h>
#include "pid_monitor_pc/QSensorPainter.hpp"
#include "ui_pid_window.h"
#include "pid_monitor_pc/QMinotaurNavigateNode.hpp"

#define SAMPLE_RANGE 400

namespace minotaur {
    
    class PIDWindow : public QMainWindow, public Ui::PIDControlWindow
    {
        Q_OBJECT
    private:
        QMinotaurNavigateNode navNode;
        QSensorPainter *sensorPainter;
        
        int sampleCount;
        QwtPlotCurve linCurve, angCurve;
        double xSamples[SAMPLE_RANGE];
        double linSamples[SAMPLE_RANGE];
        double angSamples[SAMPLE_RANGE];
        
        void initComponents();
        void initPlotComponents();
        
        float getLinearVel();
        float getAngVel();
        
        float getKP();
        float getKI();
        float getKD();
        
        void updateMeasuredProgressBars(const float p_linVel, const float p_angVel);
        void updateMeasuredPlot(const float p_linVel, const float p_angVel);
        void updateProgressBarColor(QProgressBar *p_progressbar);
        
    private Q_SLOTS:
        void brake();
        void setPID();
        void updatePIDValues();
        void setVelocity();
        void updateVelocityValues();
        void processMeasuredVelocity(const QOdometry p_velocity);
        void processMeasuredSensor(const QUltraSensor p_sensor);
    public:
        PIDWindow(QWidget *parent = 0);
        ~PIDWindow();
        
        QMinotaurNavigateNode& getNavigationNode();
    };
    
}

#endif