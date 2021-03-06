#include <cstdio>
#include "pid_monitor/PIDWindow.hpp"
#include "minotaur_common/Math.hpp"

#define MAX_LIN_VEL 0.5f
#define MAX_ANG_VEL 2.0f
#define MAX_PLOT_LIN_VEL 0.3
#define MAX_PLOT_ANG_VEL 2.0

#define MAX_KP 10.0f
#define MAX_KI 2.0f
#define MAX_KD 2.0f
#define DEF_KP 0.5f
#define DEF_KI 0.3f
#define DEF_KD 0.1f

using namespace Qt;

namespace minotaur
{
    PIDWindow::PIDWindow(QWidget *parent)
    :QMainWindow(parent)
    {
        setupUi(this);
        
        sensorPainter = new QSensorPainter(centralwidget);
        centralwidget->layout()->addWidget(sensorPainter);
        sensorPainter->setMinimumSize(200, 400);
        
        connect(&controlNode, SIGNAL(receivedOdometry(const QOdometry&)), this, SLOT(processMeasuredVelocity(const QOdometry&)));
        connect(&controlNode, SIGNAL(receivedUltrasonicData(const QUltrasonicData&)), this, SLOT(processMeasuredSensor(const QUltrasonicData&)));
        
        connect(BrakeButton, SIGNAL(clicked()), this, SLOT(brake()));
        
        connect(VelSlider, SIGNAL(valueChanged(int)), this, SLOT(updateVelocityValues()));
        connect(AngSlider, SIGNAL(valueChanged(int)), this, SLOT(updateVelocityValues()));
        connect(VelSlider, SIGNAL(sliderReleased()), this, SLOT(setVelocity()));
        connect(AngSlider, SIGNAL(sliderReleased()), this, SLOT(setVelocity()));
        
        connect(KPSlider, SIGNAL(valueChanged(int)), this, SLOT(updatePIDValues()));
        connect(KISlider, SIGNAL(valueChanged(int)), this, SLOT(updatePIDValues()));
        connect(KDSlider, SIGNAL(valueChanged(int)), this, SLOT(updatePIDValues()));
        connect(KPSlider, SIGNAL(sliderReleased()), this, SLOT(setPID()));
        connect(KISlider, SIGNAL(sliderReleased()), this, SLOT(setPID()));
        connect(KDSlider, SIGNAL(sliderReleased()), this, SLOT(setPID()));
        
        sensorSettings = loadCurrentSensorSettings();
        
        initComponents();
    }
    
    PIDWindow::~PIDWindow() { }
    
    void PIDWindow::initComponents()
    {
        VelSlider->setRange(-100, 100);
        VelSlider->setValue(0);
        
        AngSlider->setRange(-100, 100);
        AngSlider->setValue(0);
        
        KPSlider->setRange(0, 100);
        KPSlider->setValue((DEF_KP / MAX_KP) * 100);
        
        KISlider->setRange(0, 100);
        KISlider->setValue((DEF_KD / MAX_KD) * 100);
        
        KDSlider->setRange(0, 100);
        KDSlider->setValue((DEF_KD / MAX_KD) * 100);
        
        LinTargetProgressBar->setProperty("defaultStyleSheet", LinTargetProgressBar->styleSheet());
        AngTargetProgressBar->setProperty("defaultStyleSheet", AngTargetProgressBar->styleSheet());
        LinMeasuredProgressBar->setProperty("defaultStyleSheet", LinMeasuredProgressBar->styleSheet());
        AngMeasuredProgressBar->setProperty("defaultStyleSheet", AngMeasuredProgressBar->styleSheet());
        
        initPlotComponents();
    }

    void PIDWindow::initPlotComponents()
    {
        memset(linSamples, 0, SAMPLE_RANGE * sizeof (double));
        memset(angSamples, 0, SAMPLE_RANGE * sizeof (double)); 
        for(int i = 0; i < SAMPLE_RANGE; i++)
            xSamples[i] = i;
        
        sampleCount = 0;
        
        linCurve.setPen(QColor(Qt::red));
        linCurve.setSamples(xSamples, linSamples, SAMPLE_RANGE);
        angCurve.setPen(QColor(Qt::green));
        angCurve.setSamples(xSamples, angSamples, SAMPLE_RANGE);
        
        linCurve.attach(MeasureLinPlot);
        angCurve.attach(MeasureAngPlot);
        
        MeasureLinPlot->setTitle("Linear Velocity");
        MeasureLinPlot->setCanvasBackground(QColor(Qt::white));
        MeasureLinPlot->setAxisScale(QwtPlot::xBottom, 0, SAMPLE_RANGE, 50);
        MeasureLinPlot->setAxisScale(QwtPlot::yLeft, -MAX_PLOT_LIN_VEL, MAX_PLOT_LIN_VEL, 0.05);
        MeasureLinPlot->setAxisTitle(QwtPlot::xBottom, "step");
        MeasureLinPlot->setAxisTitle(QwtPlot::yLeft, "m/s");
        
        MeasureAngPlot->setTitle("Angular Velocity");
        MeasureAngPlot->setCanvasBackground(QColor(Qt::white));
        MeasureAngPlot->setAxisScale(QwtPlot::xBottom, 0, SAMPLE_RANGE, 50);
        MeasureAngPlot->setAxisScale(QwtPlot::yLeft, -MAX_PLOT_ANG_VEL, MAX_PLOT_ANG_VEL, 0.5);
        MeasureAngPlot->setAxisTitle(QwtPlot::xBottom, "step");
        MeasureAngPlot->setAxisTitle(QwtPlot::yLeft, "m/s");
    }
    
