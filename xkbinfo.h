#ifndef XKBINFO_H
#define XKBINFO_H

#include <QList>
#include <QMap>
#include <QObject>
#include <QX11Info>
#include <X11/X.h>


class _XkbDesc;
class _XDisplay;
class _XkbKeyNameRec;

class XkbInfo : public QObject {
	Q_OBJECT

public:
	explicit XkbInfo(_XDisplay *dpy = 0, QObject *parent = 0);
	~XkbInfo();

	const _XkbDesc *xkb() const;

	void get();
	QMap<QString, KeyCode> keycodesByName() const;
	QMap<KeyCode, QList<KeySym> > keycodeSyms(int group) const;

	QString getAtom(Atom atom) const;

	static QString getName4(const _XkbKeyNameRec &xname);
	
private:
	_XDisplay *m_dpy;
	_XkbDesc *m_xkb;
};

#endif // XKBINFO_H
