#include <QtGui>
#include "configwindow.h"

ConfigWindow::ConfigWindow(QWidget *parent)
    : QDialog(parent)
{
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
    netCardName->setToolTip(tr("Select the network card you used."));
    QHBoxLayout *netCardLayout = new QHBoxLayout;
    netCardLayout->addWidget(netCardName);
    netCardLayout->addWidget(netCardSelect);

    getNetCard= new QProcess;
    getNetCard->start("/bin/ls -1 /proc/net/dev_snmp6");
    connect(getNetCard, SIGNAL(readyReadStandardOutput()), this, SLOT(showNetCards()));

    // 3. Mulcast address selection
    mulCastAdrName = new QLabel(tr("Mulcast Address:"));
    mulCastAdrName->setToolTip(tr("Mulcast Address"));
    mulCastAdr = new QComboBox;
    mulCastAdr->addItems(QStringList()<<tr("standard")<<tr("ruijie")<< tr("saier"));

    QHBoxLayout *mulCastLayout = new QHBoxLayout;
    mulCastLayout->addWidget(mulCastAdrName);
    mulCastLayout->addWidget(mulCastAdr);

    // 4. time to display notification

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(mulCastLayout);
    mainLayout->addLayout(netCardLayout);
    mainLayout->addLayout(buttonLayout);
    //midLayout->addWidget(temp);

    setLayout(mainLayout);

    setWindowTitle(tr("Configuration"));
    setFixedHeight(sizeHint().height());

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

void ConfigWindow::confirmClicked()
{
    close();
}
