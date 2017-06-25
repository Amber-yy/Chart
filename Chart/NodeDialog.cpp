#include "NodeDialog.h"
#include "Chart.h"
#include <QPushbutton>
#include <QCheckBox>

struct NodeDialog::nodeDialogData
{
	QCheckBox *box[32];
	QPushButton *submit;
	Chart *parent;
};

NodeDialog::NodeDialog(QWidget *parent):QDialog(parent)
{
	data = std::make_unique<nodeDialogData>();
	data->parent = dynamic_cast<Chart *>(parent);

	setWindowTitle(u8"要显示哪些路的数据");
	
	for (int i = 0; i < 32; ++i)
	{
		data->box[i] = new QCheckBox(u8"第"+QString::number(i+1)+u8"路",this);
		data->box[i]->setGeometry(10+(i%8)*60,10+(i/8)*30,60,30);
	}
	 
	for (int i = 0; i < 8; ++i)
	{
		data->box[i]->setChecked(true);
	}

	setMaximumSize(500, 18);
	setMinimumSize(500, 180);

	data->submit = new QPushButton(u8"确定",this);
	
	data->submit->setGeometry(215,150,70,25);

	connect(data->submit, &QPushButton::clicked, [this]() 
	{
		bool v[32];
		for (int i = 0; i < 32; ++i)
		{
			if (data->box[i]->isChecked())
			{
				v[i] = true;
			}
			else
			{
				v[i] = false;
			}
		}

		data->parent->setVisible(v);
		close();
	}
	);

}

NodeDialog::~NodeDialog()
{
}
