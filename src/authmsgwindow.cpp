#include <QtGui>
#include <QProcess>

#include "authmsgwindow.h"
#include "mainwindow.h"

AuthMsgWindow::AuthMsgWindow(QWidget *parent)
        : QDialog(parent)
{
    exitButton = new QPushButton(tr("Exit"));
    miniButton = new QPushButton(tr("Minimize"));
    authMsg = new QTextEdit;
    args = new QStringList;
    sysTrayIcon = new QSystemTrayIcon(QIcon(":/mentohust.png"));

    trayActions();
    createTrayMenu();

    //connect(sysTrayIcon,SIGNAL(messageClicked()),this, SLOT(show()));
    //connect(sysTrayIcon,SIGNAL(messageClicked()),sysTrayIcon, SLOT(hide()));
    connect(sysTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,SLOT(trayIconAct(QSystemTrayIcon::ActivationReason)));

    authMsg->setReadOnly(true);

    QHBoxLayout *buttons = new QHBoxLayout;
    buttons->addWidget(miniButton);
    buttons->addWidget(exitButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(authMsg);
    mainLayout->addLayout(buttons);

    setLayout(mainLayout);
    setWindowTitle(tr("Success!"));
    resize(374,200);

    connect(exitButton, SIGNAL(clicked()), this, SLOT(exitClicked()));
    connect(miniButton, SIGNAL(clicked()), this, SLOT(miniClicked()));

    backend = new QProcess(this);
    backend->setProcessChannelMode(QProcess::MergedChannels);
    connect(backend, SIGNAL(readyReadStandardOutput()), this, SLOT(readresult()));
    connect(this, SIGNAL(authMWhidden()), authMsg, SLOT(clear()));
    //connect(backend, SIGNAL(started()), this, SLOT(readresult()));

    //backend->start("mentohust", *args);

}

AuthMsgWindow::~AuthMsgWindow()
{
}

void AuthMsgWindow::readresult()
{
    QTextCodec *data = QTextCodec::codecForName("UTF-8");
    QString result = data->toUnicode(backend->readAllStandardOutput());
    authMsg->append(result);
}

void AuthMsgWindow::setArgs(const QString &id, const QString &pd)
{

    QStringList idargs = QStringList() << "-u" << id;
    QStringList pdargs = QStringList() << "-p" << pd;
    QString idarg = idargs.join("");
    QString pdarg = pdargs.join("");
    *args = QStringList() << idarg << pdarg ;
    //args = rembCheckBox->isChecked() ? args : args << "-w" ;

}

void AuthMsgWindow::exitClicked()
{
    QProcess *exitMTH = new QProcess;
    exitMTH->start("mentohust -k");
    if(!this->isHidden())this->hide();
    emit authMWhidden();
}

void AuthMsgWindow::miniClicked()
{
    if (sysTrayIcon->isSystemTrayAvailable())
    {
        if(sysTrayIcon->supportsMessages())
        {
            //get last line of authMsg
            //authMsg->
            showMessage();
        }
        this->hide();
    }
    else this->showMinimized();
}

void AuthMsgWindow::trayIconAct(QSystemTrayIcon::ActivationReason reason)
{
    minimizeAction->setEnabled(!this->isHidden());
    restoreAction->setEnabled(this->isHidden());
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        this->setVisible(this->isHidden());
        break;
    case QSystemTrayIcon::MiddleClick:
        showMessage();
        break;
    default:
        ;
    }
}

void AuthMsgWindow::showMessage()
{
    QStringList allMessage=QString(authMsg->toPlainText()).split("\n",QString::SkipEmptyParts);
    QStringList temp = QStringList() << allMessage.at(allMessage.size()-2) << allMessage.last();
    QString message = temp.join("\n");
    sysTrayIcon->showMessage(tr("MenToHust"), message,QSystemTrayIcon::Information,5000);
}

void AuthMsgWindow::trayActions()
{
    minimizeAction = new QAction(tr("&Minimize"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));


    exitAction = new QAction(tr("&Exit"), this);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(exitClicked()));

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));


    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void AuthMsgWindow::createTrayMenu()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(exitAction);
    trayIconMenu->addAction(quitAction);

    sysTrayIcon->setContextMenu(trayIconMenu);
}
