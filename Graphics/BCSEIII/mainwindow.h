#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtCore>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    void Mouse_Pressed();
    void showMousePosition(QPoint& pos);
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_show_axes_clicked();

    void on_Draw_clicked();

    void on_set_point1_clicked();

    void on_set_point2_clicked();

    void on_pushButton_clicked();

    void on_show_grid_clicked();

    void updateSystemOnGridSizeUpdate();

    void on_grid_size_valueChanged(int arg1);

    void on_set_mid_point_circle_clicked();

    void delay(int);

    void on_set_another_point_circle_clicked();

    void on_set_center_point_ellipse_clicked();

    void on_boundary_fill_btn_clicked();
    QColor get_color_from_pixel(QPoint p);

    void on_boundary_fill_8_connected_btn_clicked();

    void on_flood_fill_btn_clicked();

    void on_flood_fill_btn_8_connected_clicked();

    void on_select_old_color_clicked();


    void on_fill_color_combo_currentTextChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    QPoint p1,p2, cp1, cp2, centerPointAxis, cpe1;
    QRgb fillColor = qRgb(160, 130, 140);
    QRgb oldColor = qRgb(0, 0, 0);
    int noOfPolygonPoints;
    bool isPickColor=false;
    void point(int,int,int,int,int, int);
    void point(int,int,QRgb);
    QPoint lastP;
//    void swap(int )
    int roundNo(float n);
    int nearestPoint(int actualPoint, int gridSize);
    void on_boundary_fill_btn_clicked_recur(QPoint p);
    void on_boundary_fill_8_connected_btn_clicked_recur(QPoint p);
    void on_flood_fill_util(QPoint p);
    void on_flood_fill_8_connected_util(QPoint p);
};

#endif // MAINWINDOW_H
