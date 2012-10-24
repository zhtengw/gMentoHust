#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include <QDialog>

class QPushButton;
class QTextEdit;
class QProcess;
class QComboBox;
class QLabel;
class QButtonGroup;
class QCheckBox;

class ConfigWindow : public QDialog
{
    Q_OBJECT
public:
    ConfigWindow(QWidget *parent = 0);
    ~ConfigWindow();


private slots:
    void confirmClicked();
    void showNetCards();
		
private:
    //void createNetCardSelect();
    //void createMulcastSelect();

    QPushButton *confirmButton;
    QPushButton *cancelButton;
    QTextEdit *temp;

    QLabel *netCardName;
    QProcess *getNetCard;
    QComboBox *netCardSelect;

    QLabel *mulCastAdrName;
    QComboBox *mulCastAdr;
    //QCheckBox *standard;
    //QCheckBox *ruijie;
    //QCheckBox *saier;
};

#endif // CONFIGWINDOW_H
