#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QObject>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    ui->redo_btn->setDisabled(true);
    ui->undo_btn->setDisabled(true);

    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()),this ,SLOT(addPosToUndoStack()));
}

MainWindow::~MainWindow(){
    delete ui;
}

/// Keyboard Key Controller

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

/// Operations

// Add new character
void MainWindow::addNewCharacter(char c){
    text = text + c;
    characterCount++;
    redoStack.clear();

    // Avoid add new character to undo stack isntantly
    // Stop previous timer to take any action
    timer->stop();
    // Start new timer with 1 second delay
    timer->start(1000);
}

void MainWindow::addPosToUndoStack(){
    undoStack.push(characterCount);
}

// Remove last character on backspace click
void MainWindow::removeLastCharacter(){
    int len = text.length();
    if(len == 0) return;
    QChar c = text.at(len-1);
    text.chop(1);
    redoStack.append(c);
}

// Redo
void MainWindow::on_redo_btn_clicked(){
    if(redoStack.empty()) return;
    QString add = redoStack.pop();
    text = text + add;
    refreshUI();
}

// Undo
void MainWindow::on_undo_btn_clicked()
{
    if(undoStack.empty()) return;
    int len = text.length();
    int pos = undoStack.pop();
    redoStack.push(text.right(len-pos+1));
    text.chop(len-pos+1);
    refreshUI();
}

// Refresh UI
void MainWindow::refreshUI(){
    text = text.remove("<span style=\"background-color:yellow\">");
    text = text.remove("</span>");
    ui->text_content->setText(text);
    ui->redo_btn->setDisabled(redoStack.empty());
    ui->undo_btn->setDisabled(undoStack.empty());
}

// Find and replace word
void MainWindow::on_find_and_replace_btn_clicked()
{
    if(ui->find_word->toPlainText().isEmpty()) return;
    text = text.replace(ui->find_word->toPlainText(), ui->replace_word->toPlainText());
    characterCount = text.length();
    refreshUI();
}

// Save text to file
void MainWindow::on_save_to_file_clicked(){
    QString filename = QFileDialog::getSaveFileName();
    QFile f(filename);
    f.open(QIODevice::WriteOnly);
    QByteArray ba = text.toLocal8Bit();
    const char *char_arr = ba.data();
    f.write(char_arr);
    f.close();
}

// Import text content from file
void MainWindow::on_open_from_file_clicked()
{
    QString filename = QFileDialog::getOpenFileName();
    QFile f(filename);
    f.open(QIODevice::ReadOnly);
    QByteArray ba = f.readAll();
    text = QString(ba);
    f.close();
    refreshUI();
}



void MainWindow::on_find_btn_clicked(){
    refreshUI();
    text = text.replace(ui->find_word->toPlainText(), "<span style=\"background-color:yellow\">" + ui->find_word->toPlainText() + "</span>");
    ui->text_content->setText(text);
}

