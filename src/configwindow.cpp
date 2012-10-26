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
#include <QtGui>
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
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(confirmButton, SIGNAL(clicked()), this, SLOT(confirmClicked()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(confirmButton);
    buttonLayout->addWidget(cancelButton);

    // 2. Net cards selection
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
    if(!setting.value("netcard").isNull()){*netCardArg = QStringList()<<"-n"<<setting.value("netcard","").toString();}
    netCardSelect->addItem(setting.value("netcard","").toString());
    connect(netCardSelect,SIGNAL(currentIndexChanged(QString)), this, SLOT(saveNetCard(QString)));

    //edit->setText("aten");
    //edit->setReadOnly(true);

    // 3. Mulcast address selection
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
    connect(mulCastAdr,SIGNAL(currentIndexChanged(int)), this, SLOT(saveMulAdr(int)));

    // 4. time to display notification
    dispNotif = new QLabel(tr("Display Notification:"));
    dispNotif->setToolTip(tr("Seconds to display system notification, letters to no show."));
    dispNotifTime = new QLineEdit;
    dispNotifArg = new QStringList;
    QLabel *unit = new QLabel(tr("s [0(no) 1-20(yes)]"));

    //line edit width
    dispNotifTime->setMaximumWidth(22);
    dispNotifTime->setMaxLength(2);
    //set default value
    dispNotifTime->setText(setting.value("displaynotification",5).toString());
    *dispNotifArg = QStringList()<<"-y"<<setting.value("displaynotification",5).toString();

    //set layout
    QHBoxLayout *dispNotifLayout = new QHBoxLayout;
    dispNotifLayout->addWidget(dispNotif);
    dispNotifLayout->addWidget(dispNotifTime);
    dispNotifLayout->addWidget(unit);

    connect(dispNotifTime,SIGNAL(textChanged(QString)), this, SLOT(saveDispNotif(QString)));


    //

    QVBoxLayout *mainLayout = new QVBoxLayout;
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

// old function to create selection widget

//void ConfigWindow::createNetCardSelect()
//{
//    netCardName = new QLabel(tr("Netcard:"));
//    netCardSelect = new QComboBox;
//    netCardName->setToolTip(tr("Select the network card you used."));
//    QHBoxLayout *netCardLayout = new QHBoxLayout;
//    netCardLayout->addWidget(netCardName);
//    netCardLayout->addWidget(netCardSelect);
//}

//void ConfigWindow::createMulcastSelect()
//{
//    mulCastAdrName = new QLabel(tr("Mulcast Address:"));
//    mulCastAdrName->setToolTip(tr("Mulcast Address"));
//    mulCastAdr = new QButtonGroup;
//    mulCastAdr->setExclusive(true);
//    standard = new QCheckBox(tr("standard"));
//    standard->setChecked(true);
//    ruijie = new QCheckBox(tr("ruijie"));
//    saier = new QCheckBox(tr("saier"));
//    mulCastAdr->addButton(standard,0);
//    mulCastAdr->addButton(ruijie,1);
//    mulCastAdr->addButton(saier,2);

//    QHBoxLayout *mulCastLayout = new QHBoxLayout;
//    mulCastLayout->addWidget(mulCastAdrName);
//    mulCastLayout->addWidget(standard);
//    mulCastLayout->addWidget(ruijie);
//    mulCastLayout->addWidget(saier);

//}

void ConfigWindow::showNetCards()
{
    //QString result = QString(getNetCard->readAllStandardOutput());
    QStringList netCards=QString(getNetCard->readAllStandardOutput())
            .remove(QRegExp("lo")).split("\n",QString::SkipEmptyParts);
    netCardSelect->addItems(netCards);
}

//it seems that it's no necessity to use confirm button
//shall I get it to be used?
void ConfigWindow::confirmClicked()
{
    close();
    delete this;
}

void ConfigWindow::saveNetCard(const QString name)
{
    *netCardArg = QStringList()<<"-n"<<name;
    QSettings setting("Aten","Gmentohust");
    setting.setValue("netcard",name);
}

void ConfigWindow::saveMulAdr(const int index)
{
    *mulCastAdrArg = QStringList()<<"-a"<<QString::number(index);
    QSettings setting("Aten","Gmentohust");
    setting.setValue("mulcastaddress",index);
}

void ConfigWindow::saveDispNotif(const QString time)
{
    if(time.toInt()>20)
    {
        QMessageBox::warning(this,tr("Out of Range"),tr("The second value should be 0-20."));
    }
    else
    {
        *dispNotifArg=QStringList()<<"-y"<<time;
        QSettings setting("Aten","Gmentohust");
        setting.setValue("displaynotification",time.toInt());
    }
}

void ConfigWindow::setArgs()
{
    *args = QStringList()<<netCardArg->join("")<<mulCastAdrArg->join("");
}
