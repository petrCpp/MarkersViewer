#ifndef IMAINVIEWEVENTS_H
#define IMAINVIEWEVENTS_H

#include <QString>
// Вызовы позднего связывания от главного представления
// приложения
class IMainViewEvents
{
public:
    virtual bool markersXmlFileOpen(const QString &fileName) = 0;
    virtual void pointsFileStartRead(const QString &fileName) = 0;
    virtual void quit() = 0;
};

#endif // IMAINVIEWEVENTS_H
