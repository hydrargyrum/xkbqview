#include "xkbinfo.h"
#include <QX11Info>
#include <X11/XKBlib.h>
#include <X11/extensions/XKBfile.h>
#include <X11/extensions/XKBgeom.h>


XkbInfo::XkbInfo(Display *dpy, QObject *parent) :
	QObject(parent), m_dpy(dpy), m_xkb(0)
{
	if (!m_dpy)
		m_dpy = QX11Info::display();

	m_xkb = XkbGetMap(m_dpy, XkbAllMapComponentsMask, XkbUseCoreKbd);
	XkbGetGeometry(m_dpy, m_xkb);
	XkbGetNames(m_dpy, XkbAllNamesMask, m_xkb);
	XkbGetCompatMap(m_dpy, XkbAllCompatMask, m_xkb);
}

XkbInfo::~XkbInfo() {
	if (m_xkb)
		XkbFreeKeyboard(m_xkb, 0, True);
}

const _XkbDesc *XkbInfo::xkb() const {
	return m_xkb;
}

QMap<KeyCode, QList<KeySym> > XkbInfo::keycodeSyms(int group) const {
	QMap<KeyCode, QList<KeySym> > keycodeSyms;
	for (int kc = m_xkb->min_key_code; kc < m_xkb->max_key_code; kc++) {
		XkbSymMapRec *symmap = m_xkb->map->key_sym_map + kc;
		if (XkbKeyNumGroups(m_xkb, kc) > 0) {
			for (int shiftLevel = 0; shiftLevel < XkbKeyGroupWidth(m_xkb, kc, 0); shiftLevel++) {
				keycodeSyms[kc].append(XkbKeySymEntry(m_xkb, kc, shiftLevel, 0));
			}
		}
	}
	return keycodeSyms;
}

QMap<QString, KeyCode> XkbInfo::keycodesByName() const {
	QMap<QString, KeyCode> ret;
	for (int i = m_xkb->min_key_code; i <= m_xkb->max_key_code; i++) {
		ret[getName4(m_xkb->names->keys[i])] = KeyCode(i);
	}
	return ret;
}

QString XkbInfo::getAtom(Atom atom) const {
	char *cname = XGetAtomName(m_dpy, atom);
	QString qname = QString::fromUtf8(cname);
	XFree(cname);
	return qname;
}

QString XkbInfo::getName4(const XkbKeyNameRec &xname) {
	return QString::fromAscii(xname.name, qstrnlen(xname.name, XkbKeyNameLength));
}
