#ifndef XKBVIEWER_H
#define XKBVIEWER_H

#include <QGraphicsView>

class XkbInfo;

class XkbViewer : public QGraphicsView {
	Q_OBJECT

public:
	explicit XkbViewer(QWidget *parent = 0);
	~XkbViewer();

	void fill();

public slots:
	void setScalePercent(int percent);

private slots:
	void sceneSelectionChanged();

private:
	QScopedPointer<XkbInfo> m_info;
};

#endif // XKBVIEWER_H
