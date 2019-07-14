#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mMarkersScene(new QGraphicsScene),
    mPointsRange({0, 0})
{
    ui->setupUi(this);
    ui->progressBar->hide();
    ui->graphicsView->setScene(mMarkersScene);
    mSourceTransform = ui->graphicsView->transform();
    connect(ui->actionOpen_Points_File, &QAction::triggered, this,
            &MainWindow::onOpenPointsFile, Qt::DirectConnection);
    connect(ui->actionOpen_markers, &QAction::triggered, this,
                    &MainWindow::onOpenMarkersFile, Qt::DirectConnection);
    connect(ui->actionQuit, &QAction::triggered, this,
                    &MainWindow::onQuit, Qt::DirectConnection);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onAbout,
                Qt::DirectConnection);
}


void MainWindow::setMainViewEventsInterface(IMainViewEvents *interf)
{
    mEventsInterf = interf;
}

void MainWindow::show()
{
    QMainWindow::show();
}

void MainWindow::hide()
{
    QMainWindow::hide();
}

void MainWindow::showMarkers(const QList<MyTypes::SMarkerType> &markers,
                             MyTypes::SPointType pointsRange)
{
    mPointsRange = pointsRange;
    mMarkers = markers;
    mMarkersScene->clear();
    if(mPointsPixmap !=nullptr)
    {
        drawPoints(*mPointsPixmap);
    }
    drawMarkers(markers, mPointsRange);
    scaleGraphicsView();
}

void MainWindow::showPoints(QPixmap &pixmap)
{
    mMarkersScene->clear();
    mPointsPixmap = &pixmap;
    drawPoints(pixmap);
    drawMarkers(mMarkers, mPointsRange);
}


void MainWindow::showProgress(float percentage)
{
    ui->progressBar->setValue(percentage);
    if(percentage == 100.0f)
    {
        ui->progressBar->hide();
    }
    else
    {
        ui->progressBar->show();
    }
}

void MainWindow::showErrorMessage(const QString &message)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle(tr("Ошибка"));
    msgBox.setText(message);
    msgBox.exec();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onOpenPointsFile()
{
    if(mEventsInterf == nullptr)
    {
        return;
    }
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Выберите файл"), "", tr("Бинарные файлы (*.bin)"));

    // Если файл не выбран, выходим
    if (fileName.isEmpty())
    {
        return;
    }
    mEventsInterf->pointsFileStartRead(fileName);

}

void MainWindow::onOpenMarkersFile()
{
    if(mEventsInterf == nullptr)
    {
        return;
    }

    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Выберите файл"), "", tr("XML файлы (*.xml)"));

    // Если файл не выбран, выходим
    if (fileName.isEmpty())
    {
        return;
    }
    if(!mEventsInterf->markersXmlFileOpen(fileName))
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("Ошибка открытия xml файла.");
        msgBox.exec();
    }
}

void MainWindow::onQuit()
{
    if(mEventsInterf != nullptr)
    {
        mEventsInterf->quit();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    if(mEventsInterf == nullptr)
    {
        return;
    }
    mEventsInterf->quit();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    scaleGraphicsView();
}

void MainWindow::wheelEvent(QWheelEvent *e)
{
    const float MOUSE_WEEL_SPEED = 0.01;
    QPoint numPixels = e->pixelDelta();
    QPoint numDegrees = e->angleDelta() / 8;

    if (!numPixels.isNull())
    {
        numDegrees = numPixels;
        //scrollWithPixels(numPixels);
    } else if (!numDegrees.isNull())
    {
        numDegrees = numDegrees / 15;
        //scrollWithDegrees(numSteps);
    }
    //QPoint delta = e->angleDelta();

    mScaleVal *= 1.0f + float(numDegrees.ry()) * MOUSE_WEEL_SPEED;

    scaleGraphView(mScaleVal);

    qDebug() << QString("x = %1").arg(mScaleVal);

}


void MainWindow::drawMarkers(const QList<MyTypes::SMarkerType> &markers,
                             MyTypes::SPointType pointsRange)
{
    QPen pen;
    pen.setColor(QColor(0xFF, 0x00, 0x00));
    const int markSize = MyTypes::MARKERS_LENGTH;
    mMarkersScene->blockSignals(true);

    int offset = pointsRange.x*MyTypes::POINTS_SCALE/2;

    for(const MyTypes::SMarkerType &marker : markers)
    {
        int x = marker.x*MyTypes::MARKERS_SCALE + offset;
        int y = marker.y*MyTypes::MARKERS_SCALE + offset;

        mMarkersScene->addLine(x-markSize, y,
                              x+markSize, y, pen);
        mMarkersScene->addLine(x, y-markSize,
                              x, y+markSize, pen);
    }
    mMarkersScene->blockSignals(false);
}

void MainWindow::drawPoints(QPixmap &pixmap)
{
    mMarkersScene->addPixmap(pixmap);
}

void MainWindow::scaleGraphicsView()
{
    float scalex = 1.0;

    float scaley = 1.0;

    if(mMarkersScene->width() > 10 &&
            mMarkersScene->height() > 10)
    {
        scalex = float(ui->graphicsView->width())/
                float(mMarkersScene->width());

        scaley =  float(ui->graphicsView->height())/
                float(mMarkersScene->height());
    }

    scalex = (scalex > scaley)? scaley: scalex;

    qDebug() << QString("graphicsView (%1, %2)").arg(ui->graphicsView->width()).arg(ui->graphicsView->height());
    qDebug() << QString("mMarkersScene (%1, %2)").arg(mMarkersScene->width()).arg(mMarkersScene->height());

    QTransform transf = mSourceTransform;
    transf.scale(scalex*0.9, scalex*0.9);
    ui->graphicsView->setTransform(transf);

    //ui->graphicsView->scale(0.999, 0.999);
}

void MainWindow::scaleGraphView(float scale)
{
    mScaleVal = scale;
    QTransform transf = mSourceTransform;
    transf.scale(scale, scale);
    ui->graphicsView->setTransform(transf);
}

void MainWindow::onAbout()
{
    if(mAbout == nullptr)
    {
        mAbout.reset(new AboutWindow);
        mAbout->setAppDescription(mDescr);
        mAbout->setAppVersion(mVersion);
        mAbout->setAppBuildDate(mBuildDate);
        mAbout->setContacts(mContacts);
        mAbout->setMyPhoto(mMyPhoto);
    }
    mAbout->show();
}
