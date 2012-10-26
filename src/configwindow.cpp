/***************************************************************************
 *  Copyright (c) 2012 by Aten Zhang <atenzd@gmail.com>                    *
 *                                                                         *
 *  This file is part of gMentoHust.                                       *
 *                                                                         *
 *  gMentoHust is free software: you can redistribute it and/or modify     *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  gMentoHust is distributed in the hope that it will be useful,          *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with gMentoHust.  If not, see <http://www.gnu.org/licenses/>.    *
 ***************************************************************************/

#include "configwindow.h"

ConfigWindow::ConfigWindow(QWidget *parent)
    : QDialog(parent)
{
    // settings
    QSettings setting("Aten","Gmentohust");

    //Widgets on configuration window
    // 1. Confirm and Exit buttons
    confirmButton = new QPushButton(tr("Confirm"));
    cancelButton = new QPushButton(tr("Cancel"));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));
    connect(confirmButton, SIGNAL(clicked()), this, SLOT(confirmClicked()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(confirmButton);
    buttonLayout->addWidget(cancelButton);

    // 2. Net cards selection
    CVNetCard = new QString;
    netCardName = new QLabel(tr("Netcard:"));
    netCardSelect = new QComboBox;
    netCardArg = new QStringList;
    netCardName->setToolTip(tr("Select the network card you used."));
    QHBoxLayout *netCardLayout = new QHBoxLayout;
    netCardLayout->addWidget(netCardName);
    netCardLayout->addWidget(netCardSelect);

    //netCardSelect->setEditable(true);
    //QLineEdit *edit = netCardSelect->lineEdit();

    getNetCard= new QProcess;
    getNetCard->start("/bin/ls -1 /proc/net/dev_snmp6");
    connect(getNetCard, SIGNAL(readyReadStandardOutput()), this, SLOT(showNetCards()));

    //set default netcard read from setting file
    if(!setting.value("netcard").isNull())
    {
        *netCardArg = QStringList()<<"-n"<<setting.value("netcard","").toString();
        *CVNetCard = setting.value("netcard","").toString();
    }
    netCardSelect->addItem(setting.value("netcard","").toString());
    connect(netCardSelect,SIGNAL(currentIndexChanged(QString)), this, SLOT(saveNetCard(QString)));

    //edit->setText("aten");
    //edit->setReadOnly(true);

    // 3. Mulcast address selection
    //CVMulAdr=0;
    mulCastAdrName = new QLabel(tr("Mulcast Address:"));
    mulCastAdrName->setToolTip(tr("Mulcast Address"));
    mulCastAdr = new QComboBox;
    mulCastAdrArg = new QStringList;
    //the index for Mulcast Address is: 0. standard 1. ruijie 2. saier
    mulCastAdr->insertItems( 0,QStringList()<<tr("standard")<<tr("ruijie")<< tr("saier"));

    QHBoxLayout *mulCastLayout = new QHBoxLayout;
    mulCastLayout->addWidget(mulCastAdrName);
    mulCastLayout->addWidget(mulCastAdr);

    //set default mulcast read from setting file, if no value read, set 0(standard)
    mulCastAdr->setCurrentIndex(setting.value("mulcastaddress",0).toInt());
    *mulCastAdrArg=QStringList()<<"-a"<<setting.value("mulcastaddress",0).toString();
    CVMulAdr = setting.value("mulcastaddress",0).toInt();
    connect(mulCastAdr,SIGNAL(currentIndexChanged(int)), this, SLOT(saveMulAdr(int)));

    // 4. time to display notification
    CVDispNotif = new QString;
    dispNotif = new QLabel(tr("Display Notification:"));
    dispNotif->setToolTip(tr("Seconds to display system notification, letters to no show."));
    dispNotifTime = new QLineEdit;
    dispNotifArg = new QStringList;
    QLabel *unitdis = new QLabel(tr("s [0(no) 1-20(yes)]"));

    //line edit width
    dispNotifTime->setMaximumWidth(22);
    dispNotifTime->setMaxLength(2);
    //set default value
    dispNotifTime->setText(setting.value("displaynotification",5).toString());
    *dispNotifArg = QStringList()<<"-y"<<setting.value("displaynotification",5).toString();
    *CVDispNotif = setting.value("displaynotification",5).toString();

    //set layout
    QHBoxLayout *dispNotifLayout = new QHBoxLayout;
    dispNotifLayout->addWidget(dispNotif);
    dispNotifLayout->addWidget(dispNotifTime);
    dispNotifLayout->addWidget(unitdis);

    connect(dispNotifTime,SIGNAL(textChanged(QString)), this, SLOT(saveDispNotif(QString)));

    // 5. DHCP type
    //CVDhcpType=0;
    dhcpTypeName = new QLabel(tr("DHCP type:"));
    dhcpTypeName->setToolTip(tr("Set the type of DHCP."));
    dhcpType = new QComboBox;
    dhcpTypeArg = new QStringList;
    //the index for DHCP type is: 0(not in used) 1(secondary authenticate) 2(post authenticate) 3(pre authenticate)
    dhcpType->insertItems( 0,QStringList()<<tr("not in used")<<tr("secondary authenticate")<< tr("post authenticate")<<tr("pre authenticate"));

    QHBoxLayout *dhcpTypeLayout = new QHBoxLayout;
    dhcpTypeLayout->addWidget(dhcpTypeName);
    dhcpTypeLayout->addWidget(dhcpType);

    //set default dhcp type read from setting file, if no value read, set 0(not in used)
    dhcpType->setCurrentIndex(setting.value("dhcptype",0).toInt());
    *dhcpTypeArg=QStringList()<<"-d"<<setting.value("dhcptype",0).toString();
    CVDhcpType = setting.value("dhcptype",0).toInt();
    connect(dhcpType,SIGNAL(currentIndexChanged(int)), this, SLOT(saveDhcpType(int)));

    // 6. authenticate timeout
    CVAuthTO = new QString;
    authTimeOutName= new QLabel(tr("Authenticate timeout:"));
    authTimeOutName->setToolTip(tr("Seconds to wait for authentication."));
    authTimeOut = new QLineEdit;
    authTimeOutArg = new QStringList;
    QLabel *unitTO = new QLabel(tr("s"));

    //line edit width
    authTimeOut->setMaximumWidth(40);
    //set default value
    authTimeOut->setText(setting.value("authtimeout",8).toString());
    *authTimeOutArg = QStringList()<<"-t"<<setting.value("authtimeout",8).toString();
    *CVAuthTO = setting.value("authtimeout",8).toString();

    //set layout
    QHBoxLayout *authTimeOutLayout = new QHBoxLayout;
    authTimeOutLayout->addWidget(authTimeOutName);
    authTimeOutLayout->addStretch();
    authTimeOutLayout->addWidget(authTimeOut);
    authTimeOutLayout->addWidget(unitTO);

    connect(authTimeOut,SIGNAL(textChanged(QString)), this, SLOT(saveAuthTimeOut(QString)));

    //

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(authTimeOutLayout);
    mainLayout->addLayout(dhcpTypeLayout);
    mainLayout->addLayout(dispNotifLayout);
    mainLayout->addLayout(mulCastLayout);
    mainLayout->addLayout(netCardLayout);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    setWindowTitle(tr("Configuration"));
    setFixedHeight(sizeHint().height());

    args=new QStringList;
    //Net cards selection



}

ConfigWindow::~ConfigWindow()
{
    
}



void ConfigWindow::showNetCards()
{
    //QString result = QString(getNetCard->readAllStandardOutput());
    QStringList netCards=QString(getNetCard->readAllStandardOutput())
            .remove(QRegExp("lo")).split("\n",QString::SkipEmptyParts);
    netCardSelect->addItems(netCards);
}

void ConfigWindow::confirmClicked()
{

    QSettings setting("Aten","Gmentohust");
    CVNetCard->isEmpty()?*netCardArg=QStringList():*netCardArg = QStringList()<<"-n"<<*CVNetCard;
    setting.setValue("netcard",*CVNetCard);

    *mulCastAdrArg = QStringList()<<"-a"<<QString::number(CVMulAdr);
    setting.setValue("mulcastaddress",CVMulAdr);

    *dispNotifArg=QStringList()<<"-y"<<*CVDispNotif;
    setting.setValue("displaynotification",CVDispNotif->toInt());

    *dhcpTypeArg = QStringList()<<"-d"<<QString::number(CVDhcpType);
    setting.setValue("dhcptype",CVDhcpType);

    *authTimeOutArg=QStringList()<<"-t"<<*CVAuthTO;
    setting.setValue("authtimeout",CVAuthTO->toInt());

    close();
}

void ConfigWindow::cancelClicked()
{
    close();
    delete this;
}

void ConfigWindow::saveNetCard(const QString name)
{
    *CVNetCard = name;
}

void ConfigWindow::saveMulAdr(const int index)
{
    CVMulAdr = index;
}

void ConfigWindow::saveDhcpType(const int index)
{
    CVDhcpType = index;
}

void ConfigWindow::saveDispNotif(const QString time)
{
    if(time.toInt()>20)
    {
        QMessageBox::warning(this,tr("Out of Range"),tr("The second value should be 0-20."));
    }
    else
    {
        *CVDispNotif = time;
    }
}

void ConfigWindow::saveAuthTimeOut(const QString time)
{
    *CVAuthTO = time;
}

void ConfigWindow::setArgs()
{
    *args = QStringList()<<netCardArg->join("")
           <<mulCastAdrArg->join("")
           <<dispNotifArg->join("")
           <<dhcpTypeArg->join("");
}
