/****************************************************************************
** Meta object code from reading C++ file 'bmsmanager.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/backend/bmsmanager.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bmsmanager.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN10BmsManagerE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN10BmsManagerE = QtMocHelpers::stringData(
    "BmsManager",
    "snapshotChanged",
    "",
    "connectedChanged",
    "connectionStateChanged",
    "errorChanged",
    "statusMessageChanged",
    "lastFrameHexChanged",
    "handleFrame",
    "frame",
    "handleReaderConnectionChanged",
    "connected",
    "handleReaderTransportError",
    "BmsSerialReader::TransportErrorCode",
    "code",
    "errorMessage",
    "recoverable",
    "handlePollTimeout",
    "handleResponseTimeout",
    "handleReconnectTimeout",
    "initializeFanControl",
    "gpioPin",
    "pwmMode",
    "pwmFrequency",
    "setFanDigitalThresholds",
    "onTemp",
    "offTemp",
    "setFanPwmRange",
    "minTemp",
    "maxTemp",
    "minPwm",
    "maxPwm",
    "setFanState",
    "on",
    "setFanSpeed",
    "pwmPercent",
    "isFanInitialized",
    "connectionState",
    "BmsManager::ConnectionState",
    "errorCode",
    "BmsManager::ErrorCode",
    "statusMessage",
    "lastFrameHex",
    "ConnectionState",
    "Idle",
    "Connecting",
    "Online",
    "Degraded",
    "Error",
    "ErrorCode",
    "None",
    "InvalidConfig",
    "OpenFailed",
    "PortClosed",
    "WriteFailed",
    "Timeout",
    "InvalidFrame",
    "ChecksumMismatch",
    "DeviceStatusError",
    "TransportError",
    "Unknown"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN10BmsManagerE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      22,   14, // methods
       6,  216, // properties
       2,  246, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  146,    2, 0x06,    9 /* Public */,
       3,    0,  147,    2, 0x06,   10 /* Public */,
       4,    0,  148,    2, 0x06,   11 /* Public */,
       5,    0,  149,    2, 0x06,   12 /* Public */,
       6,    0,  150,    2, 0x06,   13 /* Public */,
       7,    0,  151,    2, 0x06,   14 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       8,    1,  152,    2, 0x08,   15 /* Private */,
      10,    1,  155,    2, 0x08,   17 /* Private */,
      12,    3,  158,    2, 0x08,   19 /* Private */,
      17,    0,  165,    2, 0x08,   23 /* Private */,
      18,    0,  166,    2, 0x08,   24 /* Private */,
      19,    0,  167,    2, 0x08,   25 /* Private */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
      20,    3,  168,    2, 0x02,   26 /* Public */,
      20,    2,  175,    2, 0x22,   30 /* Public | MethodCloned */,
      20,    1,  180,    2, 0x22,   33 /* Public | MethodCloned */,
      24,    2,  183,    2, 0x02,   35 /* Public */,
      27,    4,  188,    2, 0x02,   38 /* Public */,
      27,    3,  197,    2, 0x22,   43 /* Public | MethodCloned */,
      27,    2,  204,    2, 0x22,   47 /* Public | MethodCloned */,
      32,    1,  209,    2, 0x02,   50 /* Public */,
      34,    1,  212,    2, 0x02,   52 /* Public */,
      36,    0,  215,    2, 0x102,   54 /* Public | MethodIsConst  */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QByteArray,    9,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void, 0x80000000 | 13, QMetaType::QString, QMetaType::Bool,   14,   15,   16,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Bool, QMetaType::Int, QMetaType::Bool, QMetaType::Int,   21,   22,   23,
    QMetaType::Bool, QMetaType::Int, QMetaType::Bool,   21,   22,
    QMetaType::Bool, QMetaType::Int,   21,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,   25,   26,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Int, QMetaType::Int,   28,   29,   30,   31,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Int,   28,   29,   30,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,   28,   29,
    QMetaType::Void, QMetaType::Bool,   33,
    QMetaType::Void, QMetaType::Int,   35,
    QMetaType::Bool,

 // properties: name, type, flags, notifyId, revision
      11, QMetaType::Bool, 0x00015001, uint(1), 0,
      37, 0x80000000 | 38, 0x00015009, uint(2), 0,
      39, 0x80000000 | 40, 0x00015009, uint(3), 0,
      15, QMetaType::QString, 0x00015001, uint(3), 0,
      41, QMetaType::QString, 0x00015001, uint(4), 0,
      42, QMetaType::QString, 0x00015001, uint(5), 0,

 // enums: name, alias, flags, count, data
      43,   43, 0x2,    5,  256,
      49,   49, 0x2,   11,  266,

 // enum data: key, value
      44, uint(BmsManager::ConnectionState::Idle),
      45, uint(BmsManager::ConnectionState::Connecting),
      46, uint(BmsManager::ConnectionState::Online),
      47, uint(BmsManager::ConnectionState::Degraded),
      48, uint(BmsManager::ConnectionState::Error),
      50, uint(BmsManager::ErrorCode::None),
      51, uint(BmsManager::ErrorCode::InvalidConfig),
      52, uint(BmsManager::ErrorCode::OpenFailed),
      53, uint(BmsManager::ErrorCode::PortClosed),
      54, uint(BmsManager::ErrorCode::WriteFailed),
      55, uint(BmsManager::ErrorCode::Timeout),
      56, uint(BmsManager::ErrorCode::InvalidFrame),
      57, uint(BmsManager::ErrorCode::ChecksumMismatch),
      58, uint(BmsManager::ErrorCode::DeviceStatusError),
      59, uint(BmsManager::ErrorCode::TransportError),
      60, uint(BmsManager::ErrorCode::Unknown),

       0        // eod
};

