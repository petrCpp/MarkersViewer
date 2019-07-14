#ifndef CAPPLICATIONINFO_H
#define CAPPLICATIONINFO_H

#include <QObject>
#include <QString>
#include <QPixmap>

class CApplicationInfo : public QObject
{
    Q_OBJECT
public:
    explicit CApplicationInfo(QObject *parent = nullptr);
    QString getApplicationName();
    QString getAppDescription();
    QString getAppVersion();
    QString getAppBuildDate();
    QString getContacts();
    QPixmap getmyPhoto();
signals:

public slots:
};

#endif // CAPPLICATIONINFO_H
