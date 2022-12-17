#pragma once
#include "wavefield.h"

#include <QMainWindow>
#include <QTimer>


namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private:
	Ui::MainWindow *ui;
	WaveField _field;
	QTimer _timer;
	QTimer _fieldTimer;
};