Q_CONSTINIT const QMetaObject BmsManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ZN10BmsManagerE.offsetsAndSizes,
    qt_meta_data_ZN10BmsManagerE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN10BmsManagerE_t,
        // property 'connected'
        QtPrivate::TypeAndForceComplete<bool, std::true_type>,
        // property 'connectionState'
        QtPrivate::TypeAndForceComplete<BmsManager::ConnectionState, std::true_type>,
        // property 'errorCode'
        QtPrivate::TypeAndForceComplete<BmsManager::ErrorCode, std::true_type>,
        // property 'errorMessage'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // property 'statusMessage'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // property 'lastFrameHex'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // enum 'ConnectionState'
        QtPrivate::TypeAndForceComplete<BmsManager::ConnectionState, std::true_type>,
        // enum 'ErrorCode'
        QtPrivate::TypeAndForceComplete<BmsManager::ErrorCode, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<BmsManager, std::true_type>,
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
        // method 'handleFrame'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QByteArray &, std::false_type>,
        // method 'handleReaderConnectionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'handleReaderTransportError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<BmsSerialReader::TransportErrorCode, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'handlePollTimeout'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handleResponseTimeout'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handleReconnectTimeout'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'initializeFanControl'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'initializeFanControl'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'initializeFanControl'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'setFanDigitalThresholds'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'setFanPwmRange'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'setFanPwmRange'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'setFanPwmRange'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'setFanState'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'setFanSpeed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'isFanInitialized'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>
    >,
    nullptr
} };

void BmsManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<BmsManager *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->snapshotChanged(); break;
        case 1: _t->connectedChanged(); break;
        case 2: _t->connectionStateChanged(); break;
        case 3: _t->errorChanged(); break;
        case 4: _t->statusMessageChanged(); break;
        case 5: _t->lastFrameHexChanged(); break;
        case 6: _t->handleFrame((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1]))); break;
        case 7: _t->handleReaderConnectionChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 8: _t->handleReaderTransportError((*reinterpret_cast< std::add_pointer_t<BmsSerialReader::TransportErrorCode>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[3]))); break;
        case 9: _t->handlePollTimeout(); break;
        case 10: _t->handleResponseTimeout(); break;
        case 11: _t->handleReconnectTimeout(); break;
        case 12: { bool _r = _t->initializeFanControl((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 13: { bool _r = _t->initializeFanControl((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 14: { bool _r = _t->initializeFanControl((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 15: _t->setFanDigitalThresholds((*reinterpret_cast< std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[2]))); break;
        case 16: _t->setFanPwmRange((*reinterpret_cast< std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[4]))); break;
        case 17: _t->setFanPwmRange((*reinterpret_cast< std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3]))); break;
        case 18: _t->setFanPwmRange((*reinterpret_cast< std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[2]))); break;
        case 19: _t->setFanState((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 20: _t->setFanSpeed((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 21: { bool _r = _t->isFanInitialized();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _q_method_type = void (BmsManager::*)();
            if (_q_method_type _q_method = &BmsManager::snapshotChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _q_method_type = void (BmsManager::*)();
            if (_q_method_type _q_method = &BmsManager::connectedChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _q_method_type = void (BmsManager::*)();
            if (_q_method_type _q_method = &BmsManager::connectionStateChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _q_method_type = void (BmsManager::*)();
            if (_q_method_type _q_method = &BmsManager::errorChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _q_method_type = void (BmsManager::*)();
            if (_q_method_type _q_method = &BmsManager::statusMessageChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _q_method_type = void (BmsManager::*)();
            if (_q_method_type _q_method = &BmsManager::lastFrameHexChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->isConnected(); break;
        case 1: *reinterpret_cast< BmsManager::ConnectionState*>(_v) = _t->connectionState(); break;
        case 2: *reinterpret_cast< BmsManager::ErrorCode*>(_v) = _t->errorCode(); break;
        case 3: *reinterpret_cast< QString*>(_v) = _t->errorMessage(); break;
        case 4: *reinterpret_cast< QString*>(_v) = _t->statusMessage(); break;
        case 5: *reinterpret_cast< QString*>(_v) = _t->lastFrameHex(); break;
        default: break;
        }
    }
}

const QMetaObject *BmsManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BmsManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN10BmsManagerE.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int BmsManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 22)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 22;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void BmsManager::snapshotChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void BmsManager::connectedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void BmsManager::connectionStateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void BmsManager::errorChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void BmsManager::statusMessageChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void BmsManager::lastFrameHexChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}
QT_WARNING_POP
