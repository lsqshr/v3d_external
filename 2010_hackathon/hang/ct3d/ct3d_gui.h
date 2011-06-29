#ifndef __CT3D_GUI_H__
#define __CT3D_GUI_H__

#include <QtGui>
#include <v3d_interface.h>
#include "coseg/CT3D/cell_track_controller.h"

class Ct3dWidget : public QWidget , public CellTrackController
{
	Q_OBJECT

public:
	Ct3dWidget(V3DPluginCallback &callback, QWidget * parent) : QWidget(parent)
	{
		this->callback = &callback;
		curwin=0;

		from_images_button = new QPushButton(tr("Create From Images"));
		from_trees_button = new QPushButton(tr("Create From Trees"));
		load_result_button = new QPushButton(tr("Load Result"));
		save_result_button = new QPushButton(tr("Save Result"));
		line1 = new QFrame();
		line1->setFrameShape(QFrame::HLine);

		apply_filter_button = new QPushButton(tr("Apply Filter"));
		fine_tuning_button = new QPushButton(tr("Fine Tuning"));
		set_color_button = new QPushButton(tr("Set Color"));
		line2 = new QFrame();
		line2->setFrameShape(QFrame::HLine);

		summary_button = new QPushButton(tr("Summary"));
		speed_button = new QPushButton(tr("Speed"));
		volume_button = new QPushButton(tr("Volume"));
		deformation_button = new QPushButton(tr("Deformation"));
		trajectory_button = new QPushButton(tr("Trajectory"));
		line3 = new QFrame();
		line3->setFrameShape(QFrame::HLine);

		go_fist_button = new QPushButton(tr("First"));
		go_last_button = new QPushButton(tr("Last"));
		go_prev_button = new QPushButton(tr("Previous"));
		go_next_button = new QPushButton(tr("Next"));
		line4 = new QFrame();
		line4->setFrameShape(QFrame::HLine);

		choose_button = new QPushButton(tr("Choose"));
		delete_button = new QPushButton(tr("Delete"));
		undo_button = new QPushButton(tr("Undo Choose"));

		vbox1 = new QVBoxLayout();
		vbox1->addWidget(from_images_button);
		vbox1->addWidget(from_trees_button);
		vbox1->addWidget(load_result_button);
		vbox1->addWidget(save_result_button);
		vbox1->addWidget(line1);
		vbox1->addWidget(apply_filter_button);
		vbox1->addWidget(fine_tuning_button);
		vbox1->addWidget(set_color_button);
		vbox1->addWidget(line2);
		vbox1->addWidget(summary_button);
		vbox1->addWidget(speed_button);
		vbox1->addWidget(volume_button);
		vbox1->addWidget(deformation_button);
		vbox1->addWidget(trajectory_button);
		vbox1->addWidget(line3);
		vbox1->addWidget(go_fist_button);
		vbox1->addWidget(go_last_button);
		vbox1->addWidget(go_prev_button);
		vbox1->addWidget(go_next_button);
		vbox1->addWidget(line4);
		vbox1->addWidget(choose_button);
		vbox1->addWidget(delete_button);
		vbox1->addWidget(undo_button);

		cmds_box = new QGroupBox(tr("commands"));
		cmds_box->setLayout(vbox1);
		cmds_box->setMinimumWidth(200);

		cells_group = new QGroupBox(tr("Cells"));
		reverse_check = new QCheckBox(tr("reverse"));
		monitor_check = new QCheckBox(tr("monitor mouse"));

		gridbox = new QGridLayout();
		gridbox->addWidget(cells_group, 0, 0, 1, 2);
		gridbox->addWidget(reverse_check, 1, 0);
		gridbox->addWidget(monitor_check, 1, 1);

		cells_box = new QGroupBox(tr("cells"));
		cells_box->setLayout(gridbox);
		cells_box->setMinimumWidth(250);

		hbox = new QHBoxLayout();
		hbox->addWidget(cmds_box,3);
		hbox->addWidget(cells_box,4);

		this->setLayout(hbox);

		connect(from_images_button, SIGNAL(clicked()), this, SLOT(onCreateFromImages()));
		connect(from_trees_button, SIGNAL(clicked()), this, SLOT(onCreateFromTrees()));
		connect(load_result_button, SIGNAL(clicked()), this, SLOT(onLoadResult()));
		connect(save_result_button, SIGNAL(clicked()), this, SLOT(onSaveResult()));

		connect(apply_filter_button, SIGNAL(clicked()), this, SLOT(onApplyFilter()));
		connect(fine_tuning_button, SIGNAL(clicked()), this, SLOT(onFineTuning()));
		connect(set_color_button, SIGNAL(clicked()), this, SLOT(onSetColor()));

		connect(summary_button, SIGNAL(clicked()), this, SLOT(onSummary()));
		connect(speed_button, SIGNAL(clicked()), this, SLOT(onSpeed()));
		connect(volume_button, SIGNAL(clicked()), this, SLOT(onVolume()));
		connect(deformation_button, SIGNAL(clicked()), this, SLOT(onDeformation()));
		connect(trajectory_button, SIGNAL(clicked()), this, SLOT(onTrajectory()));

		connect(go_fist_button, SIGNAL(clicked()), this, SLOT(onFirst()));
		connect(go_last_button, SIGNAL(clicked()), this, SLOT(onLast()));
		connect(go_prev_button, SIGNAL(clicked()), this, SLOT(onPrevious()));
		connect(go_next_button, SIGNAL(clicked()), this, SLOT(onNext()));

		connect(choose_button, SIGNAL(clicked()), this, SLOT(onChoose()));
		connect(delete_button, SIGNAL(clicked()), this, SLOT(onDelete()));
		connect(undo_button, SIGNAL(clicked()), this, SLOT(onUndo()));
	}

