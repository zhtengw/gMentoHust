#include <QtGui>

#include "mainwindow.h"
#include "configwindow.h"
#include "authmsgwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QDialog(parent)
{

     QSettings setting("Aten","Gmentohust");


    authMW = new AuthMsgWindow;
    confW = new ConfigWindow;
    netid = new QLabel(tr("NetID:"));
    idEdit = new QLineEdit;
    netid->setBuddy(idEdit);
    idEdit->setText(setting.value("netid","").toString());

    passwd = new QLabel(tr("Password:"));
    pdEdit = new QLineEdit;
    pdEdit->setEchoMode(QLineEdit::Password);
    passwd->setBuddy(pdEdit);
    pdEdit->setText(setting.value("password","").toString());

    rembCheckBox = new QCheckBox(tr("Remember for next authification"));

    authButton = new QPushButton(tr("Authenticate"));
    authButton->setDefault(true);
    authButton->setEnabled(false);

    closeButton = new QPushButton(tr("Close"));
    configButton = new QPushButton(tr("Configure"));

    enableAuthButton("");
    connect(pdEdit, SIGNAL(textChanged(const QString&)), this, SLOT(enableAuthButton(const QString&)));
    connect(idEdit, SIGNAL(textChanged(const QString&)), this, SLOT(enableAuthButton(const QString&)));

    connect(authButton, SIGNAL(clicked()), this, SLOT(authClicked()));
    connect(authButton, SIGNAL(clicked()), this, SLOT(hide()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(configButton, SIGNAL(clicked()), confW, SLOT(show()));
    connect(authMW, SIGNAL(authMWhidden()), this, SLOT(showAndHideIcon()));
    connect(rembCheckBox, SIGNAL(stateChanged(int)), this, SLOT(saveID(int)));

    QHBoxLayout *topLeftLayout = new QHBoxLayout;
    topLeftLayout->addWidget(netid);
    topLeftLayout->addWidget(idEdit);

    QHBoxLayout *midLeftLayout = new QHBoxLayout;
    midLeftLayout->addWidget(passwd);
    midLeftLayout->addWidget(pdEdit);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(topLeftLayout);
    leftLayout->addLayout(midLeftLayout);
    leftLayout->addWidget(rembCheckBox);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(authButton);
    rightLayout->addWidget(closeButton);
    rightLayout->addWidget(configButton);
    rightLayout->addStretch();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("MentoHust"));
    setFixedHeight(sizeHint().height());

}

MainWindow::~MainWindow()
{
    
}


void MainWindow::authClicked()
{
    QString tempid = idEdit->text();
    QString temppd = pdEdit->text();

    authMW->setArgs(tempid, temppd);
    authMW->backend->start("mentohust", *authMW->args);
    if (QSystemTrayIcon::isSystemTrayAvailable())
    {
        authMW->sysTrayIcon->show();
    }
    authMW->show();

}

void MainWindow::enableAuthButton(const QString &text)
{
    authButton->setEnabled(!(pdEdit->text().isEmpty()||idEdit->text().isEmpty()));
}

void MainWindow::openCfgWd()
{
    ConfigWindow *c= new ConfigWindow;
    c->show();
}

void MainWindow::showAndHideIcon()
{
    this->show();
    authMW->sysTrayIcon->hide();
}

void MainWindow::saveID(int state)
{
    if(state == 2 )
    {
        //QMessageBox::information(this,tr("ss"),tr("I'm checked!"));
        QSettings setting("Aten","Gmentohust");
        setting.setValue("netid",idEdit->text());
        setting.setValue("password",pdEdit->text());
    }
}
