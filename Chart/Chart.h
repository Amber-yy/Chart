#ifndef CHART_H
#define CHART_H

#include <QWidget>
#include <memory>

class Chart : public QWidget
{
	Q_OBJECT
public:
	Chart(QWidget *parent = 0);
	~Chart();
	void setNodeVisible(bool *v);
	bool *getNodeVisible();
protected:
	virtual void paintEvent(QPaintEvent *e)override;
protected:
	struct ChartData;
	std::unique_ptr<ChartData> data;
};

#endif // CHART_H
