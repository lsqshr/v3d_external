/* lobeseg_func.cpp
 * This is a lobeseg plugin
 * 2011-06-20 : by Hang Xiao
 */

#include "stackutil.h"
#include <v3d_interface.h>
#include "v3d_message.h"
#include "lobeseg_func.h"
#include "lobeseg_gui.h"

#include "../lobeseg.h"
#include "../../../v3d_main/worm_straighten_c/bdb_minus.h"

#include <unistd.h>
extern char *optarg;
extern int optind, opterr;

int split(char *original, char ** &splits);

const QString title = QObject::tr("Lobeseg Plugin");
int lobeseg_two_sides(V3DPluginCallback2 &callback, QWidget *parent)
{
	v3dhandleList win_list = callback.getImageWindowList();

	if(win_list.size()<1)
	{
		QMessageBox::information(0, title, QObject::tr("No image is open."));
		return -1;
	}

	TwoSidesDialog dialog(callback, parent);
	if(dialog.exec() == QDialog::Rejected) return -1;
	dialog.update();
	int i = dialog.i;
	int c = dialog.channel;
	double alpha = dialog.alpha;
	double beta = dialog.beta;
	double gamma = dialog.gamma;
	int nloops = dialog.nloops;
	int radius = dialog.radius;

	Image4DSimple * image = callback.getImage(win_list[i]);
	if(image->getCDim() <= c) {v3d_msg(QObject::tr("The channel isn't existed.")); return -1;}
	unsigned char * inimg1d = image->getRawData();
	V3DLONG sz[4];
	sz[0] = image->getXDim();
	sz[1] = image->getYDim();
	sz[2] = image->getZDim();
	sz[3] = image->getCDim();
	unsigned char * outimg1d = new unsigned char[sz[0] * sz[1] * sz[2] * (sz[3] + 1)];
	for(V3DLONG i = 0 ; i < sz[0] * sz[1] * sz[2] * sz[3]; i++) outimg1d[i] = inimg1d[i];
	for(V3DLONG i =  sz[0] * sz[1] * sz[2] * sz[3] ; i < sz[0] * sz[1] * sz[2] * (sz[3] + 1); i++) outimg1d[i] = 0;

	int in_channel_no = c;
	int out_channel_no = 1;

	BDB_Minus_ConfigParameter mypara;
	mypara.f_image = alpha;
	mypara.f_smooth = beta;
	mypara.f_length = gamma;
	mypara.nloops = nloops;
	mypara.radius = radius;
	mypara.radius_x = radius;
	mypara.radius_y = radius;
	mypara.TH = 0.1;

	if(!do_lobeseg_bdbminus(inimg1d, sz, outimg1d, in_channel_no, out_channel_no, mypara))
	{
		v3d_msg("lobeseg two sides error!");
		return -1;
	}

	Image4DSimple * p4DImage = new Image4DSimple();
	p4DImage->setData((unsigned char*)outimg1d, sz[0], sz[1], sz[2], sz[3] + 1, image->getDatatype());

	v3dhandle newwin;
	if(dialog.is_newwin)
		newwin = callback.newImageWindow();
	else
		newwin = callback.currentImageWindow();

	callback.setImage(newwin, p4DImage);
	callback.setImageName(newwin, QObject::tr("lobeseg_two_sides"));
	callback.updateImageWindow(newwin);

	return 1;
}

int lobeseg_one_side_only(V3DPluginCallback2 &callback, QWidget *parent)
{
	v3dhandleList win_list = callback.getImageWindowList();

	if(win_list.size()<1)
	{
		QMessageBox::information(0, title, QObject::tr("No image is open."));
		return -1;
	}

	OneSideOnlyDialog dialog(callback, parent);
	if(dialog.exec() == QDialog::Rejected) return -1;
	dialog.update();
	int i = dialog.i;
	int c = dialog.channel;
	double alpha = dialog.alpha;
	double beta = dialog.beta;
	double gamma = dialog.gamma;
	int nloops = dialog.nloops;
	int radius = dialog.radius;
	int x0 = dialog.x0;
	int y0 = dialog.y0;
	int x1 = dialog.x1;
	int y1 = dialog.y1;
	int keep_which = dialog.keep_which;
	int nctrls = dialog.nctrls;
	bool is_surf = dialog.is_surf;

	Image4DSimple * image = callback.getImage(win_list[i]);
	if(image->getCDim() <= c) {v3d_msg(QObject::tr("The channel isn't existed.")); return -1;}
	unsigned char * inimg1d = image->getRawData();
	V3DLONG sz[4];
	sz[0] = image->getXDim();
	sz[1] = image->getYDim();
	sz[2] = image->getZDim();
	sz[3] = image->getCDim();
	unsigned char * outimg1d = new unsigned char[sz[0] * sz[1] * sz[2] * (sz[3] + 1)];
	for(V3DLONG i = 0 ; i < sz[0] * sz[1] * sz[2] * sz[3]; i++) outimg1d[i] = inimg1d[i];
	for(V3DLONG i =  sz[0] * sz[1] * sz[2] * sz[3] ; i < sz[0] * sz[1] * sz[2] * (sz[3] + 1); i++) outimg1d[i] = 0;
	int in_channel_no = c;
	int out_channel_no = 1;

	BDB_Minus_ConfigParameter mypara;
	mypara.f_image = alpha;
	mypara.f_smooth = beta;
	mypara.f_length = gamma;
	mypara.nloops = nloops;
	mypara.radius = radius;
	mypara.radius_x = radius;
	mypara.radius_y = radius;
	mypara.TH = 0.1;

	if(!do_lobeseg_bdbminus_onesideonly(inimg1d, sz, outimg1d, in_channel_no, out_channel_no, mypara, x0, y0, x1, y1, keep_which, nctrls, is_surf))
	{
		v3d_msg("lobeseg two sides error!");
		return -1;
	}

	Image4DSimple * p4DImage = new Image4DSimple();
	p4DImage->setData((unsigned char*)outimg1d, sz[0], sz[1], sz[2], sz[3] + 1, image->getDatatype());

	v3dhandle newwin;
	if(dialog.is_newwin)
		newwin = callback.newImageWindow();
	else
		newwin = callback.currentImageWindow();

	callback.setImage(newwin, p4DImage);
	callback.setImageName(newwin, QObject::tr("lobeseg_one_side_only"));
	callback.updateImageWindow(newwin);

	return 1;
}

