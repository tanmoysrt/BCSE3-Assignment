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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <my_label.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    my_label *frame;
    QLabel *mouse_movement;
    QLabel *label_3;
    QLabel *mouse_pressed;
    QLabel *label_5;
    QFrame *x_axis;
    QFrame *y_axis;
    QCheckBox *show_axes;
    QPushButton *Draw;
    QSpinBox *circle_radius;
    QRadioButton *draw_circle;
    QRadioButton *draw_line;
    QPushButton *set_point1;
    QPushButton *set_point2;
    QPushButton *pushButton;
    QSpinBox *grid_size;
    QCheckBox *show_grid;
    QFrame *line;
    QFrame *line_3;
    QLabel *label;
    QFrame *line_4;
    QFrame *line_6;
    QGroupBox *groupBox;
    QRadioButton *ddaRadio;
    QRadioButton *bresenhamRadio;
    QPushButton *set_mid_point_circle;
    QGroupBox *groupBox_2;
    QRadioButton *bresenhamRadioCircle;
    QRadioButton *polarRadioCircle;
    QFrame *line_5;
    QCheckBox *move_axis_checkbox;
    QFrame *line_7;
    QLabel *label_2;
    QLabel *time_taken;
    QPushButton *set_another_point_circle;
    QFrame *line_8;
    QFrame *line_9;
    QRadioButton *draw_ellipse;
    QSpinBox *ellipseA;
    QSpinBox *ellipseB;
    QPushButton *set_center_point_ellipse;
    QFrame *line_2;
    QPushButton *boundary_fill_btn;
    QPushButton *flood_fill_btn;
    QLabel *label_4;
    QPushButton *boundary_fill_8_connected_btn;
    QPushButton *flood_fill_btn_8_connected;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1382, 736);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        frame = new my_label(centralWidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(0, 0, 700, 700));
        frame->setCursor(QCursor(Qt::CrossCursor));
        frame->setMouseTracking(true);
        frame->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        frame->setFrameShape(QFrame::Box);
        frame->setLineWidth(1);
        mouse_movement = new QLabel(centralWidget);
        mouse_movement->setObjectName(QString::fromUtf8("mouse_movement"));
        mouse_movement->setGeometry(QRect(720, 40, 111, 31));
        mouse_movement->setFrameShape(QFrame::Panel);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(710, 10, 131, 20));
        mouse_pressed = new QLabel(centralWidget);
        mouse_pressed->setObjectName(QString::fromUtf8("mouse_pressed"));
        mouse_pressed->setGeometry(QRect(870, 40, 111, 31));
        mouse_pressed->setFrameShape(QFrame::Panel);
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(870, 10, 111, 20));
        x_axis = new QFrame(centralWidget);
        x_axis->setObjectName(QString::fromUtf8("x_axis"));
        x_axis->setGeometry(QRect(0, 0, 700, 1));
        x_axis->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 85, 255);"));
        x_axis->setLineWidth(0);
        x_axis->setMidLineWidth(0);
        x_axis->setFrameShape(QFrame::HLine);
        x_axis->setFrameShadow(QFrame::Sunken);
        y_axis = new QFrame(centralWidget);
        y_axis->setObjectName(QString::fromUtf8("y_axis"));
        y_axis->setGeometry(QRect(0, 0, 1, 700));
        y_axis->setStyleSheet(QString::fromUtf8("\n"
"background-color: rgb(85, 255, 255);"));
        y_axis->setLineWidth(0);
        y_axis->setFrameShape(QFrame::VLine);
        y_axis->setFrameShadow(QFrame::Sunken);
        show_axes = new QCheckBox(centralWidget);
        show_axes->setObjectName(QString::fromUtf8("show_axes"));
        show_axes->setGeometry(QRect(710, 90, 111, 31));
        show_axes->setChecked(true);
        Draw = new QPushButton(centralWidget);
        Draw->setObjectName(QString::fromUtf8("Draw"));
        Draw->setGeometry(QRect(720, 620, 101, 41));
        circle_radius = new QSpinBox(centralWidget);
        circle_radius->setObjectName(QString::fromUtf8("circle_radius"));
        circle_radius->setGeometry(QRect(880, 250, 91, 31));
        circle_radius->setMinimum(1);
        circle_radius->setMaximum(500);
        circle_radius->setValue(1);
        draw_circle = new QRadioButton(centralWidget);
        draw_circle->setObjectName(QString::fromUtf8("draw_circle"));
        draw_circle->setGeometry(QRect(710, 250, 121, 31));
        draw_line = new QRadioButton(centralWidget);
        draw_line->setObjectName(QString::fromUtf8("draw_line"));
        draw_line->setGeometry(QRect(710, 490, 101, 31));
        set_point1 = new QPushButton(centralWidget);
        set_point1->setObjectName(QString::fromUtf8("set_point1"));
        set_point1->setGeometry(QRect(840, 490, 71, 31));
        set_point2 = new QPushButton(centralWidget);
        set_point2->setObjectName(QString::fromUtf8("set_point2"));
        set_point2->setGeometry(QRect(920, 490, 71, 31));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(870, 620, 111, 41));
        grid_size = new QSpinBox(centralWidget);
        grid_size->setObjectName(QString::fromUtf8("grid_size"));
        grid_size->setGeometry(QRect(880, 160, 91, 31));
        grid_size->setMinimum(1);
        grid_size->setMaximum(120);
        grid_size->setValue(5);
        show_grid = new QCheckBox(centralWidget);
        show_grid->setObjectName(QString::fromUtf8("show_grid"));
        show_grid->setGeometry(QRect(710, 120, 121, 31));
        line = new QFrame(centralWidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(700, 600, 301, 16));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        line_3 = new QFrame(centralWidget);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setGeometry(QRect(700, 80, 301, 16));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(730, 160, 131, 31));
        line_4 = new QFrame(centralWidget);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        line_4->setGeometry(QRect(700, 190, 301, 16));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);
        line_6 = new QFrame(centralWidget);
        line_6->setObjectName(QString::fromUtf8("line_6"));
        line_6->setGeometry(QRect(700, 660, 681, 16));
        line_6->setFrameShape(QFrame::HLine);
        line_6->setFrameShadow(QFrame::Sunken);
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(710, 540, 281, 61));
        ddaRadio = new QRadioButton(groupBox);
        ddaRadio->setObjectName(QString::fromUtf8("ddaRadio"));
        ddaRadio->setGeometry(QRect(20, 30, 81, 23));
        ddaRadio->setChecked(true);
        bresenhamRadio = new QRadioButton(groupBox);
        bresenhamRadio->setObjectName(QString::fromUtf8("bresenhamRadio"));
        bresenhamRadio->setGeometry(QRect(150, 30, 112, 23));
        set_mid_point_circle = new QPushButton(centralWidget);
        set_mid_point_circle->setObjectName(QString::fromUtf8("set_mid_point_circle"));
        set_mid_point_circle->setGeometry(QRect(710, 290, 131, 31));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(710, 410, 281, 61));
        bresenhamRadioCircle = new QRadioButton(groupBox_2);
        bresenhamRadioCircle->setObjectName(QString::fromUtf8("bresenhamRadioCircle"));
        bresenhamRadioCircle->setGeometry(QRect(20, 30, 111, 23));
        bresenhamRadioCircle->setChecked(true);
        polarRadioCircle = new QRadioButton(groupBox_2);
        polarRadioCircle->setObjectName(QString::fromUtf8("polarRadioCircle"));
        polarRadioCircle->setGeometry(QRect(150, 30, 112, 23));
        line_5 = new QFrame(centralWidget);
        line_5->setObjectName(QString::fromUtf8("line_5"));
        line_5->setGeometry(QRect(700, 140, 301, 16));
        line_5->setFrameShape(QFrame::HLine);
        line_5->setFrameShadow(QFrame::Sunken);
        move_axis_checkbox = new QCheckBox(centralWidget);
        move_axis_checkbox->setObjectName(QString::fromUtf8("move_axis_checkbox"));
        move_axis_checkbox->setGeometry(QRect(860, 90, 92, 23));
        line_7 = new QFrame(centralWidget);
        line_7->setObjectName(QString::fromUtf8("line_7"));
        line_7->setGeometry(QRect(700, 230, 301, 16));
        line_7->setFrameShape(QFrame::HLine);
        line_7->setFrameShadow(QFrame::Sunken);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(720, 210, 91, 17));
        time_taken = new QLabel(centralWidget);
        time_taken->setObjectName(QString::fromUtf8("time_taken"));
        time_taken->setGeometry(QRect(840, 210, 141, 17));
        set_another_point_circle = new QPushButton(centralWidget);
        set_another_point_circle->setObjectName(QString::fromUtf8("set_another_point_circle"));
        set_another_point_circle->setGeometry(QRect(860, 290, 131, 31));
        line_8 = new QFrame(centralWidget);
        line_8->setObjectName(QString::fromUtf8("line_8"));
        line_8->setGeometry(QRect(700, 470, 301, 16));
        line_8->setFrameShape(QFrame::HLine);
        line_8->setFrameShadow(QFrame::Sunken);
        line_9 = new QFrame(centralWidget);
        line_9->setObjectName(QString::fromUtf8("line_9"));
        line_9->setGeometry(QRect(700, 320, 301, 16));
        line_9->setFrameShape(QFrame::HLine);
        line_9->setFrameShadow(QFrame::Sunken);
        draw_ellipse = new QRadioButton(centralWidget);
        draw_ellipse->setObjectName(QString::fromUtf8("draw_ellipse"));
        draw_ellipse->setGeometry(QRect(710, 330, 121, 31));
        ellipseA = new QSpinBox(centralWidget);
        ellipseA->setObjectName(QString::fromUtf8("ellipseA"));
        ellipseA->setGeometry(QRect(870, 340, 44, 26));
        ellipseA->setMinimum(1);
        ellipseB = new QSpinBox(centralWidget);
        ellipseB->setObjectName(QString::fromUtf8("ellipseB"));
        ellipseB->setGeometry(QRect(940, 340, 44, 26));
        ellipseB->setMinimum(1);
        set_center_point_ellipse = new QPushButton(centralWidget);
        set_center_point_ellipse->setObjectName(QString::fromUtf8("set_center_point_ellipse"));
        set_center_point_ellipse->setGeometry(QRect(720, 370, 131, 31));
        line_2 = new QFrame(centralWidget);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setGeometry(QRect(993, 0, 20, 661));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);
        boundary_fill_btn = new QPushButton(centralWidget);
        boundary_fill_btn->setObjectName(QString::fromUtf8("boundary_fill_btn"));
        boundary_fill_btn->setGeometry(QRect(1020, 40, 341, 31));
        flood_fill_btn = new QPushButton(centralWidget);
        flood_fill_btn->setObjectName(QString::fromUtf8("flood_fill_btn"));
        flood_fill_btn->setGeometry(QRect(1020, 140, 341, 31));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(1140, 10, 111, 17));
        boundary_fill_8_connected_btn = new QPushButton(centralWidget);
        boundary_fill_8_connected_btn->setObjectName(QString::fromUtf8("boundary_fill_8_connected_btn"));
        boundary_fill_8_connected_btn->setGeometry(QRect(1020, 90, 341, 31));
        flood_fill_btn_8_connected = new QPushButton(centralWidget);
        flood_fill_btn_8_connected->setObjectName(QString::fromUtf8("flood_fill_btn_8_connected"));
        flood_fill_btn_8_connected->setGeometry(QRect(1020, 190, 341, 31));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1382, 22));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        frame->setText(QString());
        mouse_movement->setText(QString());
        label_3->setText(QCoreApplication::translate("MainWindow", " Mouse Movement", nullptr));
        mouse_pressed->setText(QString());
        label_5->setText(QCoreApplication::translate("MainWindow", "Mouse Pressed", nullptr));
        show_axes->setText(QCoreApplication::translate("MainWindow", "Show Axes", nullptr));
        Draw->setText(QCoreApplication::translate("MainWindow", "Draw", nullptr));
        draw_circle->setText(QCoreApplication::translate("MainWindow", "Draw Circle", nullptr));
        draw_line->setText(QCoreApplication::translate("MainWindow", "Draw Line", nullptr));
        set_point1->setText(QCoreApplication::translate("MainWindow", "Point 1", nullptr));
        set_point2->setText(QCoreApplication::translate("MainWindow", "Point 2", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "RESET", nullptr));
        show_grid->setText(QCoreApplication::translate("MainWindow", "Show Grid", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Grid Size", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "Select Method", nullptr));
        ddaRadio->setText(QCoreApplication::translate("MainWindow", "DDA", nullptr));
        bresenhamRadio->setText(QCoreApplication::translate("MainWindow", "Bresenham", nullptr));
        set_mid_point_circle->setText(QCoreApplication::translate("MainWindow", "Set center point", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "Select Method", nullptr));
        bresenhamRadioCircle->setText(QCoreApplication::translate("MainWindow", "Bresenham", nullptr));
        polarRadioCircle->setText(QCoreApplication::translate("MainWindow", "Polar", nullptr));
        move_axis_checkbox->setText(QCoreApplication::translate("MainWindow", "Move Axis", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Time taken", nullptr));
        time_taken->setText(QString());
        set_another_point_circle->setText(QCoreApplication::translate("MainWindow", "Set another point", nullptr));
        draw_ellipse->setText(QCoreApplication::translate("MainWindow", "Draw Ellipse", nullptr));
        set_center_point_ellipse->setText(QCoreApplication::translate("MainWindow", "Set center point", nullptr));
        boundary_fill_btn->setText(QCoreApplication::translate("MainWindow", "Boundary Fill 4 Connected", nullptr));
        flood_fill_btn->setText(QCoreApplication::translate("MainWindow", "Flood Fill 4 Connected", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Color Filling", nullptr));
        boundary_fill_8_connected_btn->setText(QCoreApplication::translate("MainWindow", "Boundary Fill 8 Connected", nullptr));
        flood_fill_btn_8_connected->setText(QCoreApplication::translate("MainWindow", "Flood Fill 8 Connected", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
