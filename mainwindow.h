#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <IMainView.h>
#include <IMainViewEvents.h>
#include <QPointer>
#include <QGraphicsScene>
#include <QPixmap>
#include <QTransform>
#include <About/aboutwindow.h>
#include <AppInfo/capplicationinfo.h>

namespace Ui {
    class MainWindow;
}


// Главное окно. В нем реализовано только
// отображение и вызовы событий при взаимодействием с ним пользователя.

class MainWindow : public QMainWindow, public IMainView
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    // Реализуем интерфейс IMainView
    void setMainViewEventsInterface(IMainViewEvents *interf) override;
    void show() override;
    void hide() override;
    void showMarkers(const QList<MyTypes::SMarkerType> &markers,
                        MyTypes::SPointType pointsRange) override;
    void showPoints(QPixmap &pixmap) override;
    void showProgress(float percentage) override;
    void showErrorMessage(const QString &message) override;

    void setAppName(const QString &appName) override
    {
        mAppName = appName;
    }
    void setAppDescription(const QString &appDescription) override
    {
        mDescr = appDescription;
    }
    void setAppVersion(const QString &appVersion) override
    {
        mVersion = appVersion;
    }
    void setAppBuildDate(const QString &buildDate) override
    {
        mBuildDate = buildDate;
    }
    void setContacts(const QString &contacts) override
    {
        mContacts = contacts;
    }
    void setMyPhoto(const QPixmap &photo) override
    {
        mMyPhoto = photo;
    }

    ~MainWindow();

protected slots:
    void onOpenPointsFile();
    void onOpenMarkersFile();
    void onQuit();
    void onAbout();

protected:
    void closeEvent(QCloseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *e) override;
private:
    std::unique_ptr<AboutWindow> mAbout;
    Ui::MainWindow *ui;
    IMainViewEvents *mEventsInterf {nullptr};
    QGraphicsScene *mMarkersScene;
    MyTypes::SPointType mPointsRange;
    QList<MyTypes::SMarkerType> mMarkers;
    float mScaleVal {1.0};
    //QList<QVector<SPointType>> mAllpoints;
    QPixmap *mPointsPixmap {nullptr};
    //bool mIsFirstFrame {false};
    //QImage mFrame1PixMap;
    //QImage mFrame2PixMap;
    QTransform mSourceTransform;

    QString mAppName;
    QString mDescr;
    QString mVersion;
    QString mBuildDate;
    QString mContacts;
    QPixmap mMyPhoto;

    void drawMarkers(const QList<MyTypes::SMarkerType> &markers, MyTypes::SPointType pointsRange);
    void drawPoints(QPixmap &pixmap);
    void scaleGraphicsView();
    void scaleGraphView(float scale);

};

#endif // MAINWINDOW_H