	~Ct3dWidget(){}
public:
	void updateWidgets()
	{
		if(curwin==0) curwin = callback->newImageWindow();

		V3DLONG sz0 = this->getWidth();
		V3DLONG sz1 = this->getHeight();
		V3DLONG sz2 = this->getDepth();
		V3DLONG sz3 = 3; //rgb

		Image4DSimple * p4DImage = new Image4DSimple();
		unsigned char * inimg1d = this->getTexData();

		p4DImage->setData(inimg1d, sz0, sz1, sz2, sz3, V3D_UINT8);

		callback->setImage(curwin, p4DImage);
		callback->setImageName(curwin, tr("Time : ").arg(this->currentTime() +1));
		callback->updateImageWindow(curwin);
	}

public slots:
	void onCreateFromImages()
	{
		QStringList filelist = QFileDialog::getOpenFileNames(
				this,
				"Select one or more files to open",
				"",
				"Images (*.tif *.tiff *.raw)");
		if(filelist.empty())
		{
			QMessageBox::information(0,"create from images","No image is loaded!");
			return;
		}
		bool ok;
		int min_size = 0;
		int max_size = 100000;
		int single_size = 10;
		min_size = QInputDialog::getInt(0, QObject::tr("get parameters"), 
				QObject::tr("min_size :"),
				1000, 1, 1000000000, 1, &ok);
		if(ok)
		{
			max_size = QInputDialog::getInt(0, QObject::tr("get parameters"), 
					QObject::tr("max_size :"),
					1000000, 1, 1000000000, 1, &ok);
			if(ok)
			{
				single_size = QInputDialog::getInt(0, QObject::tr("get parameters"), 
						QObject::tr("single_size :"),
						100, 1, 1000000000, 1, &ok);
				if(!ok)
				{
					QMessageBox::information(0,"get parameters","single size error");
					return;
				}
			}
			else
			{
				QMessageBox::information(0,"get parameters","max size error");
				return;
			}
		}
		else
		{
			QMessageBox::information(0,"get parameters","min size error");
			return;
		}
		cout<<"min_size : "<<min_size<<endl;
		cout<<"max_size : "<<max_size<<endl;
		cout<<"single_size : "<<single_size<<endl;

		vector<string> image_files;
		QStringList::iterator it = filelist.begin();
		while(it != filelist.end())
		{
			cout<<(*it).toStdString().c_str()<<endl;
			image_files.push_back((*it).toStdString());
			it++;
		}
		createCellTrack(image_files, min_size, max_size, single_size);
	}

