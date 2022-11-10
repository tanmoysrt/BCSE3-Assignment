#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QImage>
#include <iostream>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintDevice>
#include <QPoint>
#define maxHt 1800
#define maxWd 1000
#define maxVer 10000
#define fwidth 700
#define fheight 700

// Defining region codes
const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000



QImage img=QImage(700,700,QImage::Format_RGB888);

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
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

    ui->fill_color_combo->addItem("red");
    ui->fill_color_combo->addItem("blue");
    ui->fill_color_combo->addItem("white");
    ui->fill_color_combo->addItem("green");
    ui->fill_color_combo->addItem("orange");

    ui->reflect_origin->hide();

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
// Get x-corrdinate of cell given pixel
int MainWindow::getXCoordinate(int px) {
    int width = img.width();
    int gridsize = ui->grid_size->value();
    int midx = (width/(2 * gridsize)) * gridsize;

    if (px >= midx && px <= midx + gridsize)
        return 0;

    if (px >= width/2)
        return (px - midx - gridsize)/gridsize + 1;

    if (abs(midx - px)%gridsize == 0)
        return -((midx - px)/gridsize);

    return -((midx - px)/gridsize + 1);
}
// Get y-coordinate of cell given pixel
int MainWindow::getYCoordinate(int py) {
    int height = img.height();
    int gridsize = ui->grid_size->value();
    int midy = (height/(2 * gridsize)) * gridsize;

    if(py >= midy && py <= midy + gridsize)
        return 0;

    if(py >= height/2)
        return -((py - midy - gridsize)/gridsize + 1);

    if (abs(midy - py)%gridsize == 0)
        return (midy - py)/gridsize;

    return (midy - py)/gridsize + 1;
}
// Given x-coordinate, get pixel
int  MainWindow::toXPixel(int x) {
    int width = img.width();
    int gridsize = ui->grid_size->value();
    int midx = (width/(2 * gridsize)) * gridsize;
    return midx + x * gridsize;

//    return x*gridsize + gridsize/2;
}
// Given y-coordinate, get pixel
int  MainWindow::toYPixel(int y) {
    int height = img.height();
    int gridsize = ui->grid_size->value();
    int midy = (height/(2 * gridsize)) * gridsize;
    return midy - y * gridsize;
//    return y*gridsize + gridsize/2;
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
void MainWindow::point(int x, int y, QRgb rgb){
    if(x < 0 || y < 0 || x>700 || y>700) return;
    // On click light up the pixel
    int j = ui->grid_size->value();
    if(j == 1){
        img.setPixel(x,y,rgb);
        return;
    }
    int xnearest = (int)(x/j)*j;
    int ynearest = (int)(y/j)*j;

    // Fill with color
    for(int i=xnearest+1; i<fmin(xnearest+j, img.height()); i++){
        for(int z=ynearest+1; z<fmin(ynearest+j, img.height()); z++){
            img.setPixel(i,z,rgb);
        }
    }
    ui->frame->setPixmap(QPixmap::fromImage(img));
}
QColor MainWindow::get_color_from_pixel(QPoint p){
    return img.pixelColor(*(new QPoint(p.x()+1, p.y()+1)));
}
void MainWindow::showMousePosition(QPoint &pos)
{
    int gridSize = ui->grid_size->value();
    // TODO
    ui->mouse_movement->setText(" X : "+QString::number(pos.x()/gridSize)+", Y : "+QString::number(pos.y()/gridSize));
}
void MainWindow::Mouse_Pressed()
{
    if(isPickColor){
        oldColor = img.pixel(QPoint(ui->frame->x, ui->frame->y));
//        oldColor.
        ui->old_color->setPalette(QPalette(QColor(oldColor)));
        isPickColor = false;
        return;
    }
    int j = ui->grid_size->value();

    ui->mouse_pressed->setText(" X : "+QString::number(roundNo(((ui->frame->x-centerPointAxis.x()))/j))+", Y : "+QString::number(roundNo((centerPointAxis.y()-ui->frame->y)/j)));
    point(ui->frame->x,ui->frame->y, 69,205,52);

    lastP = *(new QPoint(int(ui->frame->x/j)*j, int(ui->frame->y)));

    if(ui->move_axis_checkbox->isChecked()){
        int xnearest = int((ui->frame->x)/j)*j;
        int ynearest = int((ui->frame->y)/j)*j;

        ui->x_axis->move(0,ynearest+1);
        ui->y_axis->move(xnearest+1,0);

        centerPointAxis.setX(xnearest+1);
        centerPointAxis.setY(ynearest+1);
    }

    if(addVertexToList){
        vertices.append(lastP);
        if(vertices.length() >= 2){
            int i = vertices.length();
            storeEdgeInTable(vertices[i-2].x(), vertices[i-2].y(), vertices[i-1].x(), vertices[i-1].y());
        }
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
void MainWindow::on_Draw_clicked(){
    on_Draw_clicked(143,211,0);
}
void MainWindow::on_Draw_clicked(int red,int green,int blue)
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
                point(roundNo(x), roundNo(y), red, green, blue);
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
                if(check) point(x,y, red, green, blue);
                else point(y,x, red, green, blue);
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
            while(y >= x) {
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

        xc = x0;
        yc = y0;


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

            point(xc + xk, yc + yk, 200, 14, 9, 20);
            point(xc + xk, yc - yk, 200, 14, 9, 20);
            point(xc - xk, yc + yk, 200, 14, 9, 20);
            point(xc - xk, yc - yk, 200, 14, 9, 20);

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
                point(xc + xk, yc + yk, 200, 14, 9, 20);
                point(xc + xk, yc - yk, 200, 14, 9, 20);
                point(xc - xk, yc + yk, 200, 14, 9, 20);
                point(xc - xk, yc - yk, 200, 14, 9, 20);
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
                point(xc + xk, yc + yk, 200, 14, 9, 20);
                point(xc + xk, yc - yk, 200, 14, 9, 20);
                point(xc - xk, yc + yk, 200, 14, 9, 20);
                point(xc - xk, yc - yk, 200, 14, 9, 20);
            }

        }else if(ui->polarRadioCircle->isChecked()){
            for (int theta_degrees = 0; theta_degrees <= 90; theta_degrees++) {
                double theta_radians = M_PI * theta_degrees / 180;
                int xk = roundNo(a * cos (theta_radians));
                int yk = roundNo(b * sin (theta_radians));
                point(xc + xk, yc + yk, 253, 143, 10, 20);
                point(xc + xk, yc - yk, 253, 143, 10, 20);
                point(xc - xk, yc + yk, 253, 143, 10, 20);
                point(xc - xk, yc - yk, 253, 143, 10, 20);
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
void MainWindow::on_boundary_fill_btn_clicked(){
    qDebug() << lastP.x() << " " << lastP.y() << "\n";
    on_boundary_fill_btn_clicked_recur(lastP);
}
void MainWindow::on_boundary_fill_btn_clicked_recur(QPoint p){
    if(p.x() < 0 || p.y() < 0 || p.x() >= img.width() || p.y() >= img.height()) return;
    QRgb bordercolor = qRgb(143, 211, 0);
    QRgb fillcolor = qRgb(160, 130, 140);
    int gridSize = ui->grid_size->value();
    if(img.pixel(p) == bordercolor || img.pixel(p) == fillcolor) return;


    point(p.x(), p.y(), fillcolor);
    delay(10);
    on_boundary_fill_btn_clicked_recur(*(new QPoint(p.x()-gridSize, p.y())));
    on_boundary_fill_btn_clicked_recur(*(new QPoint(p.x()+gridSize, p.y())));
    on_boundary_fill_btn_clicked_recur(*(new QPoint(p.x(), p.y()-gridSize)));
    on_boundary_fill_btn_clicked_recur(*(new QPoint(p.x(), p.y()+gridSize)));
}
void MainWindow::on_boundary_fill_8_connected_btn_clicked()
{
    qDebug() << lastP.x() << " " << lastP.y() << "\n";
    on_boundary_fill_8_connected_btn_clicked_recur(lastP);
}
void MainWindow::on_boundary_fill_8_connected_btn_clicked_recur(QPoint p){
    if(p.x() < 0 || p.y() < 0 || p.x() >= img.width() || p.y() >= img.height()) return;
    QRgb bordercolor = qRgb(143, 211, 0);
    QRgb fillcolor = qRgb(160, 130, 140);
    int gridSize = ui->grid_size->value();
    if(img.pixel(p) == bordercolor || img.pixel(p) == fillcolor) return;


    point(p.x(), p.y(), fillcolor);
    delay(10);
    on_boundary_fill_8_connected_btn_clicked_recur(*(new QPoint(p.x()-gridSize, p.y())));
    on_boundary_fill_8_connected_btn_clicked_recur(*(new QPoint(p.x()+gridSize, p.y())));
    on_boundary_fill_8_connected_btn_clicked_recur(*(new QPoint(p.x(), p.y()-gridSize)));
    on_boundary_fill_8_connected_btn_clicked_recur(*(new QPoint(p.x(), p.y()+gridSize)));

    on_boundary_fill_8_connected_btn_clicked_recur(*(new QPoint(p.x()-gridSize, p.y()-gridSize)));
    on_boundary_fill_8_connected_btn_clicked_recur(*(new QPoint(p.x()-gridSize, p.y()+gridSize)));
    on_boundary_fill_8_connected_btn_clicked_recur(*(new QPoint(p.x()+gridSize, p.y()-gridSize)));
    on_boundary_fill_8_connected_btn_clicked_recur(*(new QPoint(p.x()+gridSize, p.y()+gridSize)));
}
void MainWindow::on_flood_fill_util(QPoint p){
    if(p.x() < 0 || p.y() < 0 || p.x() >= img.width() || p.y() >= img.height()) return;
    int gridSize = ui->grid_size->value();
    if(img.pixel(p) == oldColor ||  img.pixel(p) == qRgb(0,0,0)){
        point(p.x(), p.y(), fillColor);
        delay(10);

        on_flood_fill_util(*(new QPoint(p.x()-gridSize, p.y())));
        on_flood_fill_util(*(new QPoint(p.x()+gridSize, p.y())));
        on_flood_fill_util(*(new QPoint(p.x(), p.y()-gridSize)));
        on_flood_fill_util(*(new QPoint(p.x(), p.y()+gridSize)));
    };
}
void MainWindow::on_flood_fill_8_connected_util(QPoint p){
    if(p.x() < 0 || p.y() < 0 || p.x() >= img.width() || p.y() >= img.height()) return;
    int gridSize = ui->grid_size->value();
    if(img.pixel(p) == oldColor || img.pixel(p) == qRgb(0,0,0)){
        point(p.x(), p.y(), fillColor);
        delay(10);

        on_flood_fill_8_connected_util(*(new QPoint(p.x()-gridSize, p.y())));
        on_flood_fill_8_connected_util(*(new QPoint(p.x()+gridSize, p.y())));
        on_flood_fill_8_connected_util(*(new QPoint(p.x(), p.y()-gridSize)));
        on_flood_fill_8_connected_util(*(new QPoint(p.x(), p.y()+gridSize)));

        on_flood_fill_8_connected_util(*(new QPoint(p.x()-gridSize, p.y()-gridSize)));
        on_flood_fill_8_connected_util(*(new QPoint(p.x()-gridSize, p.y()+gridSize)));
        on_flood_fill_8_connected_util(*(new QPoint(p.x()+gridSize, p.y()-gridSize)));
        on_flood_fill_8_connected_util(*(new QPoint(p.x()+gridSize, p.y()+gridSize)));

    };
}
void MainWindow::on_flood_fill_btn_clicked()
{
    point(lastP.x(), lastP.y(), oldColor);
    on_flood_fill_util(lastP);
}
void MainWindow::on_flood_fill_btn_8_connected_clicked()
{
    point(lastP.x(), lastP.y(), oldColor);
    on_flood_fill_8_connected_util(lastP);
}
void MainWindow::on_select_old_color_clicked()
{
    isPickColor = true;
}
void MainWindow::on_fill_color_combo_currentTextChanged(const QString &arg1)
{
    if(arg1 == "red"){
        fillColor = qRgb(136, 8, 8);
    }
    if(arg1 == "blue"){
        fillColor = qRgb(137, 207, 240);
    }
    if(arg1 == "white"){
        fillColor = qRgb(255, 255, 255);
    }
    if(arg1 == "green"){
        fillColor = qRgb(127, 255, 0);
    }
    if(arg1 == "orange"){
        fillColor = qRgb(255, 191, 0);
    }
    ui->fill_color->setPalette(QPalette(QColor(fillColor)));
}

// Polygon drawing
void MainWindow::on_toggle_set_vertex_scanline_clicked(){
    if(!addVertexToList) vertices.clear();
    addVertexToList = !addVertexToList;
    if(addVertexToList){
        ui->toggle_set_vertex_scanline->setText("Done Selection");
    }else{
        ui->toggle_set_vertex_scanline->setText("Select Vertex");
    }
}
void MainWindow::drawPolygon(QList<QPoint> points, int red=143,int green=211,int blue=0){
    //Draw lines
    if(points.length() < 2 ) return;
    ui->draw_line->setChecked(true);
    ui->bresenhamRadio->setChecked(true);
    ui->ddaRadio->setChecked(false);
    for (int i = 0; i < points.length()-1; ++i) {
       p1 = points[i];
       p2 = points[i+1];
       on_Draw_clicked(red, green, blue);
    }
    // connect first and last point
    p1 = points[0];
    p2 = points[points.length()-1];
    on_Draw_clicked(red, green, blue);
    ui->draw_line->setChecked(false);
    ui->bresenhamRadio->setChecked(false);
}
void MainWindow::on_draw_polygon_clicked(){
    drawPolygon(vertices, 0, 150, 150);
}
void mat_mult(double a[3][3], double b[3], double res[3]){
    int i,j;
    for(i=0;i<3;i++){
        res[i]=0;
        for(j=0;j<3;j++)
            res[i]+=(a[i][j]*b[j]);
    }
}
void MainWindow::on_translate_btn_clicked()
{
    double tx = ui->translation_x->value();
    double ty = ui->translation_y->value();
    int k = ui->grid_size->value();

    QList<QPoint> old_vertices;
    // Deep copy
    for (QPoint vertex : vertices) {
       old_vertices.append(vertex);
    }

    tx = tx*k;
    ty = ty*k;
    ty = -ty;

    double mat[3][3]={
            {1,0,tx},
            {0,1,ty},
            {0,0,1}
        };

    double coord[3];
    double res[3];
    for(int i=0;i<vertices.length();i++){
        coord[0] = vertices[i].x();
        coord[1] = vertices[i].y();
        coord[2]=1;
        mat_mult(mat,coord,res);
        vertices[i].setX(res[0]/res[2]);
        vertices[i].setY(res[1]/res[2]);
    }

    drawPolygon(vertices,  100,100,100);
}
void MainWindow::on_scale_btn_clicked()
{
    double sx = ui->scale_x->value();
    double sy = ui->scale_y->value();
    int gridsize = ui->grid_size->value();

    QList<QPoint> old_vertices;
    // Deep copy
    for (QPoint vertex : vertices) {
       old_vertices.append(vertex);
    }

    // Pivot point
    int pivotX = (lastP.x()/gridsize)*gridsize+gridsize/2;
    int pivotY = (lastP.y()/gridsize)*gridsize+gridsize/2;

    double mat[3][3]={
            {sx,0,0},
            {0,sy,0},
            {0,0,1}
        };
    double coord[3];
    double res[3];
    for(int i=0;i<vertices.length();i++){
        coord[0] = vertices[i].x() - pivotX;
        coord[1] = pivotY-vertices[i].y();
        coord[2]=1;
        mat_mult(mat,coord,res);
        vertices[i].setX(res[0]/res[2]+pivotX);
        vertices[i].setY(pivotY-res[1]/res[2]);
    }
    drawPolygon(vertices,  50,150,90);
}
void MainWindow::on_rotate_btn_clicked()
{
    double angle = ui->rotation_angle->value();
    int gridsize = ui->grid_size->value();

    QList<QPoint> old_vertices;
    // Deep copy
    for (QPoint vertex : vertices) {
       old_vertices.append(vertex);
    }

    // Pivot point
    int pivotX = (lastP.x()/gridsize)*gridsize+gridsize/2;
    int pivotY = (lastP.y()/gridsize)*gridsize+gridsize/2;

    double dang=(double)angle*M_PI/180.0;
    double sinang=sin(dang);
    double cosang=cos(dang);

    double mat[3][3]={
            {cosang,-sinang,0},
            {sinang,cosang,0},
            {0,0,1}
        };
    double coord[3];
    double res[3];
    for(int i=0;i<vertices.length();i++){
        coord[0] = vertices[i].x() - pivotX;
        coord[1] = pivotY - vertices[i].y();
        coord[2]=1;
        mat_mult(mat,coord,res);
        vertices[i].setX(res[0]/res[2]+pivotX);
        vertices[i].setY( pivotY - res[1]/res[2]);
    }
    drawPolygon(vertices,  50,150,90);
}
void MainWindow::on_shear_btn_clicked()
{
    double shx = ui->shx->value();
    double shy = ui->shy->value();
    int gridsize = ui->grid_size->value();

    QList<QPoint> old_vertices;
    // Deep copy
    for (QPoint vertex : vertices) {
       old_vertices.append(vertex);
    }

    // Pivot point
    int pivotX = (lastP.x()/gridsize)*gridsize+gridsize/2;
    int pivotY = (lastP.y()/gridsize)*gridsize+gridsize/2;

    double mat[3][3]={
            {1,shx,0},
            {shy,1,0},
            {0,0,1}
        };
    double coord[3];
    double res[3];
    for(int i=0;i<vertices.length();i++){
        coord[0] = vertices[i].x() - pivotX;
        coord[1] = pivotY - vertices[i].y();
        coord[2]=1;
        mat_mult(mat,coord,res);
        vertices[i].setX(res[0]/res[2]+pivotX);
        vertices[i].setY(pivotY-res[1]/res[2]);
    }
    drawPolygon(vertices,  50,150,90);
}
void matmul(double trmatrix[3][3], int pt[3]) {
    int result[3];
    for (int i = 0; i < 3; i++) {
        result[i] = 0;
        for (int j = 0; j < 3; j++)
            result[i] += trmatrix[i][j] * pt[j];
    }

    for (int i = 0; i < 3; i++)
        pt[i] = result[i];
}
void reflectXaxis (int pt[3]) {
    double refxmatrix[3][3] = {{1.0, 0.0, 0.0}, {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}};
    matmul(refxmatrix, pt);
}
void reflectYaxis (int pt[3]) {
    double refymatrix[3][3] = {{-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
    matmul(refymatrix, pt);
}
void MainWindow::on_reflect_btn_clicked(){
    int l1x = p1.x(), l1y = p1.y(), l2x = p2.x(), l2y = p2.y();
    int s = vertices.size();

    if(ui->reflect_x_axis->isChecked()){
        for (int i = 0; i < s; i++) {
            int px = getXCoordinate(vertices[i].x()), py = getYCoordinate(vertices[i].y());
            int pt[3] = {px, py, 1};
            reflectXaxis (pt);
            px = pt[0];
            py = pt[1];
            vertices[i].setX(toXPixel(px));
            vertices[i].setY(toYPixel(py));
        }
    }

    if(ui->reflect_y_axis->isChecked()){
        for (int i = 0; i < s; i++) {
            int px = getXCoordinate(vertices[i].x()), py = getYCoordinate(vertices[i].y());
            int pt[3] = {px, py, 1};
            reflectYaxis (pt);
            px = pt[0];
            py = pt[1];
            vertices[i].setX(toXPixel(px));
            vertices[i].setY(toYPixel(py));
        }
    }

    if(ui->reflect_x_axis->isChecked() || ui->reflect_y_axis->isChecked()){
        drawPolygon(vertices);
        p1.setX(l1x);
        p1.setY(l1y);
        p2.setX(l2x);
        p2.setY(l2y);
    }

    if(ui->reflect_line->isChecked()){


        int i,len=vertices.size();

        double dx=p1.x()-p2.x();
        double dy=p1.y()-p2.y();

        double a=-dy;
        double b=dx;
        double c=p1.x()*dy-p1.y()*dx;

        for(i=0;i<len;i++)
        {
            int x1=vertices[i].x();
            int y1=vertices[i].y();

            vertices[i].setX((int)((double)x1-(double)(2*a*(a*x1+b*y1+c))/(double)((a*a+b*b))));
            vertices[i].setY((int)((double)y1-(double)(2*b*(a*x1+b*y1+c))/(double)((a*a+b*b))));
        }
        drawPolygon(vertices,255,255,255);
    }
}
// Scanline
// *************************** Scanline Filling ****************************
struct EdgeBucket
{
    int ymax;               // max y-coordinate of edge
    float xofymin;          // x-coordinate of endpoint with lesser y-value
    float slopeinverse;     // 1 / m

    EdgeBucket (int ym = 0, float xm = 0, float si = 0) {
        ymax = ym;
        xofymin = xm;
        slopeinverse = si;
    }
};

struct EdgeTableTuple
{
    int countEdgeBucket;    // no. of edgebuckets
    EdgeBucket buckets[maxVer];
};

EdgeTableTuple EdgeTable[10000], ActiveEdgeTuple;
int pointx1,pointy1,pointx2,pointy2,flag,pointx3,pointy3,pointx4,pointy4;

void MainWindow::initEdgeTable()
{
    flag = 0;
    int i;
    for (i=0; i<maxHt; i++)
    {
        EdgeTable[i].countEdgeBucket = 0;
    }

    ActiveEdgeTuple.countEdgeBucket = 0;
}
void insertionSort(EdgeTableTuple *ett)
{
    int i,j;
    EdgeBucket temp;

    for (i = 1; i < ett->countEdgeBucket; i++)
    {
        temp.ymax = ett->buckets[i].ymax;
        temp.xofymin = ett->buckets[i].xofymin;
        temp.slopeinverse = ett->buckets[i].slopeinverse;
        j = i - 1;

    while ((temp.xofymin < ett->buckets[j].xofymin) && (j >= 0))
    {
        ett->buckets[j + 1].ymax = ett->buckets[j].ymax;
        ett->buckets[j + 1].xofymin = ett->buckets[j].xofymin;
        ett->buckets[j + 1].slopeinverse = ett->buckets[j].slopeinverse;
        j = j - 1;
    }
        ett->buckets[j + 1].ymax = temp.ymax;
        ett->buckets[j + 1].xofymin = temp.xofymin;
        ett->buckets[j + 1].slopeinverse = temp.slopeinverse;
    }
}
void storeEdgeInTuple (EdgeTableTuple *receiver,int ym,int xm,float slopInv)
{
    (receiver->buckets[(receiver)->countEdgeBucket]).ymax = ym;
    (receiver->buckets[(receiver)->countEdgeBucket]).xofymin = (float)xm;
    (receiver->buckets[(receiver)->countEdgeBucket]).slopeinverse = slopInv;

    insertionSort(receiver);
    (receiver->countEdgeBucket)++;
}
void removeEdgeByYmax(EdgeTableTuple *Tup,int yy)
{
    int i,j;
    for (i=0; i< Tup->countEdgeBucket; i++)
    {
        if (Tup->buckets[i].ymax == yy)
        {
            for ( j = i ; j < Tup->countEdgeBucket -1 ; j++ )
                {
                Tup->buckets[j].ymax =Tup->buckets[j+1].ymax;
                Tup->buckets[j].xofymin =Tup->buckets[j+1].xofymin;
                Tup->buckets[j].slopeinverse = Tup->buckets[j+1].slopeinverse;
                }
                Tup->countEdgeBucket--;
            i--;
        }
    }
}
void updatexbyslopeinv(EdgeTableTuple *Tup)
{
    int i;

    for (i=0; i<Tup->countEdgeBucket; i++)
    {
        (Tup->buckets[i]).xofymin =(Tup->buckets[i]).xofymin + (Tup->buckets[i]).slopeinverse;
    }
}
void MainWindow::storeEdgeInTable (int x1,int y1, int x2, int y2)
{
    float m,minv;
    int ymaxTS,xwithyminTS, scanline;

    if (x2==x1) {
        minv = 0.0;
    }
    else {
        m = ((float)(y2-y1))/((float)(x2-x1));

        if (y2==y1)         // Do not store horizontal edges
            return;

        minv = 1.0 / m;
    }

    if (y1 > y2) {
        scanline = y2;
        ymaxTS = y1;
        xwithyminTS = x2;
    }
    else {
        scanline = y1;
        ymaxTS = y2;
        xwithyminTS = x1;
    }
    storeEdgeInTuple(&EdgeTable[scanline],ymaxTS,xwithyminTS,minv);
}
void MainWindow::on_scanline_algo_clicked()
{

    ui->draw_line->setChecked(true);
    ui->bresenhamRadio->setChecked(true);
        int i, j, x1, ymax1, x2, ymax2, FillFlag = 0, coordCount;

        for (i=0; i<maxHt; i++)
        {
            for (j=0; j<EdgeTable[i].countEdgeBucket; j++)
            {
                storeEdgeInTuple(&ActiveEdgeTuple,EdgeTable[i].buckets[j].
                         ymax,EdgeTable[i].buckets[j].xofymin,
                        EdgeTable[i].buckets[j].slopeinverse);
            }

            removeEdgeByYmax(&ActiveEdgeTuple, i);

            insertionSort(&ActiveEdgeTuple);

            j = 0;
            FillFlag = 0;
            coordCount = 0;
            x1 = 0;
            x2 = 0;
            ymax1 = 0;
            ymax2 = 0;
            while (j<ActiveEdgeTuple.countEdgeBucket)
            {
                if (coordCount%2==0)
                {
                    x1 = (int)(ActiveEdgeTuple.buckets[j].xofymin);
                    ymax1 = ActiveEdgeTuple.buckets[j].ymax;
                    if (x1==x2)
                    {
                        if (((x1==ymax1)&&(x2!=ymax2))||((x1!=ymax1)&&(x2==ymax2)))
                        {
                            x2 = x1;
                            ymax2 = ymax1;
                        }

                        else
                        {
                            coordCount++;
                        }
                    }

                    else
                    {
                            coordCount++;
                    }
                }
                else
                {
                    x2 = (int)ActiveEdgeTuple.buckets[j].xofymin;
                    ymax2 = ActiveEdgeTuple.buckets[j].ymax;

                    FillFlag = 0;
                    if (x1==x2)
                    {
                        if (((x1==ymax1)&&(x2!=ymax2))||((x1!=ymax1)&&(x2==ymax2)))
                        {
                            x1 = x2;
                            ymax1 = ymax2;
                        }
                        else
                        {
                            coordCount++;
                            FillFlag = 1;
                        }
                    }
                    else
                    {
                        coordCount++;
                        FillFlag = 1;
                    }

                if(FillFlag)
                {

                        p1.setX(x1);p1.setY(i);
                        p2.setX(x2);p2.setY(i);
                        on_Draw_clicked(150,100,79);
                        point(x1,i);
                        point(x2,i);
                        if(flag==0){
                            pointx3=x1;
                            pointy3=i;
                            pointx4=x2;
                            pointy4=i;

                        }
                        pointx1=x1;
                        pointy1=i;
                        pointx2=x2;
                        pointy2=i;
                        delay(1);
                }
                flag=1;
            }

            j++;
        }
        updatexbyslopeinv(&ActiveEdgeTuple);
    }

        vertices.clear();
        initEdgeTable();
        p1.setX(pointx1);p1.setY(pointy1);
        p2.setX(pointx2);p2.setY(pointy2);
        on_Draw_clicked();
        p1.setX(pointx3);p1.setY(pointy3);
        p2.setX(pointx4);p2.setY(pointy4);
        on_Draw_clicked();

    ui->draw_line->setChecked(false);
    ui->bresenhamRadio->setChecked(false);
}
void MainWindow::on_scanline_algo_2_clicked()
{
    MainWindow::initEdgeTable();
}
// ************************* Line clipping *************************
void MainWindow::on_set_corner_1_clicked()
{
    int gridsize = ui->grid_size->value();
    cp1.setX((ui->frame->x/gridsize)*gridsize+gridsize/2);
    cp1.setY((ui->frame->y/gridsize)*gridsize+gridsize/2);
}
void MainWindow::on_set_corner_2_clicked()
{
    int gridsize = ui->grid_size->value();
    cp2.setX((ui->frame->x/gridsize)*gridsize+gridsize/2);
    cp2.setY((ui->frame->y/gridsize)*gridsize+gridsize/2);

    clipper_points[0][0]=cp1.x();
    clipper_points[0][1]=cp1.y();
    clipper_points[1][0]=cp1.x();
    clipper_points[1][1]=cp2.y();
    clipper_points[2][0]=cp2.x();
    clipper_points[2][1]=cp2.y();
    clipper_points[3][0]=cp2.x();
    clipper_points[3][1]=cp1.y();
}
void MainWindow::drawWindow(){
    ui->draw_line->setChecked(true);
    ui->bresenhamRadio->setChecked(true);

    p1.setX(clipper_points[0][0]);
    p1.setY(clipper_points[0][1]);
    p2.setX(clipper_points[1][0]);
    p2.setY(clipper_points[1][1]);
    on_Draw_clicked(30, 100, 200);

    p1.setX(clipper_points[1][0]);
    p1.setY(clipper_points[1][1]);
    p2.setX(clipper_points[2][0]);
    p2.setY(clipper_points[2][1]);
    on_Draw_clicked(30, 100, 200);

    p1.setX(clipper_points[2][0]);
    p1.setY(clipper_points[2][1]);
    p2.setX(clipper_points[3][0]);
    p2.setY(clipper_points[3][1]);
    on_Draw_clicked(30, 100, 200);

    p1.setX(clipper_points[3][0]);
    p1.setY(clipper_points[3][1]);
    p2.setX(clipper_points[0][0]);
    p2.setY(clipper_points[0][1]);
    on_Draw_clicked(30, 100, 200);

    ui->draw_line->setChecked(false);
    ui->bresenhamRadio->setChecked(false);
}
// Function to compute region code for a point(x, y)
int MainWindow::computeCode(int xa, int ya){

    int x_min=clipper_points[0][0],x_max=clipper_points[2][0],y_min=clipper_points[0][1],y_max=clipper_points[2][1];

    // initialized as being inside
    int code = INSIDE;
    if (xa < x_min)       // to the left of rectangle
        code |= LEFT;
    else if (xa > x_max)  // to the right of rectangle
        code |= RIGHT;
    if (ya < y_min)       // below the rectangle
        code |= BOTTOM;
    else if (ya > y_max)  // above the rectangle
        code |= TOP;

    return code;
}
void MainWindow::cohenSutherlandClip(int x1, int y1,int x2, int y2){
    int gridsize = ui->grid_size->value();
    int x_min=clipper_points[0][0],x_max=clipper_points[2][0],y_min=clipper_points[0][1],y_max=clipper_points[2][1];
    // Compute region codes for P1, P2
    int code1 = computeCode(x1, y1);
    int code2 = computeCode(x2, y2);

    // Initialize line as outside the rectangular window
    bool accept = false;

    ui->draw_line->setChecked(true);
    ui->bresenhamRadio->setChecked(true);


    while (true)
    {

        if ((code1 == 0) && (code2 == 0))
        {
            accept = true;
            break;
        }
        else if (code1 & code2)
        {
            // If both endpoints are outside rectangle,
            // in same region
            break;
        }
        else
        {
            // Some segment of line lies within the
            // rectangle
            int code_out;
            int x, y;

            // At least one endpoint is outside the
            // rectangle, pick it.
            if (code1 != 0)
                code_out = code1;
            else
                code_out = code2;
            // Find intersection point;
            // using formulas y = y1 + slope * (x - x1),
            // x = x1 + (1 / slope) * (y - y1)
            if (code_out & TOP)
            {
                // point is above the clip rectangle
                x = x1 + (int)((double)(x2 - x1) * (double)(y_max - y1) / (double)(y2 - y1));
                y = y_max;
            }
            else if (code_out & BOTTOM)
            {
                // point is below the rectangle
                x = x1 + (int)((double)(x2 - x1) * (double)(y_min - y1) / (double)(y2 - y1));
                y = y_min;
            }
            else if (code_out & RIGHT)
            {
                // point is to the right of rectangle
                y = y1 + (int)((double)(y2 - y1) * (double)(x_max - x1) / (double)(x2 - x1));
                x = x_max;
            }
            else if (code_out & LEFT)
            {
                // point is to the left of rectangle
                y = y1 + (int)((double)(y2 - y1) * (double)(x_min - x1) / (double)(x2 - x1));
                x = x_min;
            }

            // Now intersection point x,y is found
            // We replace point outside rectangle
            // by intersection point
            if (code_out == code1)
            {
                x1 = x;
                y1 = y;
                code1 = computeCode(x1, y1);
            }
            else
            {
                x2 = x;
                y2 = y;
                code2 = computeCode(x2, y2);
            }
        }
    }
    if (accept)
    {
        //If accepted
        //Just reset and draw the boundary and the line
        //Reset the screen and draw the grid

        p1.setX(x1);
        p1.setY(y1);

        p2.setX(x2);
        p2.setY(y2);

        on_Draw_clicked(100,200,150);
        drawWindow();
    }
    else
    {
        //If not accepted
        //Just reset and draw the boundary
        //Reset the screen and draw the grid
        drawWindow();
    }

    ui->draw_line->setChecked(false);
    ui->bresenhamRadio->setChecked(false);
}
void MainWindow::on_csa_clicked()
{
    for (QPair<QPoint, QPoint> line : lines) {
        p1.setX(line.first.x());
        p1.setY(line.first.y());
        p2.setX(line.second.x());
        p2.setY(line.second.y());
        ui->draw_line->setChecked(true);
        ui->bresenhamRadio->setChecked(true);
        on_Draw_clicked(0,0,0);
        ui->draw_line->setChecked(false);
        ui->bresenhamRadio->setChecked(false);
        cohenSutherlandClip(p1.x(), p1.y(), p2.x(), p2.y());
    }

}
void MainWindow::on_saveline_clicked()
{
    lines.append(*(new QPair<QPoint, QPoint>(p1, p2)));
}

// *********************** POLYGON CLIPPING *************************************
const int MAX_POINTS = 20;
// Returns x-value of point of intersectipn of two
// lines
int MainWindow::x_intersect(int x1, int y1, int x2, int y2,int x3, int y3, int x4, int y4)
{
    y1=ui->frame->height()-y1+1;
    y2=ui->frame->height()-y2+1;
    y3=ui->frame->height()-y3+1;
    y4=ui->frame->height()-y4+1;
    int num = (x1*y2 - y1*x2) * (x3-x4) -
              (x1-x2) * (x3*y4 - y3*x4);
    int den = (x1-x2) * (y3-y4) - (y1-y2) * (x3-x4);
    int retx=num/den;
    return retx;
}
// Returns y-value of point of intersectipn of
// two lines
int MainWindow:: y_intersect(int x1, int y1, int x2, int y2,int x3, int y3, int x4, int y4)
{
    y1=ui->frame->height()-y1+1;
    y2=ui->frame->height()-y2+1;
    y3=ui->frame->height()-y3+1;
    y4=ui->frame->height()-y4+1;
    int num = (x1*y2 - y1*x2) * (y3-y4) -
              (y1-y2) * (x3*y4 - y3*x4);
    int den = (x1-x2) * (y3-y4) - (y1-y2) * (x3-x4);
    int rety= (ui->frame->height()-num/den+1);
    return rety;
}
// This functions clips all the edges w.r.t one clip
// edge of clipping area
void MainWindow:: clip(int x1, int y1, int x2, int y2)
{
    int poly_size=vertices.size()-1;
    int new_poly_size = 0;

    std::vector<std::pair<int,int> > new_points;

    // (ix,iy),(kx,ky) are the co-ordinate values of
    // the points
    for (int i = 0; i < poly_size; i++)
    {
        // i and k form a line in polygon
        int k = (i+1) % poly_size;
        int ix = vertices[i].x(), iy = vertices[i].y();
        int kx = vertices[k].x(), ky = vertices[k].y();

        // Calculating position of first and second point

        int i_pos,k_pos;
        if(x2==x1 && ix>x1) i_pos=1;
        else if(x2==x1 && ix<x1) i_pos=-1;
        else if(y2==y1 && iy<y1) i_pos=1;
        else i_pos=-1;

        if(x2==x1 && kx>x1) k_pos=1;
        else if(x2==x1 && kx<x1) k_pos=-1;
        else if(y2==y1 && ky<y1) k_pos=1;
        else k_pos=-1;

        if(y1>y2||x1>x2)
        {
            i_pos=(-1)*i_pos;
            k_pos=(-1)*k_pos;
        }

        // Case 1 : When both points are inside
        if (i_pos >= 0  && k_pos >= 0)
        {
            //Only second point is added
            new_points.push_back(std::make_pair(kx,ky));
            new_poly_size++;
        }

        // Case 2: When only first point is outside
        else if (i_pos < 0  && k_pos >= 0)
        {
            // Point of intersection with edge
            // and the second point is added

            new_points.push_back(std::make_pair(x_intersect(x1,y1, x2, y2, ix, iy, kx, ky),y_intersect(x1,y1, x2, y2, ix, iy, kx, ky)));
            new_poly_size++;

            new_points.push_back(std::make_pair(kx,ky));
            new_poly_size++;
        }

        // Case 3: When only second point is outside
        else if (i_pos >= 0  && k_pos < 0)
        {
            //Only point of intersection with edge is added

            new_points.push_back(std::make_pair(x_intersect(x1,y1, x2, y2, ix, iy, kx, ky),y_intersect(x1,y1, x2, y2, ix, iy, kx, ky)));
            new_poly_size++;
        }

        // Case 4: When both points are outside
        else
        {
            //No points are added
        }
    }

    // Copying new points into original array
    // and changing the no. of vertices
    poly_size = new_poly_size;
    vertices.clear();
    for (int i = 0; i < new_points.size(); i++)
    {
        vertices.push_back(*(new QPoint(new_points[i].first, new_points[i].second)));
    }
    vertices.push_back(*(new QPoint(new_points[0].first, new_points[0].second)));
}


void MainWindow::on_sha_clicked()
{
    drawPolygon(vertices,0,0,0);
    int clipper_size=4;
    //i and k are two consecutive indexes
    for (int i=0; i<clipper_size; i++)
    {
        int k = (i+1) % clipper_size;

        // We pass the current array of vertices, it's size
        // and the end points of the selected clipper line
        clip(clipper_points[i][0],
             clipper_points[i][1], clipper_points[k][0],
             clipper_points[k][1]);
    }

    drawWindow();
    drawPolygon(vertices,255,255,255);
}
// -----------------------------------------BEZIER CURVE------------------------------------
std::vector<std::pair<int,int> > BezList;
void MainWindow::on_setbezpoint_clicked()
{
    int k=ui->grid_size->value();
    int x=((ui->frame->x)/k)*k+k/2;
    int y=((ui->frame->y)/k)*k+k/2;
    BezList.push_back(std::make_pair(x,y));
    if(BezList.size()>1)
    {
        p1.setX(BezList[BezList.size()-1].first);
        p2.setX(BezList[BezList.size()-2].first);

        p1.setY(BezList[BezList.size()-1].second);
        p2.setY(BezList[BezList.size()-2].second);

        ui->draw_line->setChecked(true);
        ui->bresenhamRadio->setChecked(true);
        on_Draw_clicked(0,255,0);
        ui->draw_line->setChecked(false);
        ui->bresenhamRadio->setChecked(false);
    }
}
void MainWindow::on_clearbezpoint_clicked()
{
    BezList.clear();
}
void MainWindow::on_drawBezierCurve_clicked()
{
    double xu = 0.0 , yu = 0.0 , u = 0.0 ;
    for(u = 0.0 ; u <= 1.0 ; u += 0.001) {
        xu = pow(1-u,3)*BezList[0].first+3*u*pow(1-u,2)*BezList[1].first+3*pow(u,2)*(1-u)*BezList[2].first+pow(u,3)*BezList[3].first;
        yu = pow(1-u,3)*BezList[0].second+3*u*pow(1-u,2)*BezList[1].second+3*pow(u,2)*(1-u)*BezList[2].second+pow(u,3)*BezList[3].second;
        point((int)xu , (int)yu,255,0,0) ;
        delay(10);
    }
}

