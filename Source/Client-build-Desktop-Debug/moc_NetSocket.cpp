/****************************************************************************
** Meta object code from reading C++ file 'NetSocket.hh'
**
** Created:
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Client/NetSocket.hh"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NetSocket.hh' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_NetSocket[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      23,   11,   10,   10, 0x05,
      70,   62,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
     101,   97,   10,   10, 0x0a,
     140,  127,   10,   10, 0x0a,
     178,   10,   10,   10, 0x0a,
     195,   10,   10,   10, 0x0a,
     252,  220,  208,   10, 0x0a,
     331,  295,   10,   10, 0x0a,
     385,  381,   10,   10, 0x0a,
     416,  381,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_NetSocket[] = {
    "NetSocket\0\0msg,viewNum\0"
    "displayReturnedMsg(QByteArray,quint32)\0"
    "viewNum\0displayViewChange(quint32)\0"
    "msg\0sendRoundMessage(QString)\0"
    "address,port\0connectToServer(QHostAddress,quint16)\0"
    "connectSignals()\0gotMessage()\0QVariantMap\0"
    "messageSource,messageSourcePort\0"
    "deserializeMessage(QHostAddress*,quint16*)\0"
    "map,messageSource,messageSourcePort\0"
    "switchMessage(QVariantMap,QHostAddress*,quint16*)\0"
    "map\0processViewChange(QVariantMap)\0"
    "processReturnedMessage(QVariantMap)\0"
};

void NetSocket::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        NetSocket *_t = static_cast<NetSocket *>(_o);
        switch (_id) {
        case 0: _t->displayReturnedMsg((*reinterpret_cast< QByteArray(*)>(_a[1])),(*reinterpret_cast< quint32(*)>(_a[2]))); break;
        case 1: _t->displayViewChange((*reinterpret_cast< quint32(*)>(_a[1]))); break;
        case 2: _t->sendRoundMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->connectToServer((*reinterpret_cast< QHostAddress(*)>(_a[1])),(*reinterpret_cast< quint16(*)>(_a[2]))); break;
        case 4: _t->connectSignals(); break;
        case 5: _t->gotMessage(); break;
        case 6: { QVariantMap _r = _t->deserializeMessage((*reinterpret_cast< QHostAddress*(*)>(_a[1])),(*reinterpret_cast< quint16*(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QVariantMap*>(_a[0]) = _r; }  break;
        case 7: _t->switchMessage((*reinterpret_cast< QVariantMap(*)>(_a[1])),(*reinterpret_cast< QHostAddress*(*)>(_a[2])),(*reinterpret_cast< quint16*(*)>(_a[3]))); break;
        case 8: _t->processViewChange((*reinterpret_cast< QVariantMap(*)>(_a[1]))); break;
        case 9: _t->processReturnedMessage((*reinterpret_cast< QVariantMap(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData NetSocket::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject NetSocket::staticMetaObject = {
    { &QUdpSocket::staticMetaObject, qt_meta_stringdata_NetSocket,
      qt_meta_data_NetSocket, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &NetSocket::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *NetSocket::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *NetSocket::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_NetSocket))
        return static_cast<void*>(const_cast< NetSocket*>(this));
    return QUdpSocket::qt_metacast(_clname);
}

int NetSocket::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QUdpSocket::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void NetSocket::displayReturnedMsg(QByteArray _t1, quint32 _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void NetSocket::displayViewChange(quint32 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
