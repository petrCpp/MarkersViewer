#ifndef IMAINVIEW_H
#define IMAINVIEW_H


#include <IMainViewEvents.h>
#include <QList>
#include <mysimpletypes.h>
#include <memory>
#include <QPixmap>

// Интерфейс главного представления (вида)
class IMainView
{
public:
    virtual void setMainViewEventsInterface(IMainViewEvents *interf) = 0;
    virtual void show() = 0;
    virtual void hide() = 0;
    virtual void showMarkers(const QList<MyTypes::SMarkerType> &markers,
                            MyTypes::SPointType pointsRange) = 0;
    virtual void showPoints(QPixmap& pixmap) = 0;
    virtual void showProgress(float percentage) = 0;
    virtual void showErrorMessage(const QString &message) = 0;

    virtual void setAppName(const QString &appName) = 0;
    virtual void setAppDescription(const QString &appDescription) = 0;
    virtual void setAppVersion(const QString &appVersion) = 0;
    virtual void setAppBuildDate(const QString &buildDate) = 0;
    virtual void setContacts(const QString &contacts) = 0;
    virtual void setMyPhoto(const QPixmap &photo) = 0;


    virtual ~IMainView() = 0;
};

#endif // IMAINVIEW_H
