#include "Chart.h"
#include "NodeDialog.h"

#include <QTime>
#include <QTimer>
#include <QLinkedList>
#include <QComboBox>
#include <QPushButton>
#include <QPaintEvent>
#include <QPainter>

const int maxValueNum = 20;

struct Smart_Mesh_Data
{
	unsigned char    utcSecs[8]; 
	unsigned int   utcUsecs; 
	unsigned char    macAddress[8]; 
	unsigned short   srcPort; 
	unsigned short   dstPort; 
	unsigned char    dataLen; 
	unsigned short    data[34]; 
};

struct nodeData
{
	unsigned short data[32];
};

struct nodeInfo
{
	unsigned char mac[8];
	QTime lastUpdate;
	QLinkedList<nodeData> dataSequence;
	nodeInfo(unsigned char *m)
	{
		memcpy(mac, m, sizeof(mac));
		lastUpdate = QTime::currentTime();
	}

	/*更新数据*/
	void update(unsigned short *d)
	{
		nodeData temp;
		memcpy(&temp.data,d,sizeof(temp.data));
		dataSequence.push_back(temp);
		if (dataSequence.size() > maxValueNum)
		{
			dataSequence.pop_front();
		}
		lastUpdate = QTime::currentTime();
	}

	/*若没有接受到数据，则用之前的数据自动填充*/
	void autoFill()
	{
		nodeData temp=dataSequence.last();
		dataSequence.push_back(temp);
		if (dataSequence.size() > maxValueNum)
		{
			dataSequence.pop_front();
		}
	}

};

struct Chart::ChartData
{
	QComboBox *nodeList;
	QPushButton *setNode;
	NodeDialog *dialog;
	QTimer *updateTimer;
	QLinkedList<nodeInfo> allNodes;
	Smart_Mesh_Data tempData;
	bool visible[32];
	int currentNode;
};

void getData(Smart_Mesh_Data *data)
{

}

Chart::Chart(QWidget *parent): QWidget(parent)
{
	try
	{
		data = std::make_unique<ChartData>();

		data->currentNode = -1;
		memset(data->visible, 0, 8);
		memset(data->visible + 8, 1, 24);

		data->nodeList = new QComboBox(this);
		data->setNode = new QPushButton(u8"显示设置",this);
		data->dialog = new NodeDialog(this);
		data->updateTimer = new QTimer(this);

		data->nodeList->setGeometry(0, 0, 100, 25);
		data->setNode->setGeometry(110, 0, 100, 25);

		connect(data->setNode, &QPushButton::clicked, [this]() 
		{
			data->dialog->exec();
		}
		);

		connect(data->updateTimer, &QTimer::timeout, [this]()
		{
			getData(&data->tempData);
			update();
		}
		);

		void (QComboBox::*index)(int) = &QComboBox::currentIndexChanged;
		connect(data->nodeList,index, [this](int a) 
		{
			data->currentNode = a;
		}
		);

		setMaximumSize(800,600);
		setMinimumSize(800,600);
	}
	catch (std::bad_alloc &)
	{
		abort();
	}
}

Chart::~Chart()
{

}

void Chart::setNodeVisible(bool * v)
{
	memcpy(data->visible,v,32);
}

bool * Chart::getNodeVisible()
{
	return data->visible;
}

void Chart::paintEvent(QPaintEvent * e)
{
	QPainter painter(this);
	
	





}
