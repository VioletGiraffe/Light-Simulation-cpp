#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>
#include <QImage>

class View final : public QWidget
{
public:
	View(const WaveField& field, QWidget* parent = nullptr);

	void paintEvent(QPaintEvent*) override;

private:
	const WaveField& _field; // TODO: remove tight coupling
};


View::View(const WaveField &field, QWidget *parent) :
	QWidget{parent},
	_field{field}
{
}

void View::paintEvent(QPaintEvent *)
{
	QImage canvas(_field.width, _field.height, QImage::Format_RGB32);
	auto* scanLine = canvas.scanLine(0);

	for (size_t i = 0; i < _field.width; i++) {
		for (size_t j = 0; j < _field.height; j++) {
			const size_t dataIndex = (i + j * _field.width) * 4;
			const auto colorValue = _field.pixelColor(i, j);
			for (int k = 0; k < 3; k++)
			{
				scanLine[dataIndex + k] = colorValue[k];
			}

			scanLine[dataIndex + 3] = 255;
		}
	}


	QPainter p(this);
	p.drawImage(QPoint{0, 0}, canvas);
}

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	auto* view = new View(_field, this);
	setCentralWidget(view);

	connect(&_timer, &QTimer::timeout, view, (void (QWidget::*)())&View::update);
	_timer.start(100);

	connect(&_fieldTimer, &QTimer::timeout, this, [this] {
		_field.update(0.01f);
	});
	_fieldTimer.start(1);

}

MainWindow::~MainWindow()
{
	delete ui;
}
