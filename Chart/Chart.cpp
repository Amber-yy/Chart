#include "Chart.h"
#include "NodeDialog.h"

#include <QTime>
#include <QTimer>
#include <QLinkedList>
#include <QComboBox>
#include <QPushButton>
#include <QPaintEvent>
#include <QPainter>

#include <algorithm>
#include <random>
#include <ctime>

#include <QDebug>

const int maxValueNum = 180;
std::random_device rd;


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
	time_t lastUpdate;
	QLinkedList<nodeData> dataSequence;
	nodeInfo(unsigned char *m)
	{
		memcpy(mac, m, sizeof(mac));
		lastUpdate = clock();
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
		lastUpdate = clock();
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
	time(reinterpret_cast<time_t *>(data->utcSecs));
	*reinterpret_cast<unsigned int *>(&data->utcUsecs) = rd()%(1000*1000);
	int a = rd() % 5;

	if (a == 0)
	{
		unsigned char mac[10] = {10,56,89,249,61,77,93,132,202,85};
		memcpy(data->macAddress, mac, 8);
	}
	else if (a == 1)
	{
		unsigned char mac[10] = { 73,35,159,136,241,78,65,26,16,94 };
		memcpy(data->macAddress, mac, 8);
	}
	else if (a == 2)
	{
		unsigned char mac[10] = { 45,82,34,189,216,217,143,19,27,35 };
		memcpy(data->macAddress, mac, 8);
	}
	else if (a == 3)
	{
		unsigned char mac[10] = { 202,115,80,214,45,98,3,177,223,165 };
		memcpy(data->macAddress, mac, 8);
	}
	else if (a == 4)
	{
		unsigned char mac[10] = { 97,54,96,71,16,102,125,105,75,45 };
		memcpy(data->macAddress, mac, 8);
	}

	for (int i = 2; i < 34; ++i)
	{
		data->data[i] = rd() % 2048 + (i-2)*2048;
	}

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

		data->nodeList->setGeometry(130, 10, 180, 25);
		data->setNode->setGeometry(20, 10, 100, 25);

		connect(data->setNode, &QPushButton::clicked, [this]() 
		{
			data->dialog->exec();
		}
		);

		connect(data->updateTimer, &QTimer::timeout, [this]()
		{
			Smart_Mesh_Data tempData = data->tempData;
			getData(&data->tempData);

			if (memcmp(&tempData, &data->tempData, sizeof(Smart_Mesh_Data)) != 0)
			{
				bool find = false;

				for (auto &t : data->allNodes)
				{
					if (memcmp(data->tempData.macAddress, t.mac, 8) == 0)
					{
						find = true;
						t.update(data->tempData.data + 2);
					}
					else
					{
						t.autoFill();
					}
				}

				if (!find)
				{
					data->allNodes.push_back(nodeInfo(data->tempData.macAddress));
					data->allNodes.last().update(data->tempData.data+2);

					QString mac;
					char code[] = {'1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
					for (int i = 0; i < 8; ++i)
					{
						mac += code[data->tempData.macAddress[i] / 16];
						mac+= code[data->tempData.macAddress[i] / 16];
						mac += ':';
					}

					mac.chop(1);

					data->nodeList->addItem(mac);
				}
			}

			time_t currentTime = clock();
			int index = 0;
			for (auto it = data->allNodes.begin(); it != data->allNodes.end(); ++index)
			{

				if (currentTime - it->lastUpdate>=60000)
				{
					auto tempIt = it++;
					data->allNodes.erase(tempIt);
					if (data->currentNode > index)
					{
						--data->currentNode;
					}
					else if (data->currentNode == index)
					{
						data->currentNode = -1;
					}
				}
				else
				{
					++it;
				}

			}

			update();
		}
		);

		void (QComboBox::*index)(int) = &QComboBox::currentIndexChanged;
		connect(data->nodeList,index, [this](int a) 
		{
			data->currentNode = a;
			update();
		}
		);

		data->updateTimer->start(500);

		setMaximumSize(940,670);
		setMinimumSize(940,670);
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
	//if (data->currentNode == -1)
	//{
	//	return;
	//}

	QPainter painter(this);
	painter.fillRect(rect(), Qt::black);
	painter.fillRect(20,50,width()-40,height()-70,Qt::white);





	
}
