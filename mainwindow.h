#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QString>
#include <ui_mainwindow.h>
#include <QList>

class memoitem{
public:
    int done;
    QString contanct;
    QString begintime;
    QString endtime;
    int row;
};

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

public:
    QString timeRecord;
    QList<memoitem> memoItem;
    explicit MainWindow(QWidget *parent = 0);
    void initToDoList();
    void closeEvent( QCloseEvent * event );
    int findMemoItemIndex(QString s, int row);
    void showDone();
    void showTodo();
private slots:
    void addList();
    void deleteList();
    void addButtonEnableFunc();
    void doneButtonEnableFunc();
    void saveBeforeExit();
    void dateShow();
    void cleanCookie();
};

#endif // MAINWINDOW_H
