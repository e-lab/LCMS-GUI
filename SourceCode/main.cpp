/**
 * @file
 * Contains the initializing class.
 *
 * @mainpage Control Utility
 *
 * This application is a template for comunicating with a OpalKelly project
 * board. Data collected can be displayed as an image or on as a plot.
 *
 * @section overview Overview
 * The applications initialization follows closely that of a standard wxWidgets
 * app with the first class ELabApp being derived from wxApp and written in the
 * main.cpp file.  This first object creates and shows the application window,
 * within which is contained the controls and graphic displays, all of which
 * are created by the MainFrame class and the other classes it initializes.
 *
 * As much as possible the functionality of the app has been compartmentalized,
 * with each class trying to contain only one logical component.  This has been
 * in the hope that the code can be taken and easily modified to be used in
 * other projects.
 */
#include "main.h"
#include "MainFrame.h"


IMPLEMENT_APP (ELabApp)


bool ELabApp::OnInit()
{
	// Create the main application window
	MainFrame *frame = new MainFrame (wxT ("Control Utility"), wxSize (910,700));

	frame->Show (true);
	return true;
}

