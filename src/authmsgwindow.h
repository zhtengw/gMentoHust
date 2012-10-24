#ifndef AUTHMSGWINDOW_H
#define AUTHMSGWINDOW_H

#include <QDialog>
#include <QSystemTrayIcon>

class QTextEdit;
class QPushButton;
class QProcess;
class QString;
class QIcon;
class QMenu;

class AuthMsgWindow : public QDialog
{
    Q_OBJECT

public:
    AuthMsgWindow(QWidget *parent = 0);
    ~AuthMsgWindow();

    void setArgs(const QString &id, const QString &pd);

    friend class MainWindow;

signals:
    void authMWhidden();

private slots:
    void readresult();
    void exitClicked();
    void miniClicked();
    void trayIconAct(QSystemTrayIcon::ActivationReason reason);
    void showMessage();

private:
    void trayActions();
    void createTrayMenu();

    QTextEdit *authMsg;
    QPushButton *exitButton;
    QPushButton *miniButton;
    QProcess *backend;
    QStringList *args;
    QSystemTrayIcon *sysTrayIcon;
    QMenu *trayIconMenu;
    QAction *minimizeAction;
    QAction *exitAction;
    QAction *restoreAction;
    QAction *quitAction;
    //QIcon *icon;
};

#endif // AUTHMSGWINDOW_H
