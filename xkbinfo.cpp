#include "xkbinfo.h"
#include <QX11Info>
#include <X11/XKBlib.h>
#include <X11/extensions/XKBfile.h>
#include <X11/extensions/XKBgeom.h>


XkbInfo::XkbInfo(Display *dpy) :
	m_xkb(0)
{
	if (!dpy)
		dpy = QX11Info::display();

	XkbDescRec *xkb = XkbGetMap(dpy, XkbAllMapComponentsMask, XkbUseCoreKbd);
	m_xkb = QSharedPointer<XkbDescRec>(xkb, &XkbInfo::freeKeyboard);

	XkbGetGeometry(dpy, m_xkb.data());
	XkbGetNames(dpy, XkbAllNamesMask, m_xkb.data());
	XkbGetCompatMap(dpy, XkbAllCompatMask, m_xkb.data());
}

XkbInfo::XkbInfo(QSharedPointer<_XkbDesc> xkb) :
	m_xkb(xkb)
{
}

XkbInfo::~XkbInfo() {
}

void XkbInfo::freeKeyboard(_XkbDesc *xkb) {
	XkbFreeKeyboard(xkb, 0, true);
}

_XkbDesc *XkbInfo::xkb() const {
	return m_xkb.data();
}

QSharedPointer<_XkbDesc> XkbInfo::sharedXkb() const {
	return m_xkb;
}

_XDisplay *XkbInfo::display() const {
	return m_xkb->dpy;
}

XkbGeom XkbInfo::geom() const {
	return XkbGeom(m_xkb);
}

#include <QFile>

enum QXkbFileDataType {
	kXkbFileTypes = 0,
	kXkbFileCompats = 1,
	kXkbFileSymbols = 2,
	kXkbFileKeycodes = 4,
	kXkbFileGeometry = 5
};

QMap<KeyCode, QList<KeySym> > XkbInfo::keycodeSyms(int group) const {
	QMap<KeyCode, QList<KeySym> > keycodeSyms;

	for (int kc = m_xkb->min_key_code; kc < m_xkb->max_key_code; kc++) {
		if (XkbKeyNumGroups(m_xkb, kc) > 0) {
			for (int shiftLevel = 0; shiftLevel < XkbKeyGroupWidth(m_xkb, kc, group); shiftLevel++) {
				keycodeSyms[kc].append(XkbKeySymEntry(m_xkb, kc, shiftLevel, group));
				XkbKeyTypeRec *type = XkbCMKeyType(m_xkb->map, kc, group);
				int i = 0;
			}
		}
	}

	FILE *f = fopen("/tmp/1.xkb", "w");
	XkbFileInfo finfo;
	finfo.xkb = m_xkb.data();
	finfo.defined = 0;
	finfo.type = 3;
	XkbWriteXKBFile(f, &finfo, False, 0, 0);
	fclose(f);

	return keycodeSyms;
}

QMap<QString, KeyCode> XkbInfo::keycodesByName() const {
	QMap<QString, KeyCode> ret;
	for (int kc = m_xkb->min_key_code; kc <= m_xkb->max_key_code; kc++) {
		ret[getName4(m_xkb->names->keys[kc])] = KeyCode(kc);
	}
	return ret;
}

QString XkbInfo::getAtom(Display *dpy, Atom atom) {
	char *cname = XGetAtomName(dpy, atom);
	QString qname = QString::fromUtf8(cname);
	XFree(cname);
	return qname;
}

QString XkbInfo::getAtom(Atom atom) const {
	return getAtom(m_xkb->dpy, atom);
}

QString XkbInfo::getName4(const XkbKeyNameRec &xname) {
	return QString::fromAscii(xname.name, qstrnlen(xname.name, XkbKeyNameLength));
}

QString XkbInfo::symName(KeySym symbol) {
	return XKeysymToString(symbol);
}
