#pragma once

#include <QDialog>
#include <memory>

class NodeDialog :public QDialog
{
public:
	NodeDialog(QWidget *parent);
	~NodeDialog();
protected:
	struct nodeDialogData;
	std::unique_ptr<nodeDialogData> data;
};

