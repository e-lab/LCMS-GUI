/**
 * @file
 * Contains the class that constructs the GUI and all supporting objects.
 */
#include "MainFrame.h"
#include "Logo.h"
#include "yale_icon.xpm"
//#include "purdue_icon.xpm"

#include "DeviceController.h"

#include "ControlProtocol.h"
#include "ControlBiases.h"
#include "ControlConfig.h"
#include "ControlSyncConfig.h"
#include "Logging.h"

#include "GraphicsPlot.h"

#ifdef _DEBUG
    #include <crtdbg.h>
    #define DEBUG_NEW new(_NORMAL_BLOCK ,__FILE__, __LINE__)
    #define new DEBUG_NEW
#else
    #define DEBUG_NEW new
#endif


MainFrame::MainFrame (const wxString& title, const wxSize& size) : wxFrame (NULL, wxID_ANY, title, wxDefaultPosition, size)
{
	SetIconResource();
	CreateStatusBar (1);
	SetStatusText (wxT ("LCMS"));
	CreateMenuBar();

	// Make the MainFrame panel and sizer
	wxPanel* panelMain = new wxPanel (this, wxID_ANY);
	wxSizer* sizerMain = new wxBoxSizer (wxHORIZONTAL);

	// Create display panel, must called before xemDeviceCtrl is created.
	CreateGraphics (panelMain);

	// Make the object which interfaces with the hardware, must be
	// created before CreateControls.
	xemDeviceCtrl = new DeviceController (display);

	// Create control panel
	CreateControls (panelMain);

	//((GraphicsPlot *)display)->SetMPProtocol(protocol);


	sizerMain->Add (controls, 0, wxEXPAND | wxALL, 5);
	sizerMain->Add (graphics, 1, wxEXPAND | wxALIGN_CENTER | wxALL, 20);
	panelMain->SetSizerAndFit (sizerMain);


	// Connect events to event handlers
	Connect (ON_START, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (MainFrame::OnStart));
	Connect (ON_STOP, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (MainFrame::OnStop));

	Connect (wxID_SAVE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (MainFrame::OnSave));
	Connect (SAVE_CONFIG, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (MainFrame::OnSaveProtocol));
	Connect (RELOAD_CONFIGS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (MainFrame::OnReloadConfigs));
	Connect (CONFIGURATION, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (MainFrame::Configuration));
	Connect (wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (MainFrame::OnQuit));
	Connect (wxID_ABOUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (MainFrame::OnAbout));
}

MainFrame::~MainFrame()
{
	delete controls;
	controls = (wxPanel*) NULL;

	delete graphics;
	graphics = (wxPanel*) NULL;

	delete xemDeviceCtrl;
	xemDeviceCtrl = (DeviceController*) NULL;

}

void MainFrame::OnStart (wxCommandEvent& event)
{
	for (unsigned int xx = 0; xx < controlBook.size(); xx++) {
		(controlBook.at (xx))->Start();
	}

	xemDeviceCtrl->Start();
}

void MainFrame::OnStop (wxCommandEvent& event)
{
	xemDeviceCtrl->Stop();
}

void MainFrame::OnSave (wxCommandEvent& event)
{
	xemDeviceCtrl->Stop();

	// Ask for a data filename from the user.
	wxFileDialog saveLocation (this, wxT ("Choose a save location"),
	                           wxT (""), wxT ("collected-data"),
	                           wxT ("Application Data Files (*.dat)|*.dat|All files (*.*)|*.*"),
	                           wxFD_SAVE | wxOVERWRITE_PROMPT);

	if (saveLocation.ShowModal() == wxID_CANCEL) {
		return;
	}

	display->SetCommandString (Command::SAVE_DATA, saveLocation.GetPath());
}

void MainFrame::Configuration (wxCommandEvent & event)
{
	// Ask for a config filename from the user.
	wxFileDialog dlg (this, wxT ("Please load the .bit file"),
	                  wxT (""), wxT (""),
	                  wxT ("Xilinx Configuration Files (*.bit)|*.bit|All files (*.*)|*.*"),
	                  wxOPEN | wxFILE_MUST_EXIST);

	if (dlg.ShowModal() == wxID_CANCEL) {
		return;
	}
	wxString filename = dlg.GetPath();
	xemDeviceCtrl->Initialize (filename);
}

void MainFrame::OnQuit (wxCommandEvent& event)
{
	xemDeviceCtrl->Stop();
	Close (true);
}

void MainFrame::OnAbout (wxCommandEvent& event)
{
	wxString msg;
	msg.Printf (wxT ("About.\n") wxT ("Welcome to the control program.\n"));
	wxMessageBox (msg, wxT ("About Application"), wxOK | wxICON_INFORMATION, this);
}

