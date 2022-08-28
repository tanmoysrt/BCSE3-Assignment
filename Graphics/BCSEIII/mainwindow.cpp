#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QImage>
#include <iostream>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintDevice>
#include <QPoint>

QImage img=QImage(700,700,QImage::Format_RGB888);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set background of axis
    ui->x_axis->setStyleSheet("background-color: rgb(222, 190, 66);");
    ui->y_axis->setStyleSheet("background-color: rgb(222, 190, 66);");

    int k=ui->grid_size->value();

    // Update the geometry of axis
    ui->x_axis->setGeometry(0,225,700, k);
    ui->y_axis->setGeometry(225, 0, k, 700);

    // Hide the axis
    ui->x_axis->hide();
    ui->y_axis->hide();

    // Mouse event
    connect(ui->frame,SIGNAL(Mouse_Pos()),this,SLOT(Mouse_Pressed()));
    connect(ui->frame,SIGNAL(sendMousePosition(QPoint&)),this,SLOT(showMousePosition(QPoint&)));
}

MainWindow::~MainWindow()
{
    delete ui;
}
int MainWindow::roundNo(float n) {
    if (n - (int)n < 0.5)
        return (int)n;
    return (int)(n + 1);
}
void MainWindow::point(int x,int y,int red=225,int green=176,int blue=2)
{
    // On click light up the pixel
    int j = ui->grid_size->value();
    if(j == 1){
        img.setPixel(x,y,qRgb(red,green,blue));
        return;
    }
    int xnearest = int((x)/j)*j;
    int ynearest = int((y)/j)*j;

    // Fill with color
    for(int i=xnearest+1; i<xnearest+j; i++){
        for(int z=ynearest+1; z<ynearest+j; z++){
            img.setPixel(i,z,qRgb(red,green,blue));
        }
    }
    ui->frame->setPixmap(QPixmap::fromImage(img));
}
void MainWindow::showMousePosition(QPoint &pos)
{
    ui->mouse_movement->setText(" X : "+QString::number(pos.x())+", Y : "+QString::number(pos.y()));
}
void MainWindow::Mouse_Pressed()
{
    ui->mouse_pressed->setText(" X : "+QString::number(ui->frame->x)+", Y : "+QString::number(ui->frame->y));
    point(ui->frame->x,ui->frame->y);
    int j = ui->grid_size->value();
    int xnearest = int((ui->frame->x)/j)*j;
    int ynearest = int((ui->frame->y)/j)*j;
    ui->x_axis->move(0,ynearest+1);
    ui->y_axis->move(xnearest+1,0);
}
void MainWindow::on_show_axes_clicked()
{
    if(ui->show_axes->isChecked())
    {
        ui->x_axis->show();
        ui->y_axis->show();
    }
    else{
        ui->x_axis->hide();
        ui->y_axis->hide();
    }
}
void MainWindow::on_set_point1_clicked()
{
    if(ui->draw_line->isChecked()){
        p1.setX(ui->frame->x);
        p1.setY(ui->frame->y);
    }
}
void MainWindow::on_set_point2_clicked()
{
    if(ui->draw_line->isChecked()){
        p2.setX(ui->frame->x);
        p2.setY(ui->frame->y);
    }
}
void MainWindow::on_Draw_clicked()
{
    int gridSize = ui->grid_size->value();
    if(ui->draw_circle->isChecked()){
    }
    if(ui->draw_line->isChecked()){
        if(ui->ddaRadio->isChecked()){
            int x1 = (p1.x()/gridSize);
            int x2 = (p2.x()/gridSize);
            int y1 = (p1.y()/gridSize);
            int y2 = (p2.y()/gridSize);
            // Dx , dy is real pizxel
            int dx = x2-x1;
            int dy = y2-y1;

            int steps=0;
            if(abs(dx) > abs(dy)){
                steps = abs(dx);
            }else{
                steps = abs(dy);
            }

            float x = x1*gridSize+gridSize/2;
            float y = y1*gridSize+gridSize/2;

            float x_incr = ((float)dx)/steps;
            float y_incr = ((float)dy)/steps;

            for(int i=1; i<=steps; i++){
                x += x_incr*gridSize;
                y += y_incr*gridSize;
                point(roundNo(x), roundNo(y));
            }

        }
        if(ui->bresenhamRadio->isChecked()){
            // Dx , dy is simulated pixel
            int dx = roundNo((p2.x()-p1.x())/gridSize);
            int dy = roundNo((p2.y()-p1.y())/gridSize);

            int x=roundNo(p1.x()/gridSize);
            int y=roundNo(p1.y()/gridSize);

            int p=2*dy-dx;

            int x2 = roundNo(p2.x()/gridSize);

            while(x<x2){
                point(x*gridSize,y*gridSize);

                if(p<0){
                    x=x+1;
                    p=p+2*dy;
                }else{
                    y=y+1;
                    p=p+2*dy-2*dx;
                }
            }
        }
    }
    ui->frame->setPixmap(QPixmap::fromImage(img));
}
void MainWindow::on_pushButton_clicked()
{
    // Reset canvas
    for(int j=0;j<img.height();j++)
    {
        for(int i=0;i<img.width();i++)
        {
            img.setPixel(i,j,qRgb(0,0,0));
        }
    }
    ui->frame->setPixmap(QPixmap::fromImage(img));

    // Reset checkbox states
    ui->show_grid->setCheckState(Qt::Unchecked);
}
void MainWindow::updateSystemOnGridSizeUpdate(){
    int i,j,k=ui->grid_size->value();

    // Update the geometry of axis
    ui->x_axis->setGeometry(0,225,700, k);
    ui->y_axis->setGeometry(225, 0, k, 700);

    // Nearest x and y
    int xnearest = int((ui->frame->x)/k)*k;
    int ynearest = int((ui->frame->y)/k)*k;

    // Update axis
    ui->x_axis->move(0,ynearest+1);
    ui->y_axis->move(xnearest+1,0);

    // Make the canvas back to black
    for(i=0;i<img.width();i++){
        for(j=0;j<img.height();j++) img.setPixel(j,i,qRgb(0,0,0));
    }

    for(i=0;i<img.height();i++){
        for(j=0;j<img.width();j++) img.setPixel(i,j,qRgb(0,0,0));
    }

    // If need to show grid, draw the specific pixels
    if(ui->show_grid->isChecked()){
        for(i=0;i<img.width();i+=k){
            for(j=0;j<img.height();j++)img.setPixel(j,i,qRgb(255,255,255));
        }

        for(i=0;i<img.height();i+=k){
            for(j=0;j<img.width();j++) img.setPixel(i,j,qRgb(255,255,255));
        }
    }

    ui->frame->setPixmap(QPixmap::fromImage(img));
}
void MainWindow::on_show_grid_clicked()
{
   updateSystemOnGridSizeUpdate();
}
void MainWindow::on_grid_size_valueChanged(int arg1)
{
    updateSystemOnGridSizeUpdate();
}

