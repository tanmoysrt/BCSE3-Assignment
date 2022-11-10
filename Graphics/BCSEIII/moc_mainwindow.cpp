/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.3.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "mainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QScreen>
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.3.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    const uint offsetsAndSize[88];
    char stringdata0[973];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 10), // "MainWindow"
QT_MOC_LITERAL(11, 13), // "Mouse_Pressed"
QT_MOC_LITERAL(25, 0), // ""
QT_MOC_LITERAL(26, 17), // "showMousePosition"
QT_MOC_LITERAL(44, 7), // "QPoint&"
QT_MOC_LITERAL(52, 3), // "pos"
QT_MOC_LITERAL(56, 20), // "on_show_axes_clicked"
QT_MOC_LITERAL(77, 15), // "on_Draw_clicked"
QT_MOC_LITERAL(93, 21), // "on_set_point1_clicked"
QT_MOC_LITERAL(115, 21), // "on_set_point2_clicked"
QT_MOC_LITERAL(137, 21), // "on_pushButton_clicked"
QT_MOC_LITERAL(159, 20), // "on_show_grid_clicked"
QT_MOC_LITERAL(180, 28), // "updateSystemOnGridSizeUpdate"
QT_MOC_LITERAL(209, 25), // "on_grid_size_valueChanged"
QT_MOC_LITERAL(235, 4), // "arg1"
QT_MOC_LITERAL(240, 31), // "on_set_mid_point_circle_clicked"
QT_MOC_LITERAL(272, 5), // "delay"
QT_MOC_LITERAL(278, 35), // "on_set_another_point_circle_c..."
QT_MOC_LITERAL(314, 35), // "on_set_center_point_ellipse_c..."
QT_MOC_LITERAL(350, 28), // "on_boundary_fill_btn_clicked"
QT_MOC_LITERAL(379, 20), // "get_color_from_pixel"
QT_MOC_LITERAL(400, 1), // "p"
QT_MOC_LITERAL(402, 40), // "on_boundary_fill_8_connected_..."
QT_MOC_LITERAL(443, 25), // "on_flood_fill_btn_clicked"
QT_MOC_LITERAL(469, 37), // "on_flood_fill_btn_8_connected..."
QT_MOC_LITERAL(507, 27), // "on_select_old_color_clicked"
QT_MOC_LITERAL(535, 38), // "on_fill_color_combo_currentTe..."
QT_MOC_LITERAL(574, 37), // "on_toggle_set_vertex_scanline..."
QT_MOC_LITERAL(612, 23), // "on_draw_polygon_clicked"
QT_MOC_LITERAL(636, 24), // "on_translate_btn_clicked"
QT_MOC_LITERAL(661, 20), // "on_scale_btn_clicked"
QT_MOC_LITERAL(682, 21), // "on_rotate_btn_clicked"
QT_MOC_LITERAL(704, 20), // "on_shear_btn_clicked"
QT_MOC_LITERAL(725, 22), // "on_reflect_btn_clicked"
QT_MOC_LITERAL(748, 24), // "on_scanline_algo_clicked"
QT_MOC_LITERAL(773, 26), // "on_scanline_algo_2_clicked"
QT_MOC_LITERAL(800, 23), // "on_set_corner_1_clicked"
QT_MOC_LITERAL(824, 23), // "on_set_corner_2_clicked"
QT_MOC_LITERAL(848, 14), // "on_csa_clicked"
QT_MOC_LITERAL(863, 19), // "on_saveline_clicked"
QT_MOC_LITERAL(883, 14), // "on_sha_clicked"
QT_MOC_LITERAL(898, 22), // "on_setbezpoint_clicked"
QT_MOC_LITERAL(921, 24), // "on_clearbezpoint_clicked"
QT_MOC_LITERAL(946, 26) // "on_drawBezierCurve_clicked"

    },
    "MainWindow\0Mouse_Pressed\0\0showMousePosition\0"
    "QPoint&\0pos\0on_show_axes_clicked\0"
    "on_Draw_clicked\0on_set_point1_clicked\0"
    "on_set_point2_clicked\0on_pushButton_clicked\0"
    "on_show_grid_clicked\0updateSystemOnGridSizeUpdate\0"
    "on_grid_size_valueChanged\0arg1\0"
    "on_set_mid_point_circle_clicked\0delay\0"
    "on_set_another_point_circle_clicked\0"
    "on_set_center_point_ellipse_clicked\0"
    "on_boundary_fill_btn_clicked\0"
    "get_color_from_pixel\0p\0"
    "on_boundary_fill_8_connected_btn_clicked\0"
    "on_flood_fill_btn_clicked\0"
    "on_flood_fill_btn_8_connected_clicked\0"
    "on_select_old_color_clicked\0"
    "on_fill_color_combo_currentTextChanged\0"
    "on_toggle_set_vertex_scanline_clicked\0"
    "on_draw_polygon_clicked\0"
    "on_translate_btn_clicked\0on_scale_btn_clicked\0"
    "on_rotate_btn_clicked\0on_shear_btn_clicked\0"
    "on_reflect_btn_clicked\0on_scanline_algo_clicked\0"
    "on_scanline_algo_2_clicked\0"
    "on_set_corner_1_clicked\0on_set_corner_2_clicked\0"
    "on_csa_clicked\0on_saveline_clicked\0"
    "on_sha_clicked\0on_setbezpoint_clicked\0"
    "on_clearbezpoint_clicked\0"
    "on_drawBezierCurve_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      38,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  242,    2, 0x0a,    1 /* Public */,
       3,    1,  243,    2, 0x0a,    2 /* Public */,
       6,    0,  246,    2, 0x08,    4 /* Private */,
       7,    0,  247,    2, 0x08,    5 /* Private */,
       8,    0,  248,    2, 0x08,    6 /* Private */,
       9,    0,  249,    2, 0x08,    7 /* Private */,
      10,    0,  250,    2, 0x08,    8 /* Private */,
      11,    0,  251,    2, 0x08,    9 /* Private */,
      12,    0,  252,    2, 0x08,   10 /* Private */,
      13,    1,  253,    2, 0x08,   11 /* Private */,
      15,    0,  256,    2, 0x08,   13 /* Private */,
      16,    1,  257,    2, 0x08,   14 /* Private */,
      17,    0,  260,    2, 0x08,   16 /* Private */,
      18,    0,  261,    2, 0x08,   17 /* Private */,
      19,    0,  262,    2, 0x08,   18 /* Private */,
      20,    1,  263,    2, 0x08,   19 /* Private */,
      22,    0,  266,    2, 0x08,   21 /* Private */,
      23,    0,  267,    2, 0x08,   22 /* Private */,
      24,    0,  268,    2, 0x08,   23 /* Private */,
      25,    0,  269,    2, 0x08,   24 /* Private */,
      26,    1,  270,    2, 0x08,   25 /* Private */,
      27,    0,  273,    2, 0x08,   27 /* Private */,
      28,    0,  274,    2, 0x08,   28 /* Private */,
      29,    0,  275,    2, 0x08,   29 /* Private */,
      30,    0,  276,    2, 0x08,   30 /* Private */,
      31,    0,  277,    2, 0x08,   31 /* Private */,
      32,    0,  278,    2, 0x08,   32 /* Private */,
      33,    0,  279,    2, 0x08,   33 /* Private */,
      34,    0,  280,    2, 0x08,   34 /* Private */,
      35,    0,  281,    2, 0x08,   35 /* Private */,
      36,    0,  282,    2, 0x08,   36 /* Private */,
      37,    0,  283,    2, 0x08,   37 /* Private */,
      38,    0,  284,    2, 0x08,   38 /* Private */,
      39,    0,  285,    2, 0x08,   39 /* Private */,
      40,    0,  286,    2, 0x08,   40 /* Private */,
      41,    0,  287,    2, 0x08,   41 /* Private */,
      42,    0,  288,    2, 0x08,   42 /* Private */,
      43,    0,  289,    2, 0x08,   43 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::QColor, QMetaType::QPoint,   21,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->Mouse_Pressed(); break;
        case 1: _t->showMousePosition((*reinterpret_cast< std::add_pointer_t<QPoint&>>(_a[1]))); break;
        case 2: _t->on_show_axes_clicked(); break;
        case 3: _t->on_Draw_clicked(); break;
        case 4: _t->on_set_point1_clicked(); break;
        case 5: _t->on_set_point2_clicked(); break;
        case 6: _t->on_pushButton_clicked(); break;
        case 7: _t->on_show_grid_clicked(); break;
        case 8: _t->updateSystemOnGridSizeUpdate(); break;
        case 9: _t->on_grid_size_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 10: _t->on_set_mid_point_circle_clicked(); break;
        case 11: _t->delay((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 12: _t->on_set_another_point_circle_clicked(); break;
        case 13: _t->on_set_center_point_ellipse_clicked(); break;
        case 14: _t->on_boundary_fill_btn_clicked(); break;
        case 15: { QColor _r = _t->get_color_from_pixel((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QColor*>(_a[0]) = std::move(_r); }  break;
        case 16: _t->on_boundary_fill_8_connected_btn_clicked(); break;
        case 17: _t->on_flood_fill_btn_clicked(); break;
        case 18: _t->on_flood_fill_btn_8_connected_clicked(); break;
        case 19: _t->on_select_old_color_clicked(); break;
        case 20: _t->on_fill_color_combo_currentTextChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 21: _t->on_toggle_set_vertex_scanline_clicked(); break;
        case 22: _t->on_draw_polygon_clicked(); break;
        case 23: _t->on_translate_btn_clicked(); break;
        case 24: _t->on_scale_btn_clicked(); break;
        case 25: _t->on_rotate_btn_clicked(); break;
        case 26: _t->on_shear_btn_clicked(); break;
        case 27: _t->on_reflect_btn_clicked(); break;
        case 28: _t->on_scanline_algo_clicked(); break;
        case 29: _t->on_scanline_algo_2_clicked(); break;
        case 30: _t->on_set_corner_1_clicked(); break;
        case 31: _t->on_set_corner_2_clicked(); break;
        case 32: _t->on_csa_clicked(); break;
        case 33: _t->on_saveline_clicked(); break;
        case 34: _t->on_sha_clicked(); break;
        case 35: _t->on_setbezpoint_clicked(); break;
        case 36: _t->on_clearbezpoint_clicked(); break;
        case 37: _t->on_drawBezierCurve_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.offsetsAndSize,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_MainWindow_t
, QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QPoint &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QColor, std::false_type>, QtPrivate::TypeAndForceComplete<QPoint, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QString &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 38)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 38;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 38)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 38;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
