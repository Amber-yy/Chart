#ifndef CHART_H
#define CHART_H

#include <QtWidgets/QWidget>
#include "ui_Chart.h"

class Chart : public QWidget
{
	Q_OBJECT

public:
	Chart(QWidget *parent = 0);
	~Chart();

private:
	Ui::ChartClass ui;
};

#endif // CHART_H
