/****************************************************************************
** GofunCatButton meta object code from reading C++ file 'gofun_cat_button.h'
**
** Created: Sun Nov 28 17:15:01 2004
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.3   edited Aug 5 16:40 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "gofun_cat_button.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *GofunCatButton::className() const
{
    return "GofunCatButton";
}

QMetaObject *GofunCatButton::metaObj = 0;
static QMetaObjectCleanUp cleanUp_GofunCatButton( "GofunCatButton", &GofunCatButton::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString GofunCatButton::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GofunCatButton", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString GofunCatButton::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GofunCatButton", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* GofunCatButton::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QPushButton::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"popupItemDnD", 1, param_slot_0 };
    static const QUMethod slot_1 = {"catSettings", 0, 0 };
    static const QUParameter param_slot_2[] = {
	{ 0, &static_QUType_ptr, "QResizeEvent", QUParameter::In }
    };
    static const QUMethod slot_2 = {"resizeEvent", 1, param_slot_2 };
    static const QMetaData slot_tbl[] = {
	{ "popupItemDnD(int)", &slot_0, QMetaData::Public },
	{ "catSettings()", &slot_1, QMetaData::Public },
	{ "resizeEvent(QResizeEvent*)", &slot_2, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"GofunCatButton", parentObject,
	slot_tbl, 3,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_GofunCatButton.setMetaObject( metaObj );
    return metaObj;
}

void* GofunCatButton::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "GofunCatButton" ) )
	return this;
    return QPushButton::qt_cast( clname );
}

bool GofunCatButton::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: popupItemDnD((int)static_QUType_int.get(_o+1)); break;
    case 1: catSettings(); break;
    case 2: resizeEvent((QResizeEvent*)static_QUType_ptr.get(_o+1)); break;
    default:
	return QPushButton::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool GofunCatButton::qt_emit( int _id, QUObject* _o )
{
    return QPushButton::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool GofunCatButton::qt_property( int id, int f, QVariant* v)
{
    return QPushButton::qt_property( id, f, v);
}

bool GofunCatButton::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
