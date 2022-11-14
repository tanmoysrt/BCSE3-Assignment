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

    void on_boundary_fill_8_connected_btn_clicked();

    void on_flood_fill_btn_clicked();

    void on_flood_fill_btn_8_connected_clicked();

    void on_select_old_color_clicked();


    void on_fill_color_combo_currentTextChanged(const QString &arg1);

    void on_toggle_set_vertex_scanline_clicked();

    void on_draw_polygon_clicked();

    void on_translate_btn_clicked();

    void on_scale_btn_clicked();

    void on_rotate_btn_clicked();

    void on_shear_btn_clicked();

    void on_reflect_btn_clicked();

    void on_scanline_algo_clicked();

    void on_scanline_algo_2_clicked();

    void on_set_corner_1_clicked();

    void on_set_corner_2_clicked();

    void on_csa_clicked();

    void on_saveline_clicked();

    void on_sha_clicked();

    void on_setbezpoint_clicked();

    void on_clearbezpoint_clicked();

    void on_drawBezierCurve_clicked();

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
    bool addVertexToList=false;
    QList<QPoint> vertices ;
    int clipper_points[4][2];
    QList<QPair<QPoint, QPoint>> lines;


//    void swap(int )
    int roundNo(float n);
    int nearestPoint(int actualPoint, int gridSize);
    QRgb get_color_from_pixel(QPoint p);
    void on_boundary_fill_btn_clicked_recur(QPoint p);
    void on_boundary_fill_8_connected_btn_clicked_recur(QPoint p);
    void on_flood_fill_util(QPoint p);
    void on_flood_fill_8_connected_util(QPoint p);
    void drawPolygon(QList<QPoint>,int,int,int);
    void on_Draw_clicked(int, int, int);
    void initEdgeTable();
    void storeEdgeInTable (int x1,int y1, int x2, int y2);
    void drawWindow();
    int getXCoordinate(int px);
    int getYCoordinate(int py);
    int toXPixel(int x);
    int toYPixel(int y);
    int computeCode(int xa, int ya);
    void cohenSutherlandClip(int x1, int y1,int x2, int y2);
    int x_intersect(int x1, int y1, int x2, int y2,int x3, int y3, int x4, int y4);
    int y_intersect(int x1, int y1, int x2, int y2,int x3, int y3, int x4, int y4);
    void clip(int x1, int y1, int x2, int y2);

};


#endif // MAINWINDOW_H
