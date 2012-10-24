#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QMessageBox>
#include <QSettings>

#include <authmsgwindow.h>
#include <configwindow.h>

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QProcess;
class AuthMsgWindow;

class MainWindow : public QDialog //QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    friend class AuthMsgWindow;
private slots:
    void authClicked();
    void enableAuthButton(const QString &text);
    void openCfgWd();
    void showAndHideIcon();
    void saveID(int state);

private:
    QLabel *netid;
    QLabel *passwd;
    QLineEdit *idEdit;
    QLineEdit *pdEdit;
    QCheckBox *rembCheckBox;
    QPushButton *authButton;
    QPushButton *closeButton;
    QPushButton *configButton;
    AuthMsgWindow *authMW;
    ConfigWindow *confW;
};

#endif // MAINWINDOW_H
