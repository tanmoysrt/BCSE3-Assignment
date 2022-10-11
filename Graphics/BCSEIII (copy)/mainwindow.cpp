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
    ui->x_axis->setStyleSheet("background-color: rgb(208,240,191);");
    ui->y_axis->setStyleSheet("background-color: rgb(208,240,191);");

    int k=ui->grid_size->value();

    centerPointAxis.setX(0);
    centerPointAxis.setY(0);

    // Update the geometry of axis
    ui->x_axis->setGeometry(0,350,700, k);
    ui->y_axis->setGeometry(350, 0, k, 700);

    // Hide the axis
//    ui->x_axis->hide();
//    ui->y_axis->hide();

    // Mouse event
    connect(ui->frame,SIGNAL(Mouse_Pos()),this,SLOT(Mouse_Pressed()));
    connect(ui->frame,SIGNAL(sendMousePosition(QPoint&)),this,SLOT(showMousePosition(QPoint&)));
}

MainWindow::~MainWindow()
{
    delete ui;
}
int MainWindow::roundNo(float n) {
//    if (abs(n - (int)n) <= 0.5)
//        return (int)n;
//    return (int)(n + 1);
    return round(n);
}
void MainWindow::point(int x,int y,int red=143,int green=211,int blue=0, int delayms=0)
{
    if(x < 0 || y < 0) return;
    // On click light up the pixel
    int j = ui->grid_size->value();
    if(j == 1){
        img.setPixel(x,y,qRgb(red,green,blue));
        return;
    }
    int xnearest = (int)(x/j)*j;
    int ynearest = (int)(y/j)*j;

    // Fill with color
    for(int i=xnearest+1; i<fmin(xnearest+j, img.height()); i++){
        for(int z=ynearest+1; z<fmin(ynearest+j, img.height()); z++){
            img.setPixel(i,z,qRgb(red,green,blue));
        }
    }
    ui->frame->setPixmap(QPixmap::fromImage(img));
    delay(delayms);
}
void MainWindow::showMousePosition(QPoint &pos)
{
    int gridSize = ui->grid_size->value();
    // TODO
    ui->mouse_movement->setText(" X : "+QString::number(pos.x()/gridSize)+", Y : "+QString::number(pos.y()/gridSize));
}
void MainWindow::Mouse_Pressed()
{
    int j = ui->grid_size->value();

    ui->mouse_pressed->setText(" X : "+QString::number(roundNo(((ui->frame->x-centerPointAxis.x()))/j))+", Y : "+QString::number(roundNo((centerPointAxis.y()-ui->frame->y)/j)));
    point(ui->frame->x,ui->frame->y, 69,205,52);



    if(ui->move_axis_checkbox->isChecked()){
        int xnearest = int((ui->frame->x)/j)*j;
        int ynearest = int((ui->frame->y)/j)*j;

        ui->x_axis->move(0,ynearest+1);
        ui->y_axis->move(xnearest+1,0);

        centerPointAxis.setX(xnearest+1);
        centerPointAxis.setY(ynearest+1);
    }
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
    clock_t start, end;

    start = clock();

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
            double x1 = p1.x()/gridSize;
            double y1 = p1.y()/gridSize;
            double x2 = p2.x()/gridSize;
            double y2 = p2.y()/gridSize;

            // Dx , dy is simulated pixel
            int dx = fabs(x2 - x1);
            int dy = fabs(y2 - y1);

            int xinc = (x1 > x2 ? -1 : 1);
            int yinc = (y1 > y2 ? -1 : 1);
            bool check = 1;
            int x = x1*gridSize + gridSize/2;
            int y = y1*gridSize + gridSize/2;
            if(dy > dx) { // if slope > 1, then swap
                std::swap(dx,dy);
                std::swap(x,y);
                std::swap(xinc,yinc);
                check = 0;
            }
            int decision = 2*dy - dx;
            int step = dx;
            for(int i=0;i<=step;i++) {
                if(check) point(x,y);
                else point(y,x);
                if(decision < 0) {
                    x += xinc*gridSize;
                    decision += 2*dy;
                } else {
                   x += xinc*gridSize;
                   y += yinc*gridSize;
                   decision += 2*dy - 2*dx;
                }
           }
        }
    }
    if(ui->draw_circle->isChecked()){
        int xc=cp1.x(),yc=cp1.y();
        int r = ui->circle_radius->value();
        float gridsize=ui->grid_size->value();


        if(ui->bresenhamRadioCircle->isChecked()){
            int x0 = xc/gridsize;
            int y0 = yc/gridsize;
            x0 = x0*gridsize + gridsize/2;
            y0 = y0*gridsize + gridsize/2;
            int x = 0;
            int y = r*gridsize;
            int p = (3 - 2*r)*gridsize;
            while(y > x) {
                point(x0 - x, y0 - y,124,255,0, 20);
                point(x0 + x, y0 - y,124,255,0, 20);
                point(x0 - x, y0 + y,124,255,0, 20);
                point(x0 + x, y0 + y,124,255,0, 20);
                point(x0 - y, y0 - x,124,255,0, 20);
                point(x0 + y, y0 - x,124,255,0, 20);
                point(x0 - y, y0 + x,124,255,0, 20);
                point(x0 + y, y0 + x,124,255,0, 20);
                x += gridsize;
                if(p <= 0) {
                    p += 4*x + 6;
                } else {
                    p += 4*(x-y) + 10;
                    y -= gridsize;
                }
            }
        }

        if(ui->polarRadioCircle->isChecked()){
           int x0 = xc/gridsize;
           int y0 = yc/gridsize;
           x0 = x0*gridsize + gridsize/2;
           y0 = y0*gridsize + gridsize/2;
           for (int theta_degrees = 0; theta_degrees <= 45; theta_degrees++) {
               double theta_radians = M_PI * theta_degrees / 180;
               int xk = r * cos (theta_radians) * gridsize;
               int yk = r * sin (theta_radians) * gridsize;


               point(x0 - xk, y0 - yk, 253, 143, 10, 20);
               point(x0 + xk, y0 - yk, 253, 143, 10, 20);
               point(x0 - xk, y0 + yk, 253, 143, 10, 20);
               point(x0 + xk, y0 + yk, 253, 143, 10, 20);
               point(x0 - yk, y0 - xk, 253, 143, 10, 20);
               point(x0 + yk, y0 - xk, 253, 143, 10, 20);
               point(x0 - yk, y0 + xk, 253, 143, 10, 20);
               point(x0 + yk, y0 + xk, 253, 143, 10, 20);
           }
        }
    }

    if(ui->draw_ellipse->isChecked()){
        int xc = cpe1.x();
        int yc = cpe1.y();
        float gridsize=ui->grid_size->value();

        int x0 = xc/gridsize;
        int y0 = yc/gridsize;
        x0 = x0*gridsize + gridsize/2;
        y0 = y0*gridsize + gridsize/2;

        xc =x0;
        yc=y0;


        int a = ui->ellipseA->value()*gridSize;
        int b = ui->ellipseB->value()*gridSize;

        if(ui->bresenhamRadioCircle->isChecked()){
            int rx2 = a*a;
            int ry2 = b*b;
            int tworx2 = 2*rx2;
            int twory2 = 2*ry2;
            int p;
            int xk = 0;
            int yk = b;
            int px = 0;
            int py = tworx2*yk;

            point(xc + xk, yc + yk, 200, 14, 9, 0);
            point(xc + xk, yc - yk, 200, 14, 9, 0);
            point(xc - xk, yc + yk, 200, 14, 9, 0);
            point(xc - xk, yc - yk, 200, 14, 9, 0);

            // Region 1
            p = roundNo(ry2-(rx2*b)+0.25*rx2);
            while(px<py){
                xk++;
                px+=twory2;
                if(p<0){
                    p+=ry2+px;
                }else{
                    yk--;
                    py -= tworx2;
                    p += ry2+px-py;
                }
                point(xc + xk, yc + yk, 200, 14, 9, 0);
                point(xc + xk, yc - yk, 200, 14, 9, 0);
                point(xc - xk, yc + yk, 200, 14, 9, 0);
                point(xc - xk, yc - yk, 200, 14, 9, 0);
            }
            // Region 2
            p = roundNo(ry2*(xk+0.5)*(xk+0.5)+rx2*(yk-1)*(yk-1) - rx2*ry2);
            while(yk > 0){
                yk--;
                py -= tworx2;
                if(p>0){
                    p += rx2-py;
                }else{
                    xk++;
                    px += twory2;
                    p += rx2-py+px;
                }
                point(xc + xk, yc + yk, 200, 14, 9, 0);
                point(xc + xk, yc - yk, 200, 14, 9, 0);
                point(xc - xk, yc + yk, 200, 14, 9, 0);
                point(xc - xk, yc - yk, 200, 14, 9, 0);
            }

        }else if(ui->polarRadioCircle->isChecked()){
            for (int theta_degrees = 0; theta_degrees <= 90; theta_degrees++) {
                double theta_radians = M_PI * theta_degrees / 180;
                int xk = roundNo(a * cos (theta_radians));
                int yk = roundNo(b * sin (theta_radians));
                point(xc + xk, yc + yk, 253, 143, 10, 0);
                point(xc + xk, yc - yk, 253, 143, 10, 0);
                point(xc - xk, yc + yk, 253, 143, 10, 0);
                point(xc - xk, yc - yk, 253, 143, 10, 0);
            }
        }
    }

    end = clock();
    double time_taken = double(end-start)/double(CLOCKS_PER_SEC);
    QString z =  QString::number(time_taken*1000000000);
    ui->time_taken->setText(z);
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
    ui->x_axis->setGeometry(0,350,700, k);
    ui->y_axis->setGeometry(350, 0, k, 700);

    // Nearest x and y
    int xnearest = int((ui->y_axis->geometry().left())/k)*k;
    int ynearest = int((ui->x_axis->geometry().top())/k)*k;

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
            for(j=0;j<img.height();j++)img.setPixel(j,i,qRgb(119,119,119));
        }

        for(i=0;i<img.height();i+=k){
            for(j=0;j<img.width();j++) img.setPixel(i,j,qRgb(119,119,119));
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
void MainWindow::on_set_mid_point_circle_clicked()
{
    if(ui->draw_circle->isChecked()){
        cp1.setX(ui->frame->x);
        cp1.setY(ui->frame->y);
    }
}
//delay function to introduce animation while drawing
void MainWindow::delay(int n) {
    QTime dieTime= QTime::currentTime().addMSecs(n);
    while (QTime::currentTime() < dieTime)
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::on_set_another_point_circle_clicked()
{
    if(ui->draw_circle->isChecked()){
        cp2.setX(ui->frame->x);
        cp2.setY(ui->frame->y);
    }
}


void MainWindow::on_set_center_point_ellipse_clicked()
{
    cpe1.setX(ui->frame->x);
    cpe1.setY(ui->frame->y);
}

