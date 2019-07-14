#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>
#include <IMainViewEvents.h>
#include <IMainView.h>
#include <memory>
#include <PointsBinFile/BinFileReader.h>

// Контроллер приложения.
// В нем реализована основная бизнеслогика приложения.

class AppController : public QObject, IMainViewEvents
{
    Q_OBJECT
public:
    explicit AppController(QObject *parent = nullptr);
    ~AppController();
    // Реализация обработки событий
    bool markersXmlFileOpen(const QString &fileName) override;
    void pointsFileStartRead(const QString &fileName) override;
    void quit() override;


signals:
public slots:

private slots:
     void onPercenteges(float percentage);
     void onPorcionDataReady();
     void onErrorHappened(const QString &message);
private:
    std::unique_ptr<IMainView> mMainView;
    std::unique_ptr<BinFileReader> mBinFileReader;
    //MyTypes::SMarkerType mMarkersRange;
};

#endif // APPCONTROLLER_H
