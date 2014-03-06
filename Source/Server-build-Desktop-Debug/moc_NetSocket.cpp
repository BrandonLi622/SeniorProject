/****************************************************************************
** Meta object code from reading C++ file 'NetSocket.hh'
**
** Created:
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Server/NetSocket.hh"
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
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      26,   11,   10,   10, 0x05,
      87,   70,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
     119,   10,   10,   10, 0x0a,
     136,   10,   10,   10, 0x0a,
     193,  161,  149,   10, 0x0a,
     272,  236,   10,   10, 0x0a,
     330,  322,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_NetSocket[] = {
    "NetSocket\0\0msg,id,viewNum\0"
    "receivedMessage(QByteArray,quint32,quint32)\0"
    "viewNum,isMember\0displayViewChange(quint32,bool)\0"
    "connectSignals()\0gotMessage()\0QVariantMap\0"
    "messageSource,messageSourcePort\0"
    "deserializeMessage(QHostAddress*,quint16*)\0"
    "map,messageSource,messageSourcePort\0"
    "switchMessage(QVariantMap,QHostAddress*,quint16*)\0"
    "newView\0proposeViewChange(quint32)\0"
};

void NetSocket::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        NetSocket *_t = static_cast<NetSocket *>(_o);
        switch (_id) {
        case 0: _t->receivedMessage((*reinterpret_cast< QByteArray(*)>(_a[1])),(*reinterpret_cast< quint32(*)>(_a[2])),(*reinterpret_cast< quint32(*)>(_a[3]))); break;
        case 1: _t->displayViewChange((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 2: _t->connectSignals(); break;
        case 3: _t->gotMessage(); break;
        case 4: { QVariantMap _r = _t->deserializeMessage((*reinterpret_cast< QHostAddress*(*)>(_a[1])),(*reinterpret_cast< quint16*(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QVariantMap*>(_a[0]) = _r; }  break;
        case 5: _t->switchMessage((*reinterpret_cast< QVariantMap(*)>(_a[1])),(*reinterpret_cast< QHostAddress*(*)>(_a[2])),(*reinterpret_cast< quint16*(*)>(_a[3]))); break;
        case 6: _t->proposeViewChange((*reinterpret_cast< quint32(*)>(_a[1]))); break;
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
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void NetSocket::receivedMessage(QByteArray _t1, quint32 _t2, quint32 _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void NetSocket::displayViewChange(quint32 _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
