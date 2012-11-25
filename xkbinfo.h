#ifndef XKBINFO_H
#define XKBINFO_H

#include <QList>
#include <QMap>
#include <QObject>
#include <QSharedPointer>
#include <QX11Info>
#include <X11/X.h>
#include "xkbgeom.h"


class _XkbDesc;
class _XDisplay;
class _XkbKeyNameRec;

class XkbInfo {
public:
	explicit XkbInfo(_XDisplay *dpy = 0);
	explicit XkbInfo(QSharedPointer<_XkbDesc> xkb);
	~XkbInfo();

	_XDisplay *display() const;
	_XkbDesc *xkb() const;
	QSharedPointer<_XkbDesc> sharedXkb() const;

	XkbGeom geom() const;
	QMap<QString, KeyCode> keycodesByName() const;
	QMap<KeyCode, QList<KeySym> > keycodeSyms(int group) const;


	QString getAtom(Atom atom) const;
	static QString getAtom(Display *dpy, Atom atom);
	static QString getName4(const _XkbKeyNameRec &xname);

private:
	static void freeKeyboard(_XkbDesc *xkb);

	QSharedPointer<_XkbDesc> m_xkb;
};

#endif // XKBINFO_H