bool lobeseg(const V3DPluginArgList & input, V3DPluginArgList & output)
{
	char * err_str = new char[500];
	strcpy(err_str, "");

	char infile[500];
	char outfile[500];
	int in_channel_no = 0;
	int out_channel_no = 1;
	double alpha = 1.0;
	double beta  = 0.5;
	double gamma = 0.5;
	int nloops = 500;
	int radius = 20;
	int x0 = 30;
	int y0 = 0;
	int x1 = 100;
	int y1 = 50;
	int keep_which = 0; //left
	int nctrls = 20;
	bool is_surf = false;

	bool single_side = false;

	V3DPluginArgItem item;
	item.type = "error string";
	item.p = err_str;
	output<<item;

	char ** argv;
	int argc = split((char *) input.at(0).p , argv);
	
	char optstring[] = "sSi:o:c:A:B:G:n:p:k:N:";
	int c;
	while((c = getopt(argc, argv, optstring)) != -1)
	{
		switch(c)
		{
			case 's' : single_side = true; break;
			case 'S' : is_surf = true; break;
			case 'p' : 
					   if (strcmp (optarg, "(null)") == 0 || optarg[0] == '-')
					   {
						   strcpy(err_str, "Found illegal or NULL parameter for the option -p");
						   return false;
					   }
					   else
					   {
						   string pos(optarg);
						   if(pos.find('+') == string::npos || pos.find_first_of('+') != pos.find_last_of('+') || pos.find('x') == string::npos || pos.find_first_of('x') == pos.find_last_of('x'))
						   {
							   strcpy(err_str, "Found illegal or NULL parameter for the option -p");
							   return false;
						   }
						   else
						   {
							   string str_x0 = pos.substr(0, pos.find_first_of('x'));
							   string str_y0 = pos.substr(pos.find_first_of('x') + 1, pos.find_first_of('+') - pos.find_first_of('x') -1);
							   string str_x1 = pos.substr(pos.find_first_of('+') + 1, pos.find_last_of('x') - pos.find_first_of('+') - 1);
							   string str_y1 = pos.substr(pos.find_last_of('x') + 1, pos.length() - pos.find_last_of('x') -1);
							   x0 = atoi(str_x0.c_str());
							   y0 = atoi(str_y0.c_str());
							   x1 = atoi(str_x1.c_str());
							   y1 = atoi(str_y1.c_str());
							   if(x0 < 0 || x0 > 100 || y0 < 0 || y0 > 100 || x1 < 0 || x1 > 100 || y1 < 0 || y1 > 100)
							   {
								   strcpy(err_str, "Found illegal or NULL parameter for the option -p");
								   return false;
							   }
						   }
					   }
					   break;
			case 'k':
					   if (strcmp (optarg, "(null)") == 0 || optarg[0] == '-')
					   {
						   strcpy(err_str, "Found illegal or NULL parameter for the option -k");
						   return false;
					   }
					   else
					   {
						   if(strcmp(optarg,"left") == 0 || strcmp(optarg, "Left") == 0) keep_which = 0;
						   else if(strcmp(optarg,"right") == 0 || strcmp(optarg, "Right") == 0) keep_which = 1;
						   else {
							   strcpy(err_str, "Found illegal or NULL parameter for the option -k");
							   return false;
						   };
					   }
					   break;
			case 'N' : 
					   if (strcmp (optarg, "(null)") == 0 || optarg[0] == '-')
					   {
						   strcpy(err_str, "Found illegal or NULL parameter for the option -N");
						   return false;
					   }
					   else
					   {
						   //cout<<" number of control points : "<<optarg<<endl;
						   nctrls = atoi(optarg);
						   if(nctrls<=0)
						   {
							   strcpy(err_str, "Found illegal or NULL parameter for the option -N");
							   return false;
						   }
					   }
					   break;
			case 'i':
					   if (strcmp (optarg, "(null)") == 0 || optarg[0] == '-')
					   {
						   strcpy(err_str, "Found illegal or NULL parameter for the option -i");
						   return false;
					   }
					   strcpy(infile , optarg);
					   break;
			case 'o' :
					   if (strcmp (optarg, "(null)") == 0 || optarg[0] == '-')
					   {
						   strcpy(err_str, "Found illegal or NULL parameter for the option -o");
						   return false;
					   }
					   strcpy(outfile, optarg);
					   break;
			case 'c' :
					   if (strcmp (optarg, "(null)") == 0 || optarg[0] == '-')
					   {
						   strcpy(err_str, "Found illegal or NULL parameter for the option -c");
						   return false;
					   }
					   in_channel_no = atoi (optarg);
					   if (in_channel_no < 0)
					   {
						   strcpy(err_str, "Illegal channel. Not found! It must be >=0.");
						   return false;
					   }
					   break;
			case 'A' :
					   if (strcmp (optarg, "(null)") == 0 || optarg[0] == '-')
					   {
						   strcpy(err_str, "Found illegal or NULL parameter for the option -A");
						   return false;
					   }
					   alpha = atof (optarg);
					   if (alpha<0)
					   {
						   strcpy(err_str,"alpha must not be less than than 0.");
						   return false;
					   }
					   break;
			case 'B' :
					   if (strcmp (optarg, "(null)") == 0 || optarg[0] == '-')
					   {
						   strcpy(err_str, "Found illegal or NULL parameter for the option -B");
						   return false;
					   }
					   beta = atof (optarg);
					   if (beta<0)
					   {
						   strcpy(err_str,"beta must not be less than than 0.");
						   return false;
					   }
					   break;
			case 'G' :
					   if (strcmp (optarg, "(null)") == 0 || optarg[0] == '-')
					   {
						   strcpy(err_str, "Found illegal or NULL parameter for the option -G");
						   return false;
					   }
					   gamma = atof (optarg);
					   if (gamma<0)
					   {
						   strcpy(err_str,"gamma must not be less than than 0.");
						   return false;
					   }
					   break;
			case 'n' :
					   if (strcmp (optarg, "(null)") == 0 || optarg[0] == '-')
					   {
						   strcpy(err_str, "Found illegal or NULL parameter for the option -n");
						   return false;
					   }
					   nloops = atoi (optarg);
					   if (nloops < 1)
					   {
						   strcpy(err_str, "The number of loop must be >= 1.");
						   return false;
					   }
					   break;
			case '?' :
					   sprintf(err_str, "Unknow option '-%c' or incomplete argument lists.", optopt);
						return false;
		}
	}

	BDB_Minus_ConfigParameter mypara;
	mypara.f_image = alpha;
	mypara.f_smooth = beta;
	mypara.f_length = gamma;
	mypara.nloops = nloops;
	mypara.radius = radius;
	mypara.radius_x = radius;
	mypara.radius_y = radius;
	mypara.TH = 0.1;

	unsigned char * inimg1d = 0;
	V3DLONG *sz=0;
	int datatype;
	loadImage(infile, inimg1d, sz, datatype);

	unsigned char * outimg1d = new unsigned char[sz[0] * sz[1] * sz[2] * (sz[3] + 1)];
	for(V3DLONG i = 0 ; i < sz[0] * sz[1] * sz[2] * sz[3]; i++) outimg1d[i] = inimg1d[i];
	for(V3DLONG i =  sz[0] * sz[1] * sz[2] * sz[3] ; i < sz[0] * sz[1] * sz[2] * (sz[3] + 1); i++) outimg1d[i] = 0;

	if(!single_side)
	{
		if (!do_lobeseg_bdbminus(inimg1d, sz, outimg1d, in_channel_no, out_channel_no, mypara))
		{
			strcpy(err_str, "Fail to do two sides lobe segmentation.");
			return false;
		}
	}
	else
	{
		if (!do_lobeseg_bdbminus_onesideonly(inimg1d, sz, outimg1d, in_channel_no, out_channel_no, mypara, x0, y0, x1, y1, keep_which,nctrls, is_surf))
		{
			strcpy(err_str, "Fail to do one side lobe segmentation.");
			return false;
		}
	}

	saveImage(outfile, outimg1d,sz, datatype);

	return true;
}

int split(char *original, char ** &splits)
{
	int i = 0;
	int j = 0;
	char* str = new char[strlen(original)];
	while (original[i] != '\0')
	{
		if(original[i] == ' ' || original[i] == '\t')
		{
			while (original[i] == ' ' || original[i] == '\t')
			{

				str[i] = '\0';
				i++;

			}
			if(original[i] != '\0')
			{
				str[i] = original[i];
				splits[j++] = &(str[i]);
			}
		}
		else
		{
			if(i == 0) splits[j++] = &(str[0]);
			str[i] = original[i];
		}
		i++;
	}
	str[i] = '\0';
	return j;
}
