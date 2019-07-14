#include "aboutwindow.h"
#include "ui_aboutwindow.h"

AboutWindow::AboutWindow(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::AboutWindow)
{
    ui->setupUi(this);
}

AboutWindow::~AboutWindow()
{
    delete ui;
}


void AboutWindow::setAppDescription(const QString &descr)
{
    //mDescr = descr;
    ui->descriptionEdt->setText(descr);
}

void AboutWindow::setAppVersion(const QString &version)
{
    mVersion = version;
    showVersionDate();
}

void AboutWindow::setAppBuildDate(const QString &buildDate)
{
    mBuildDate = buildDate;
    showVersionDate();
}


void AboutWindow::showVersionDate()
{
    ui->progVersionLbl->setText(QString("<b>%1 built %2</b> <i></i>").
                                arg(mVersion).
                                arg(mBuildDate));
}


void AboutWindow::setContacts(const QString &contacts)
{
    ui->contactsEdt->setText(contacts);
}

void AboutWindow::setMyPhoto(const QPixmap &myPhoto)
{
    ui->myPhotoLbl->setPixmap(myPhoto);
}
