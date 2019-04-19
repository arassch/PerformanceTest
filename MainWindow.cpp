#include <fstream>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QThread>
#include <QTimer>
#include <QFileDialog>

#include "MainWindow.h"


#include "ui_MainWindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_renderThread = new QThread;


    g_Framebuffer = new CFrameBuffer(1024,1024);
    g_Data = new CSphereData("/Users/sarac.schvartzman/Dropbox/Code/Crytek/SphereDataViewerOriginalQt/sphere_sample_points.txt", g_Framebuffer);

    g_Data->moveToThread(m_renderThread);
    connect(m_renderThread, SIGNAL(started()), g_Data, SLOT(Render()));
//    connect(m_algorithm, SIGNAL(finished()), m_renderThread, SLOT(quit()));
//    connect(m_algorithm, SIGNAL(finished()), this, SLOT(writeToFile()));
    m_renderThread->start();

    m_qscene = new QGraphicsScene( this );

    QImage image(g_Framebuffer->GetFrameBuffer(),
                 1024, 1024,
                 QImage::Format_RGB888);
    m_pixmapItem = m_qscene->addPixmap(QPixmap::fromImage(image));
    ui->graphicsView->setScene(m_qscene);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10);



    m_series = new QLineSeries();

    m_chart = new QChart();
    m_chart->legend()->hide();
    m_chart->addSeries(m_series);
    m_chart->createDefaultAxes();
    m_chart->setTitle("Simple line chart example");

    QLineSeries* originalSeries = new QLineSeries();
    std::ifstream fin("/Users/sarac.schvartzman/Dropbox/Code/Crytek/results/original.txt");
    unsigned int t;
    unsigned int i=0;
    while (fin >> t)
    {
        *originalSeries << QPointF(i, t);
        m_maxTime = std::max(m_maxTime, t);
        m_maxFrame = std::max(m_maxFrame, i);
        i++;
    }

    m_chart->addSeries(originalSeries);

    ui->chart->setChart(m_chart);
    ui->chart->setRenderHint(QPainter::Antialiasing);

    connect(ui->actionSave_times, &QAction::triggered, this, &MainWindow::saveTimes);

}

MainWindow::~MainWindow()
{
    g_Data->deleteLater();
    m_renderThread->deleteLater();
    delete ui;
}

void MainWindow::update()
{
    QImage image(g_Framebuffer->GetFrameBuffer(), 1024, 1024, QImage::Format_RGB888);
    m_pixmapItem->setPixmap(QPixmap::fromImage(image));

    for(unsigned int i=m_series->count(); i<g_Data->m_times.size(); ++i)
    {
        *m_series << QPointF(i, g_Data->m_times[i]);
        m_maxTime = std::max(m_maxTime, g_Data->m_times[i]);
        m_maxFrame = std::max(m_maxFrame, i);
    }

    m_chart->axisX()->setRange(0,m_maxFrame);
    m_chart->axisY()->setRange(0,m_maxTime);

    ui->chart->repaint();
}

void MainWindow::saveTimes()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                "/Users/sarac.schvartzman/Dropbox/Code/Crytek/results/result.txt",
                                tr("Text (*.txt)"));
    std::ofstream fout(fileName.toStdString());
    for(int i=0; i<m_series->count(); ++i)
        fout << m_series->at(i).y() << std::endl;
    fout.close();
}
