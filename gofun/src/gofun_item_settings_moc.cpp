/****************************************************************************
** GofunItemSettings meta object code from reading C++ file 'gofun_item_settings.h'
**
** Created: Sun Nov 14 19:17:41 2004
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.3   edited Aug 5 16:40 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "gofun_item_settings.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *GofunItemSettings::className() const
{
    return "GofunItemSettings";
}

QMetaObject *GofunItemSettings::metaObj = 0;
static QMetaObjectCleanUp cleanUp_GofunItemSettings( "GofunItemSettings", &GofunItemSettings::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString GofunItemSettings::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GofunItemSettings", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString GofunItemSettings::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GofunItemSettings", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* GofunItemSettings::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = GofunSettingsDlg::staticMetaObject();
    static const QUMethod slot_0 = {"iconDialog", 0, 0 };
    static const QUMethod slot_1 = {"dirDialog", 0, 0 };
    static const QUMethod slot_2 = {"addEnvVar", 0, 0 };
    static const QUMethod slot_3 = {"remEnvVar", 0, 0 };
    static const QUParameter param_slot_4[] = {
	{ 0, &static_QUType_ptr, "QListViewItem", QUParameter::In },
	{ 0, &static_QUType_varptr, "\x0e", QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"envItemEdit", 3, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"userChkToggled", 1, param_slot_5 };
    static const QMetaData slot_tbl[] = {
	{ "iconDialog()", &slot_0, QMetaData::Public },
	{ "dirDialog()", &slot_1, QMetaData::Public },
	{ "addEnvVar()", &slot_2, QMetaData::Public },
	{ "remEnvVar()", &slot_3, QMetaData::Public },
	{ "envItemEdit(QListViewItem*,const QPoint&,int)", &slot_4, QMetaData::Public },
	{ "userChkToggled(bool)", &slot_5, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"GofunItemSettings", parentObject,
	slot_tbl, 6,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_GofunItemSettings.setMetaObject( metaObj );
    return metaObj;
}

void* GofunItemSettings::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "GofunItemSettings" ) )
	return this;
    return GofunSettingsDlg::qt_cast( clname );
}

bool GofunItemSettings::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: iconDialog(); break;
    case 1: dirDialog(); break;
    case 2: addEnvVar(); break;
    case 3: remEnvVar(); break;
    case 4: envItemEdit((QListViewItem*)static_QUType_ptr.get(_o+1),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+2)),(int)static_QUType_int.get(_o+3)); break;
    case 5: userChkToggled((bool)static_QUType_bool.get(_o+1)); break;
    default:
	return GofunSettingsDlg::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool GofunItemSettings::qt_emit( int _id, QUObject* _o )
{
    return GofunSettingsDlg::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool GofunItemSettings::qt_property( int id, int f, QVariant* v)
{
    return GofunSettingsDlg::qt_property( id, f, v);
}

bool GofunItemSettings::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
