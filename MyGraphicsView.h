#ifndef MYGRAPHICSSCENE_H
#define MYGRAPHICSSCENE_H


#include <iostream>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QTime>

using namespace std;



class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    MyGraphicsView(QWidget *parent = 0) :
        QGraphicsView(parent) {}

    virtual ~MyGraphicsView() {}

public Q_SLOTS:


    void mousePressEvent(QMouseEvent* event)
    {
        QPointF pt = mapToScene(event->pos());
        cout << "Click " << pt.x() << " " << pt.y() << endl;

        QGraphicsView::mousePressEvent(event);
    }
};

#endif // MYGRAPHICSSCENE_H
