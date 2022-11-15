#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QObject>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QFontDatabase fontDB;
    qDebug() << QFontDatabase::families();
    ui->content->setFont(fontDB.font("Noto Sans Bengali", "regular", 20));

    QByteArray hex =  QByteArray::fromHex("0x0990");

    ui->content->setText(QString::fromUtf16((char16_t*)hex.data()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Key pressed event
void MainWindow::keyPressEvent(QKeyEvent *event){
    int key =  event->key();

    // Handle shift
    if(key == 16777248){
        isUpperCase = true;
    }

    // Handle Caps Lock
    if(key == 16777252){
        isUpperCase = !isUpperCase;
    }

    // Filter Special characters
    if((key >= 33 && key <= 64) || (key >= 91 && key <= 96) || (key >= 123 && key <= 126)){
        // Can add directly to text
        addNewCharacter(key);
    }

    // Filter only A-Z characters
    if(key >= 65 && key <= 90){
        if(isUpperCase){
            addNewCharacter(key);
        }else{
            // add key+32
            addNewCharacter(key+32);
        }
    }

    if(key == 32){
        // add space
        addNewCharacter(' ');
    }

    if(key == 16777220){// add direcctly
        // add new line
        addNewCharacter('\n');
    }

    if(key == 16777219){
        // backspace
        removeLastCharacter();
    }

    refreshUI();
}


// Key release event
void MainWindow::keyReleaseEvent(QKeyEvent *event){
    int key =  event->key();

    // Handle Shift
    if(key == 16777248){
        isUpperCase = false;
    }
}

// Add new character
void MainWindow::addNewCharacter(int c){

}

void MainWindow::refreshUI(){

}

void MainWindow::removeLastCharacter(){

}
