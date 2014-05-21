#ifndef NXT_QT_QMAP_WIDGET_HPP_
#define NXT_QT_QMAP_WIDGET_HPP_

#include <QWidget>
#include <QMutex>
#include <QPainter>
#include <QPoint>
#include <QVector>
#include "minotaur_pc/MapCreator.hpp"
#include "minotaur_pc/RobotPosition.hpp"

namespace minotaur
{
    class QMapWidget : public QWidget
    {
        
    private:
        MapCreator mapCreator;
        RobotPosition robotPos;
        QMutex mapMutex;
        QMutex positionMutex;
        QVector<QPoint> lastPositions;
        int positionCounter;
        
        void setRobotPosition(const RobotPosition& p_robotPos);
        void setMapCreatorPosition(const RobotPosition& p_robotPos);
        
    protected:
        void paintEvent(QPaintEvent *event);
        void paintMap(QPainter& p_painter);
        void paintRobot(QPainter& p_painter);
        
    public:
        QMapWidget(QWidget *parent);
        virtual ~QMapWidget() { }
        
        void setPosition(const RobotPosition& p_robotPos);
        void step(const int p_id, const int p_distance);
    };
}

#endif