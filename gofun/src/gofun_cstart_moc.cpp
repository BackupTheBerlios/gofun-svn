/****************************************************************************
** GofunCStart meta object code from reading C++ file 'gofun_cstart.h'
**
** Created: Sun Nov 14 19:17:43 2004
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.3   edited Aug 5 16:40 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "gofun_cstart.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *GofunCStart::className() const
{
    return "GofunCStart";
}

QMetaObject *GofunCStart::metaObj = 0;
static QMetaObjectCleanUp cleanUp_GofunCStart( "GofunCStart", &GofunCStart::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString GofunCStart::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GofunCStart", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString GofunCStart::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GofunCStart", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* GofunCStart::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"start", 0, 0 };
    static const QUParameter param_slot_1[] = {
	{ 0, &static_QUType_ptr, "GofunItem", QUParameter::In }
    };
    static const QUMethod slot_1 = {"load", 1, param_slot_1 };
    static const QMetaData slot_tbl[] = {
	{ "start()", &slot_0, QMetaData::Public },
	{ "load(GofunItem*)", &slot_1, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"GofunCStart", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_GofunCStart.setMetaObject( metaObj );
    return metaObj;
}

void* GofunCStart::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "GofunCStart" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool GofunCStart::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: start(); break;
    case 1: load((GofunItem*)static_QUType_ptr.get(_o+1)); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool GofunCStart::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool GofunCStart::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool GofunCStart::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