void MainFrame::OnSaveProtocol (wxCommandEvent& event)
{
	// Ask for a protocol configuration filename from the user.
	wxFileDialog saveLocation (this, wxT ("Choose a config file save location."),
	                           wxT (""), wxT ("LCMS-Protocol"),
	                           wxT ("LCMS Protocol Configuration Files (*.cfg)|*.cfg|All files (*.*)|*.*"),
	                           wxFD_SAVE | wxOVERWRITE_PROMPT);

	if (saveLocation.ShowModal() == wxID_CANCEL) {
		return;
	}

	protocol->SaveCurrentConfig (saveLocation.GetPath());
}

void MainFrame::OnReloadConfigs (wxCommandEvent& event)
{
	// Ask for the protocol configuration save directory from the user.
	wxDirDialog saveDirectory (this, wxT ("Select the protocol configuration directory."), wxGetCwd());

	if (saveDirectory.ShowModal() == wxID_CANCEL) {
		return;
	}

	protocol->LoadConfigFiles (saveDirectory.GetPath());
}

void MainFrame::SetIconResource()
{
	wxIcon icon (yale_icon);
	//wxIcon icon (purdue_icon);
	SetIcon (icon);
}

void MainFrame::CreateMenuBar()
{
	wxMenu *fileMenu = new wxMenu;
	wxMenu *helpMenu = new wxMenu;

	fileMenu->Append (SAVE_CONFIG, wxT ("&Save Protocol"), wxT ("Save the protocol configuration file"));
	fileMenu->Append (RELOAD_CONFIGS, wxT ("&Reload Protocols"), wxT ("Reload the protocol configuration files"));
	fileMenu->AppendSeparator();
	fileMenu->Append (wxID_SAVE, wxT ("&Save.."), wxT ("Save currently displayed data"));
	fileMenu->Append (CONFIGURATION, wxT ("&Config.."), wxT ("Load the XEM configuration bit file"));
	fileMenu->AppendSeparator();
	fileMenu->Append (wxID_EXIT, wxT ("E&xit\tAlt-X"), wxT ("Quit this program"));

	helpMenu->Append (wxID_ABOUT, wxT ("&About...\tF1"), wxT ("Show about dialog"));

	wxMenuBar *menuBar = new wxMenuBar();
	menuBar->Append (fileMenu, wxT ("&File"));
	menuBar->Append (helpMenu, wxT ("&Help"));
	SetMenuBar (menuBar);
}

void MainFrame::CreateControls (wxPanel* panelMain)
{
	controls = new wxPanel (panelMain);
	wxSizer* controlSizer = new wxBoxSizer (wxVERTICAL);


	// Tabbed command panels
	wxBookCtrl* book = new wxBookCtrl (controls, BOOKCTRL);

	protocol = new ControlProtocol (book, xemDeviceCtrl);
	book->AddPage (protocol, wxT ("Protocol"), true);
	controlBook.push_back (protocol);

	ControlBiases* biases = new ControlBiases (book, xemDeviceCtrl);
	book->AddPage (biases, wxT ("Biases"));
	controlBook.push_back (biases);

	ControlConfig* configuration = new ControlConfig (book, xemDeviceCtrl);
	book->AddPage (configuration, wxT ("Config"));
	configuration->SetGraphicsPlot ( (GraphicsPlot *) display);
	controlBook.push_back (configuration);

	ControlSyncConfig* syncConfig = new ControlSyncConfig (book, xemDeviceCtrl);
	book->AddPage (syncConfig, wxT ("Sync Conf"));
	controlBook.push_back (syncConfig);

	Logging* logging = new Logging (book);
	book->AddPage (logging, wxT ("Logging"));

	controlSizer->Add (book, 1, wxALL | wxEXPAND, 5);


	// Start and Stop buttons
	wxPanel* buttons = new wxPanel (controls);
	wxSizer* sizerButtons = new wxBoxSizer (wxHORIZONTAL);
	wxButton* startButton = new wxButton (buttons, ON_START, wxT ("Start"));
	wxButton* stopButton = new wxButton (buttons, ON_STOP, wxT ("Stop"));
	sizerButtons->Add (startButton, 0, wxRIGHT | wxBOTTOM, 25);
	sizerButtons->Add (stopButton, 0, wxLEFT | wxBOTTOM, 25);
	buttons->SetSizerAndFit (sizerButtons);

	controlSizer->Add (buttons, 0, wxTOP | wxALIGN_CENTER, 20);

	// Logo panel
	Logo* yaleLogo = new Logo (controls);
	controlSizer->Add (yaleLogo, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);

	controls->SetSizerAndFit (controlSizer);

}

void MainFrame::CreateGraphics (wxPanel* panelMain)
{
	graphics = new wxPanel (panelMain);
	wxSizer* graphicsSizer = new wxBoxSizer (wxVERTICAL);

	// Create the plot panel used to display the data
	GraphicsPlot* graphicsPanel = new GraphicsPlot (graphics);

	graphicsSizer->Add (graphicsPanel, 1, wxEXPAND | wxALIGN_CENTER);
	display = graphicsPanel;

	graphics->SetSizerAndFit (graphicsSizer);
}