	void onCreateFromTrees()
	{
		QStringList filelist = QFileDialog::getOpenFileNames(
				this,
				"Select one or more files to open",
				"",
				"Images (*.tree)");
		if(filelist.empty())
		{
			QMessageBox::information(0,"create from trees","No tree is loaded!");
			return;
		}
		vector<string> tree_files;
		QStringList::iterator it = filelist.begin();
		while(it != filelist.end())
		{
			cout<<(*it).toStdString().c_str()<<endl;
			tree_files.push_back((*it).toStdString());
			it++;
		}
		createCellTrack(tree_files);
	}

	void onLoadResult()
	{
		QStringList filelist = QFileDialog::getOpenFileNames(
				this,
				"Select one or more files to open",
				"",
				"Images (*.tif *.tiff *.raw)");
		if(filelist.empty())
		{
			QMessageBox::information(0,"create from images","No image is loaded!");
			return;
		}
		vector<string> image_results;
		QStringList::iterator it = filelist.begin();
		while(it != filelist.end())
		{
			cout<<(*it).toStdString().c_str()<<endl;
			image_results.push_back((*it).toStdString());
			it++;
		}
		vector<string> tree_files;
		loadCellTrack(image_results, tree_files);
	}

	void onSaveResult()
	{
		if(celltrack == NULL || celltrack->frameNum() == 0 )    return;
		QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),"",QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
		if(dir == "") return;
		else celltrack->exportImages("", (char*)dir.toStdString().c_str());
		QMessageBox::information(this,"Save Frames", "Successfully Saved!");
	}

	void onApplyFilter()
	{
	}

	void onFineTuning()
	{
	}

	void onSetColor()
	{
	}

	void onSummary()
	{
	}

	void onSpeed()
	{
	}

	void onVolume()
	{
	}

	void onDeformation()
	{
	}

	void onTrajectory()
	{
	}

	void onFirst()
	{
		if(celltrack == NULL || celltrack->frameNum() == 0 )	return;
		setFirst();
		updateWidgets();
	}

	void onLast()
	{
		if(celltrack == NULL || celltrack->frameNum() == 0 )	return;
		setLast();
		updateWidgets();
	}

	void onPrevious()
	{
		if(celltrack == NULL || celltrack->frameNum() == 0 )	return;
		setPrev();
		updateWidgets();
	}

	void onNext()
	{
		if(celltrack == NULL || celltrack->frameNum() == 0 )	return;
		setNext();
		updateWidgets();
	}

	void onChoose()
	{
	}

	void onDelete()
	{
	}

	void onUndo()
	{
	}

public:
	QPushButton * from_images_button;
	QPushButton * from_trees_button;
	QPushButton * load_result_button;
	QPushButton * save_result_button;
	QFrame * line1;

	QPushButton * apply_filter_button;
	QPushButton * fine_tuning_button;
	QPushButton * set_color_button;
	QFrame * line2;

	QPushButton * summary_button;
	QPushButton * speed_button;
	QPushButton * volume_button;
	QPushButton * deformation_button;
	QPushButton * trajectory_button;
	QFrame * line3;

	QPushButton * go_fist_button;
	QPushButton * go_last_button;
	QPushButton * go_prev_button;
	QPushButton * go_next_button;
	QFrame * line4;

	QPushButton * choose_button;
	QPushButton * delete_button;
	QPushButton * undo_button;

	QVBoxLayout * vbox1;
	QGroupBox * cmds_box;

	QGroupBox * cells_group;
	QCheckBox * reverse_check;
	QCheckBox * monitor_check;

	QGridLayout * gridbox;
	QGroupBox * cells_box;

	QHBoxLayout * hbox;

	V3DPluginCallback * callback;
	v3dhandle curwin;

};

#endif
