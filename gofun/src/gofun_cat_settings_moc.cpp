/****************************************************************************
** GofunCatSettings meta object code from reading C++ file 'gofun_cat_settings.h'
**
** Created: Tue Nov 30 20:24:13 2004
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.3   edited Aug 5 16:40 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "gofun_cat_settings.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *GofunCatSettings::className() const
{
    return "GofunCatSettings";
}

QMetaObject *GofunCatSettings::metaObj = 0;
static QMetaObjectCleanUp cleanUp_GofunCatSettings( "GofunCatSettings", &GofunCatSettings::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString GofunCatSettings::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GofunCatSettings", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString GofunCatSettings::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GofunCatSettings", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* GofunCatSettings::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = GofunSettingsDlg::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"GofunCatSettings", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_GofunCatSettings.setMetaObject( metaObj );
    return metaObj;
}

void* GofunCatSettings::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "GofunCatSettings" ) )
	return this;
    return GofunSettingsDlg::qt_cast( clname );
}

bool GofunCatSettings::qt_invoke( int _id, QUObject* _o )
{
    return GofunSettingsDlg::qt_invoke(_id,_o);
}

bool GofunCatSettings::qt_emit( int _id, QUObject* _o )
{
    return GofunSettingsDlg::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool GofunCatSettings::qt_property( int id, int f, QVariant* v)
{
    return GofunSettingsDlg::qt_property( id, f, v);
}

bool GofunCatSettings::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
