#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>
#include "xkbinfo.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ui->viewer->fill();

	connect(ui->viewer, SIGNAL(selectedKeycode(int)), this, SLOT(selectedKeyCode(int)));

	for (KeySym sym = 0; sym < 255; sym++) {
		QString name = XkbInfo::symName(sym);
		if (name.isEmpty())
			continue;
		ui->symCombobox->addItem(name);
		ui->symCombobox->setItemData(ui->symCombobox->count() - 1, quint32(sym), KeySymRole);
	}
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::selectedKeyCode(int kc) {
	KeySym sym = XkbInfo().keycodeSyms(0).value(kc).value(0, NoSymbol);
	int symComboIndex = ui->symCombobox->findData(quint32(sym), KeySymRole);
	if (symComboIndex >= 0)
		ui->symCombobox->setCurrentIndex(symComboIndex);
}