    QMinotaurControlNode& PIDWindow::getControlNode()
    {
        return controlNode;
    }
    
    void PIDWindow::brake()
    {
        VelSlider->setValue(0);
        AngSlider->setValue(0);
        setVelocity();
    }
    
    void PIDWindow::setVelocity()
    {
        updateVelocityValues();
        controlNode.setVelocity(getLinearVel(), getAngVel());
    }
    
    void PIDWindow::updateVelocityValues()
    {
        VelValueLabel->setText(QString::number(getLinearVel(), 'f', 2));
        AngValueLabel->setText(QString::number(getAngVel(), 'f', 2));
        
        int value = abs((getLinearVel() / MAX_LIN_VEL) * 100);
        LinTargetProgressBar->setValue(value);
        updateProgressBarColor(LinTargetProgressBar);
        LinTargetValueLabel->setText(QString::number(getLinearVel(), 'f', 2));
        
        value = abs((getAngVel() / MAX_ANG_VEL) * 100);
        AngTargetProgressBar->setValue(value);
        updateProgressBarColor(AngTargetProgressBar);
        AngTargetValueLabel->setText(QString::number(getAngVel(), 'f', 2));
    }
    
    float PIDWindow::getLinearVel()
    {
        return ((float) (VelSlider->value()) / 100.0f) * MAX_LIN_VEL;
    }
    
    float PIDWindow::getAngVel()
    {
        return ((float) (AngSlider->value()) / 100.0f) * MAX_ANG_VEL;
    }
    
    void PIDWindow::setPID()
    {
        updatePIDValues();
        controlNode.setPIDParameter(getKP(), getKI(), getKD());
    }
    
    void PIDWindow::updatePIDValues()
    {
        KPValueLabel->setText(QString::number(getKP(), 'f', 2));
        KIValueLabel->setText(QString::number(getKI(), 'f', 2));
        KDValueLabel->setText(QString::number(getKD(), 'f', 2));
    }
    
    float PIDWindow::getKP()
    {
        return ((float) (KPSlider->value()) / 100.0f) * MAX_KP;
    }
    
    float PIDWindow::getKI()
    {
        return ((float) (KISlider->value()) / 100.0f) * MAX_KI;
    }
    
    float PIDWindow::getKD()
    {
        return ((float) (KDSlider->value()) / 100.0f) * MAX_KD;
    }
    
    void PIDWindow::processMeasuredVelocity(const QOdometry &p_odom)
    {
        float linVel = getLinearVelocity(p_odom.odometry);
        float angVel = getAngularVelocity(p_odom.odometry);
        updateMeasuredProgressBars(linVel, angVel);
        updateMeasuredPlot(linVel, angVel);
    }
    
    void PIDWindow::updateMeasuredProgressBars(const float p_linVel, const float p_angVel)
    {
        int value = abs((p_linVel / MAX_LIN_VEL) * 100);
        LinMeasuredProgressBar->setValue(value);
        updateProgressBarColor(LinMeasuredProgressBar);
        LinMeasuredValueLabel->setText(QString::number(p_linVel, 'f', 2));
        
        value = abs((p_angVel / MAX_ANG_VEL) * 100);
        AngMeasuredProgressBar->setValue(value);
        updateProgressBarColor(AngMeasuredProgressBar);
        AngMeasuredValueLabel->setText(QString::number(p_angVel, 'f', 2));
    }
    
    void PIDWindow::updateMeasuredPlot(const float p_linVel, const float p_angVel)
    {
        linSamples[sampleCount] = p_linVel;
        angSamples[sampleCount] = p_angVel;
        
        linCurve.setSamples(xSamples, linSamples, SAMPLE_RANGE);
        angCurve.setSamples(xSamples, angSamples, SAMPLE_RANGE);
        
        MeasureLinPlot->replot();
        MeasureAngPlot->replot();
        
        ++sampleCount;
        sampleCount %= SAMPLE_RANGE;
    }
    
    void PIDWindow::updateProgressBarColor(QProgressBar *p_progressbar)
    {
        if(p_progressbar->value() <= 50)
            p_progressbar->setStyleSheet(p_progressbar->property("defaultStyleSheet").toString() + " QProgressBar::chunk { background: green; }");
        else if(p_progressbar->value() <= 75)
            p_progressbar->setStyleSheet(p_progressbar->property("defaultStyleSheet").toString() + " QProgressBar::chunk { background: yellow; }");
        else
            p_progressbar->setStyleSheet(p_progressbar->property("defaultStyleSheet").toString() + " QProgressBar::chunk { background: red; }");
    }
    
    void PIDWindow::processMeasuredSensor(const QUltrasonicData &p_sensor)
    {
        int x = cos(-sensorSettings[p_sensor.data.sensorID].direction - M_PI / 2) * p_sensor.data.distance;
        int y = sin(-sensorSettings[p_sensor.data.sensorID].direction - M_PI / 2) * p_sensor.data.distance;
        
        while(p_sensor.data.sensorID >= sensorPainter->getCount())
            sensorPainter->addMeasurement(QPoint());
        
        sensorPainter->setMeasurement(p_sensor.data.sensorID ,QPoint(x,y));
        sensorPainter->update();
    }
    
}
