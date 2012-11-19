#ifndef XKBVIEWER_H
#define XKBVIEWER_H

#include <QGraphicsView>

class _XkbDesc;

class XkbViewer : public QGraphicsView {
	Q_OBJECT

public:
	explicit XkbViewer(QWidget *parent = 0);

	void fill();

public slots:
	void setScalePercent(int percent);

private:
	QList<QPolygon> getShapes();

	_XkbDesc *m_xkb;
};

#endif // XKBVIEWER_H
