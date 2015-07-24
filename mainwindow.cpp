#include "mainwindow.h"
#include <QtCore>
#include <QFile>
#include <QTextCodec>
#include <QMessageBox>
#include <QDateTime>
#include <QList>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    connect(addButton,SIGNAL(clicked()),this,SLOT(addList()));
    connect(doneButton,SIGNAL(clicked()),this,SLOT(deleteList()));
    connect(clean,SIGNAL(clicked()),this,SLOT(cleanCookie()));
    connect(lineEdit,SIGNAL(textChanged(QString)),this,SLOT(addButtonEnableFunc()));
    connect(listWidget,SIGNAL(currentRowChanged(int)),this,SLOT(doneButtonEnableFunc()));
    connect(listWidget,SIGNAL(currentRowChanged(int)),this,SLOT(dateShow()));
    initToDoList();
}

void MainWindow::addList(){
    memoitem item;
    item.done=0;

    item.contanct = lineEdit->text();

    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss ddd");
    item.begintime = str;
    item.row = -1;

    memoItem.append(item);

    lineEdit->clear();

    showTodo();
}

void MainWindow::deleteList(){
    if (listWidget->currentItem()==NULL){
        doneButton->setEnabled(false);
        return ;
    }

    QString contant = listWidget->currentItem()->text();
    qDebug()<<"crnt row:"<<listWidget->currentRow()<<endl;

    int index = findMemoItemIndex(contant,listWidget->currentRow());
    memoItem[index].done=1;
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss ddd");
    memoItem[index].endtime = str;

    //listWidget->takeItem(listWidget->currentRow());
    showTodo();
    showDone();

}

void MainWindow::addButtonEnableFunc(){
    addButton->setEnabled(!(lineEdit->text().isEmpty()));
}
void MainWindow::doneButtonEnableFunc(){
    doneButton->setEnabled(listWidget->currentItem()!=NULL);
}

void MainWindow::cleanCookie(){
    QList<memoitem>::iterator i;
    i=memoItem.begin();
    for (i=memoItem.begin(); i!=memoItem.end(); i++){
        if((*i).done==1){
            i=memoItem.erase(i);
            i--;
        }
    }
    /*
    for (int i=0; i<memoItem.count(); i++){
        if (memoItem[i].done==1)
            memoItem.removeAt(i);
    }*/
    qDebug()<<memoItem.count();
    doneList->clear();
}

void MainWindow::initToDoList(){

    QFile file("List.data");
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        return;
    }

    QTextStream in(&file);
    while( !in.atEnd()){
        memoitem item;

        QString line = in.readLine();
        (line=="0")? (item.done=0) : (item.done=1);

        line = in.readLine();
        item.contanct = line;
        line = in.readLine();
        item.begintime = line;

        if (item.done==1){
            line = in.readLine();
            item.endtime = line;
        }

        memoItem.append(item);
    }
    in.flush();
    file.close();

    showTodo();
    showDone();
}

void MainWindow::saveBeforeExit(){

    QFile file("List.data");
    file.remove();
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream out(&file);
    for (int i=0; i<memoItem.count(); i++){
        out<<memoItem[i].done<<endl;
        out<<memoItem[i].contanct<<endl;
        out<<memoItem[i].begintime<<endl;
        if(memoItem[i].done==1)
            out<<memoItem[i].endtime<<endl;
    }
    out.flush();
    file.close();
}

void MainWindow::dateShow(){
    if (!listWidget->currentItem()){
        dateLabel->setText("");
        return ;
    }
    int index = findMemoItemIndex(listWidget->currentItem()->text(),listWidget->currentRow());
    if (index>=0)
        dateLabel->setText(memoItem[index].begintime);
}

void MainWindow::closeEvent( QCloseEvent * event )
{
    saveBeforeExit();
    event->accept();
}

inline int MainWindow::findMemoItemIndex(QString s,int row){
    for(int i=0; i<memoItem.count(); i++){
        qDebug()<<memoItem[i].contanct<<"  "<<memoItem[i].row<<endl;
        if (memoItem[i].contanct == s && memoItem[i].row == row)
            return i;
    }
    return -1;
}

void MainWindow::showDone(){
    doneList->clear();
    for(int i=0; i<memoItem.count(); i++){
        if (memoItem[i].done==1){
            QString temp = memoItem[i].contanct;
            temp += "   ";
            temp += memoItem[i].begintime;
            temp += "--";
            temp += memoItem[i].endtime;
            doneList->addItem(temp);
        }
    }
}
void MainWindow::showTodo(){
    listWidget->clear();
    int count = 0;
    for(int i=0; i<memoItem.count(); i++){
        if (memoItem[i].done==0){
            listWidget->addItem(memoItem[i].contanct);
            memoItem[i].row = count;
            count++;
        }
    }
}
