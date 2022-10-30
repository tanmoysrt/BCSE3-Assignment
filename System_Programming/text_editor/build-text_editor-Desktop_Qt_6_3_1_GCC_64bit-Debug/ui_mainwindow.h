/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *open_from_file;
    QPushButton *save_to_file;
    QPushButton *redo_btn;
    QPushButton *undo_btn;
    QPlainTextEdit *find_word;
    QPlainTextEdit *replace_word;
    QPushButton *find_and_replace_btn;
    QLabel *text_content;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        open_from_file = new QPushButton(centralwidget);
        open_from_file->setObjectName(QString::fromUtf8("open_from_file"));
        open_from_file->setGeometry(QRect(10, 10, 89, 25));
        open_from_file->setFocusPolicy(Qt::ClickFocus);
        save_to_file = new QPushButton(centralwidget);
        save_to_file->setObjectName(QString::fromUtf8("save_to_file"));
        save_to_file->setGeometry(QRect(110, 10, 89, 25));
        save_to_file->setFocusPolicy(Qt::ClickFocus);
        redo_btn = new QPushButton(centralwidget);
        redo_btn->setObjectName(QString::fromUtf8("redo_btn"));
        redo_btn->setGeometry(QRect(210, 10, 89, 25));
        redo_btn->setFocusPolicy(Qt::ClickFocus);
        undo_btn = new QPushButton(centralwidget);
        undo_btn->setObjectName(QString::fromUtf8("undo_btn"));
        undo_btn->setGeometry(QRect(310, 10, 89, 25));
        undo_btn->setFocusPolicy(Qt::ClickFocus);
        find_word = new QPlainTextEdit(centralwidget);
        find_word->setObjectName(QString::fromUtf8("find_word"));
        find_word->setGeometry(QRect(420, 10, 111, 25));
        find_word->setFocusPolicy(Qt::ClickFocus);
        replace_word = new QPlainTextEdit(centralwidget);
        replace_word->setObjectName(QString::fromUtf8("replace_word"));
        replace_word->setGeometry(QRect(540, 10, 111, 25));
        replace_word->setFocusPolicy(Qt::ClickFocus);
        find_and_replace_btn = new QPushButton(centralwidget);
        find_and_replace_btn->setObjectName(QString::fromUtf8("find_and_replace_btn"));
        find_and_replace_btn->setGeometry(QRect(670, 10, 121, 25));
        find_and_replace_btn->setFocusPolicy(Qt::ClickFocus);
        text_content = new QLabel(centralwidget);
        text_content->setObjectName(QString::fromUtf8("text_content"));
        text_content->setGeometry(QRect(10, 50, 771, 511));
        text_content->setFocusPolicy(Qt::ClickFocus);
        text_content->setAutoFillBackground(false);
        text_content->setStyleSheet(QString::fromUtf8("background-color : rgb(255, 255, 255);"));
        text_content->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        text_content->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Simple Text Editor", nullptr));
        open_from_file->setText(QCoreApplication::translate("MainWindow", "Open", nullptr));
        save_to_file->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
        redo_btn->setText(QCoreApplication::translate("MainWindow", "Redo", nullptr));
        undo_btn->setText(QCoreApplication::translate("MainWindow", "Undo", nullptr));
        find_and_replace_btn->setText(QCoreApplication::translate("MainWindow", "Find & Replace", nullptr));
        text_content->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
