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

	static const Qt::ItemDataRole KeyCodeNameRole = Qt::ItemDataRole(Qt::UserRole);
	static const Qt::ItemDataRole KeyCodeRole = Qt::ItemDataRole(Qt::UserRole + 1);

signals:
	void selected(int keycode);

public slots:
	void setScalePercent(int percent);

private slots:
	void sceneSelectionChanged();

private:
	QScopedPointer<XkbInfo> m_info;
};

#endif // XKBVIEWER_H
