/****************************************************************************
** GofunWidget meta object code from reading C++ file 'gofun_widget.h'
**
** Created: Wed Nov 24 19:27:21 2004
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.3   edited Aug 5 16:40 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "gofun_widget.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *GofunWidget::className() const
{
    return "GofunWidget";
}

QMetaObject *GofunWidget::metaObj = 0;
static QMetaObjectCleanUp cleanUp_GofunWidget( "GofunWidget", &GofunWidget::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString GofunWidget::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GofunWidget", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString GofunWidget::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GofunWidget", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* GofunWidget::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"changeCategory", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ 0, &static_QUType_ptr, "QIconViewItem", QUParameter::In }
    };
    static const QUMethod slot_1 = {"executeItem", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ 0, &static_QUType_ptr, "QIconViewItem", QUParameter::In },
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"executeItem", 2, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ 0, &static_QUType_ptr, "QIconViewItem", QUParameter::In },
	{ 0, &static_QUType_varptr, "\x0e", QUParameter::In }
    };
    static const QUMethod slot_3 = {"rightClickedItem", 2, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"popupMenuItem", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"popupMenuSpace", 1, param_slot_5 };
    static const QUMethod slot_6 = {"openSettingsDlg", 0, 0 };
    static const QUMethod slot_7 = {"addCategory", 0, 0 };
    static const QUMethod slot_8 = {"showHelp", 0, 0 };
    static const QUMethod slot_9 = {"showAbout", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "changeCategory(int)", &slot_0, QMetaData::Public },
	{ "executeItem(QIconViewItem*)", &slot_1, QMetaData::Public },
	{ "executeItem(QIconViewItem*,const QString&)", &slot_2, QMetaData::Public },
	{ "rightClickedItem(QIconViewItem*,const QPoint&)", &slot_3, QMetaData::Public },
	{ "popupMenuItem(int)", &slot_4, QMetaData::Public },
	{ "popupMenuSpace(int)", &slot_5, QMetaData::Public },
	{ "openSettingsDlg()", &slot_6, QMetaData::Public },
	{ "addCategory()", &slot_7, QMetaData::Public },
	{ "showHelp()", &slot_8, QMetaData::Public },
	{ "showAbout()", &slot_9, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"GofunWidget", parentObject,
	slot_tbl, 10,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_GofunWidget.setMetaObject( metaObj );
    return metaObj;
}

void* GofunWidget::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "GofunWidget" ) )
	return this;
    return QWidget::qt_cast( clname );
}

bool GofunWidget::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: changeCategory((int)static_QUType_int.get(_o+1)); break;
    case 1: executeItem((QIconViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 2: executeItem((QIconViewItem*)static_QUType_ptr.get(_o+1),(const QString&)static_QUType_QString.get(_o+2)); break;
    case 3: rightClickedItem((QIconViewItem*)static_QUType_ptr.get(_o+1),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+2))); break;
    case 4: popupMenuItem((int)static_QUType_int.get(_o+1)); break;
    case 5: popupMenuSpace((int)static_QUType_int.get(_o+1)); break;
    case 6: openSettingsDlg(); break;
    case 7: addCategory(); break;
    case 8: showHelp(); break;
    case 9: showAbout(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool GofunWidget::qt_emit( int _id, QUObject* _o )
{
    return QWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool GofunWidget::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool GofunWidget::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
