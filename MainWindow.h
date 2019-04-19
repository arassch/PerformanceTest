#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

QT_CHARTS_USE_NAMESPACE

#include "FrameBuffer.h"
#include "SphereData.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

public Q_SLOTS:
    void update();
    void saveTimes();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *m_qscene;
    QGraphicsPixmapItem *m_pixmapItem;

    QThread* m_renderThread;

    CSphereData *g_Data;
    CFrameBuffer *g_Framebuffer;

    QLineSeries *m_series;
    QChart *m_chart;
    unsigned int m_maxTime;
    unsigned int m_maxFrame;
};

#endif // MAINWINDOW_H
