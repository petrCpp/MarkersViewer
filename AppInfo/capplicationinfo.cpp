#include "capplicationinfo.h"

CApplicationInfo::CApplicationInfo(QObject *parent) : QObject(parent)
{

}

QString CApplicationInfo::getApplicationName()
{
    return tr("Dictionary Maker");
}

QString CApplicationInfo::getAppDescription()
{
    const QString itemsFont = "<font size=\"3\" color=#0B890B>";
    const QString notImplementedFont = "<font size=\"3\" color=#740808>";
    return tr("Тестовое задание: отображение сигналов различных модуляций "
              "на комплексной плоскости.");
}

QString CApplicationInfo::getAppVersion()
{
    return tr("<b><font size=\"3\" color=#00512E>%1 </b>"
              ).arg(PROGRAM_VERSION);
}

QString CApplicationInfo::getAppBuildDate()
{
    return QString(__DATE__);
}

QString CApplicationInfo::getContacts()
{
    return tr("<b><font size=\"3\" color=#244A8C> Abrosimov Petr</b><br>"
              "<b><font size=\"3\" color=#500B8C> Software Engineer</b><br>"
              "<b><font size=\"3\" color=#00512E> Email:</b>"
              "<a href=\"mailto:petruha_1@ngs.ru\"> <cite>petruha_1@ngs.ru</cite></a>  <br>");
}

QPixmap CApplicationInfo::getmyPhoto()
{
    return QPixmap(":/resources/my_photo.jpg");
}
