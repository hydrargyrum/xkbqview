#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	static const Qt::ItemDataRole KeySymRole = Qt::ItemDataRole(Qt::UserRole + 10);

	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void selectedKeyCode(int kc);
	
private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
