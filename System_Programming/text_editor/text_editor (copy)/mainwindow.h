#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QKeyEvent>
#include <QMainWindow>
#include <QStack>
#include <QTimer>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    bool isUpperCase=false;
    int characterCount = 0;
    QString text;
    QStack<int> undoStack;
    QStack<QString> redoStack;


    QTimer *timer;



    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

protected:
    // Override key press and release
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    // Operation
    void addNewCharacter(char c);
    void removeLastCharacter();

    void addToUndoStack(int pos);
    void refreshUI();



private slots:
    void on_find_and_replace_btn_clicked();
    void on_save_to_file_clicked();
    void on_open_from_file_clicked();
    void on_redo_btn_clicked();
    void on_undo_btn_clicked();
    void addPosToUndoStack();
    void on_find_btn_clicked();
};
#endif // MAINWINDOW_H
