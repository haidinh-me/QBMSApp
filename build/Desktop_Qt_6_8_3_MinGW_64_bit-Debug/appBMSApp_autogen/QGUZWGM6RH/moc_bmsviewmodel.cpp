/****************************************************************************
** Meta object code from reading C++ file 'bmsviewmodel.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/viewmodels/bmsviewmodel.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bmsviewmodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN12BmsViewModelE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN12BmsViewModelE = QtMocHelpers::stringData(
    "BmsViewModel",
    "snapshotChanged",
    "",
    "connectedChanged",
    "connectionStateChanged",
    "errorChanged",
    "statusMessageChanged",
    "lastFrameHexChanged",
    "availablePortsChanged",
    "portNameChanged",
    "baudRateChanged",
    "pollingIntervalMsChanged",
    "connectToUart",
    "disconnectFromUart",
    "refreshNow",
    "reloadAvailablePorts",
    "snapshot",
    "QVariantMap",
    "connected",
    "connectionState",
    "connectionStateText",
    "errorCode",
    "errorMessage",
    "statusMessage",
    "lastFrameHex",
    "availablePorts",
    "portName",
    "baudRate",
    "pollingIntervalMs"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN12BmsViewModelE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
      12,  112, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      10,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   98,    2, 0x06,   13 /* Public */,
       3,    0,   99,    2, 0x06,   14 /* Public */,
       4,    0,  100,    2, 0x06,   15 /* Public */,
       5,    0,  101,    2, 0x06,   16 /* Public */,
       6,    0,  102,    2, 0x06,   17 /* Public */,
       7,    0,  103,    2, 0x06,   18 /* Public */,
       8,    0,  104,    2, 0x06,   19 /* Public */,
       9,    0,  105,    2, 0x06,   20 /* Public */,
      10,    0,  106,    2, 0x06,   21 /* Public */,
      11,    0,  107,    2, 0x06,   22 /* Public */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
      12,    0,  108,    2, 0x02,   23 /* Public */,
      13,    0,  109,    2, 0x02,   24 /* Public */,
      14,    0,  110,    2, 0x02,   25 /* Public */,
      15,    0,  111,    2, 0x02,   26 /* Public */,

 // signals: parameters
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

 // methods: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags, notifyId, revision
      16, 0x80000000 | 17, 0x00015009, uint(0), 0,
      18, QMetaType::Bool, 0x00015001, uint(1), 0,
      19, QMetaType::Int, 0x00015001, uint(2), 0,
      20, QMetaType::QString, 0x00015001, uint(2), 0,
      21, QMetaType::Int, 0x00015001, uint(3), 0,
      22, QMetaType::QString, 0x00015001, uint(3), 0,
      23, QMetaType::QString, 0x00015001, uint(4), 0,
      24, QMetaType::QString, 0x00015001, uint(5), 0,
      25, QMetaType::QStringList, 0x00015001, uint(6), 0,
      26, QMetaType::QString, 0x00015103, uint(7), 0,
      27, QMetaType::Int, 0x00015103, uint(8), 0,
      28, QMetaType::Int, 0x00015103, uint(9), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject BmsViewModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ZN12BmsViewModelE.offsetsAndSizes,
    qt_meta_data_ZN12BmsViewModelE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN12BmsViewModelE_t,
        // property 'snapshot'
        QtPrivate::TypeAndForceComplete<QVariantMap, std::true_type>,
        // property 'connected'
        QtPrivate::TypeAndForceComplete<bool, std::true_type>,
        // property 'connectionState'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'connectionStateText'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // property 'errorCode'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'errorMessage'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // property 'statusMessage'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // property 'lastFrameHex'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // property 'availablePorts'
        QtPrivate::TypeAndForceComplete<QStringList, std::true_type>,
        // property 'portName'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // property 'baudRate'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'pollingIntervalMs'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<BmsViewModel, std::true_type>,
        // method 'snapshotChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'connectedChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'connectionStateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'errorChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'statusMessageChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'lastFrameHexChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'availablePortsChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'portNameChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'baudRateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'pollingIntervalMsChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'connectToUart'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'disconnectFromUart'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'refreshNow'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'reloadAvailablePorts'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void BmsViewModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<BmsViewModel *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->snapshotChanged(); break;
        case 1: _t->connectedChanged(); break;
        case 2: _t->connectionStateChanged(); break;
        case 3: _t->errorChanged(); break;
        case 4: _t->statusMessageChanged(); break;
        case 5: _t->lastFrameHexChanged(); break;
        case 6: _t->availablePortsChanged(); break;
        case 7: _t->portNameChanged(); break;
        case 8: _t->baudRateChanged(); break;
        case 9: _t->pollingIntervalMsChanged(); break;
        case 10: _t->connectToUart(); break;
        case 11: _t->disconnectFromUart(); break;
        case 12: _t->refreshNow(); break;
        case 13: _t->reloadAvailablePorts(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _q_method_type = void (BmsViewModel::*)();
            if (_q_method_type _q_method = &BmsViewModel::snapshotChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _q_method_type = void (BmsViewModel::*)();
            if (_q_method_type _q_method = &BmsViewModel::connectedChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _q_method_type = void (BmsViewModel::*)();
            if (_q_method_type _q_method = &BmsViewModel::connectionStateChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _q_method_type = void (BmsViewModel::*)();
            if (_q_method_type _q_method = &BmsViewModel::errorChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _q_method_type = void (BmsViewModel::*)();
            if (_q_method_type _q_method = &BmsViewModel::statusMessageChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _q_method_type = void (BmsViewModel::*)();
            if (_q_method_type _q_method = &BmsViewModel::lastFrameHexChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _q_method_type = void (BmsViewModel::*)();
            if (_q_method_type _q_method = &BmsViewModel::availablePortsChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
        {
            using _q_method_type = void (BmsViewModel::*)();
            if (_q_method_type _q_method = &BmsViewModel::portNameChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 7;
                return;
            }
        }
        {
            using _q_method_type = void (BmsViewModel::*)();
            if (_q_method_type _q_method = &BmsViewModel::baudRateChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 8;
                return;
            }
        }
        {
            using _q_method_type = void (BmsViewModel::*)();
            if (_q_method_type _q_method = &BmsViewModel::pollingIntervalMsChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 9;
                return;
            }
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QVariantMap*>(_v) = _t->snapshot(); break;
        case 1: *reinterpret_cast< bool*>(_v) = _t->isConnected(); break;
        case 2: *reinterpret_cast< int*>(_v) = _t->connectionState(); break;
        case 3: *reinterpret_cast< QString*>(_v) = _t->connectionStateText(); break;
        case 4: *reinterpret_cast< int*>(_v) = _t->errorCode(); break;
        case 5: *reinterpret_cast< QString*>(_v) = _t->errorMessage(); break;
        case 6: *reinterpret_cast< QString*>(_v) = _t->statusMessage(); break;
        case 7: *reinterpret_cast< QString*>(_v) = _t->lastFrameHex(); break;
        case 8: *reinterpret_cast< QStringList*>(_v) = _t->availablePorts(); break;
        case 9: *reinterpret_cast< QString*>(_v) = _t->portName(); break;
        case 10: *reinterpret_cast< int*>(_v) = _t->baudRate(); break;
        case 11: *reinterpret_cast< int*>(_v) = _t->pollingIntervalMs(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 9: _t->setPortName(*reinterpret_cast< QString*>(_v)); break;
        case 10: _t->setBaudRate(*reinterpret_cast< int*>(_v)); break;
        case 11: _t->setPollingIntervalMs(*reinterpret_cast< int*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *BmsViewModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BmsViewModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN12BmsViewModelE.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int BmsViewModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 14;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void BmsViewModel::snapshotChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void BmsViewModel::connectedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void BmsViewModel::connectionStateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void BmsViewModel::errorChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void BmsViewModel::statusMessageChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void BmsViewModel::lastFrameHexChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void BmsViewModel::availablePortsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void BmsViewModel::portNameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void BmsViewModel::baudRateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void BmsViewModel::pollingIntervalMsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}
QT_WARNING_POP
