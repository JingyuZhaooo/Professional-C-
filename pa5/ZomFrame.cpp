//
//  ZomFrame.cpp
//  zombiewx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#include "ZomFrame.h"
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/log.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/filedlg.h>
#include "ZomDrawPanel.h"
#include <wx/wfstream.h>

enum
{
	ID_SImSTART = 1000,
	ID_TURN_TIMER,
	ID_LOADZOMBIE = 2000,
	ID_LOADSURVIVOR = 3000,
	ID_RANDOMIZE = 4000,
	ID_RESET = 5000,
};

wxBEGIN_EVENT_TABLE(ZomFrame, wxFrame)
	EVT_MENU(wxID_EXIT, ZomFrame::OnExit)
	EVT_MENU(wxID_NEW, ZomFrame::OnNew)
	EVT_MENU(ID_SImSTART, ZomFrame::OnSimStart)
	EVT_MENU(ID_LOADZOMBIE, ZomFrame::OnLoadZombie)
	EVT_MENU(ID_LOADSURVIVOR, ZomFrame::OnLoadSurvivor)
	EVT_MENU(ID_RANDOMIZE, ZomFrame::OnRandomize)
	EVT_MENU(ID_RESET, ZomFrame::OnReset)
	EVT_TIMER(ID_TURN_TIMER, ZomFrame::OnTurnTimer)
wxEND_EVENT_TABLE()

ZomFrame::ZomFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame(NULL, wxID_ANY, title, pos, size)
, mIsActive(false)
{
	// File menu
	wxMenu* menuFile = new wxMenu;
	menuFile->Append(wxID_NEW);
	menuFile->Append(wxID_EXIT);
	
	// Simulation menu
	mSimMenu = new wxMenu;
	mSimMenu->Append(ID_SImSTART, "Start/stop\tSpace", "Start or stop the simulation");
	
	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(mSimMenu, "&Simulation");
	mSimMenu->Append(ID_LOADZOMBIE, "Load Zombies");
	mSimMenu->Append(ID_LOADSURVIVOR, "Load Survivors");
	mSimMenu->Append(ID_RANDOMIZE, "Randomize");
	mSimMenu->Append(ID_RESET, "Reset");
	SetMenuBar(menuBar);
	CreateStatusBar();
	
	// Prepare the draw panel and show this frame
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	mPanel = new ZomDrawPanel(this);
	sizer->Add(mPanel, 1, wxEXPAND);
	
	SetSizer(sizer);
	SetAutoLayout(true);
	Show(true);
	
	mTurnTimer = new wxTimer(this, ID_TURN_TIMER);

	// TEMP CODE: Initialize zombie test machine
    //zombieMachine.LoadMachine(std::string(""));
	//zombieMachine.BindState(zombieTestState);
	// END TEMP CODE

	mZombieMachine = std::make_shared<Machine<ZombieTraits>>();
	mSurvivorMachine = std::make_shared<Machine<HumanTraits>>();
	mWorld = std::make_shared<World>();
	//mPanel = new ZomDrawPanel(this);
	mPanel->SetWorld(mWorld);

	mSimMenu->Enable(ID_SImSTART, false);
	mSimMenu->Enable(ID_LOADZOMBIE, true);
	mSimMenu->Enable(ID_LOADSURVIVOR, true);
	mSimMenu->Enable(ID_RESET, false);
	mSimMenu->Enable(ID_RANDOMIZE, false);
	// Initialize all tiles to be empty
	emptyTiles.resize(20);
	for (int i = 0; i < 20; i++)
	{
		emptyTiles[i].resize(20);
		for (int j = 0; j < 20; j++)
		{
			emptyTiles[i][j] = true;	
		}
	}
	mEnableFlag1 = false;
	mEnableFlag2 = false;
}

ZomFrame::~ZomFrame()
{
	delete mTurnTimer;
}

void ZomFrame::OnExit(wxCommandEvent& event)
{
	Close(true);
}

void ZomFrame::OnNew(wxCommandEvent& event)
{
	// TODO: Add code for File>New
	mWorld->Reset();
	mZombieMachine.reset();
	mZombieMachine = std::make_shared<Machine<ZombieTraits>>();
	mSurvivorMachine.reset();
	mSurvivorMachine = std::make_shared<Machine<HumanTraits>>();
	mPanel->PaintNow();

	mTurnTimer->Stop();
	mSimMenu->Enable(ID_SImSTART, false);
	mSimMenu->Enable(ID_LOADZOMBIE, true);
	mSimMenu->Enable(ID_LOADSURVIVOR, true);
	mSimMenu->Enable(ID_RESET, false);
	mSimMenu->Enable(ID_RANDOMIZE, false);
	// Mark all tiles to be empty
	for (int i = 0; i < 20; i++)
	{
		emptyTiles[i].resize(20);
		for (int j = 0; j < 20; j++)
		{
			emptyTiles[i][j] = false;
		}
	}
	mEnableFlag1 = false;
	mEnableFlag2 = false;
}

void ZomFrame::OnSimStart(wxCommandEvent & event)
{
	if (!mIsActive)
	{
		// Add timer to run once per second
		mTurnTimer->Start(1000);
		mIsActive = true;
	}
	else
	{
		mTurnTimer->Stop();
		mIsActive = false;
	}
}

void ZomFrame::OnTurnTimer(wxTimerEvent& event)
{
	// TEMP CODE: Take turn for zombie machine
	zombieMachine.TakeTurn(zombieTestState);
	// END TEMP CODE
}

void ZomFrame::OnLoadZombie(wxCommandEvent & event)
{
	wxFileDialog openFileDialog(this, _("Open Zombie file"), "./data", "", "ZOM files (*.zom)|*.zom", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;     // the user changed idea...

					// proceed loading the file chosen by the user;
					// this can be done with e.g. wxWidgets input streams:
	wxFileInputStream input_stream(openFileDialog.GetPath());
	if (!input_stream.IsOk())
	{
		wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
		return;
	}
	mZombieFileName = openFileDialog.GetPath();
	mWorld->SetZombieFileName(mZombieFileName);
	mZombieMachine->SetWorld(mWorld);
	mZombieMachine->LoadMachine(mZombieFileName);

	mPanel->PaintNow();
	mEnableFlag1 = true;	// set the flag to true
	if (mEnableFlag2)		// if there is a survivor file already loaded, enable the 3 options on menu
	{
		mSimMenu->Enable(ID_RANDOMIZE, true);
	}
}

void ZomFrame::OnLoadSurvivor(wxCommandEvent & event)
{
	wxFileDialog openFileDialog(this, _("Open Zombie file"), "./data", "", "ZOM files (*.zom)|*.zom", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;     // the user changed idea...

					// proceed loading the file chosen by the user;
					// this can be done with e.g. wxWidgets input streams:
	wxFileInputStream input_stream(openFileDialog.GetPath());
	if (!input_stream.IsOk())
	{
		wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
		return;
	}
	mSurvivorFileName = openFileDialog.GetPath();
	mWorld->SetSurvivorFileName(mSurvivorFileName);
	mSurvivorMachine->SetWorld(mWorld);
	mSurvivorMachine->LoadMachine(mSurvivorFileName);
	mPanel->PaintNow();

	mEnableFlag2 = true;	// set the flag to true
	if (mEnableFlag1)		// if there is a zombie file already loaded, enable the 3 options on menu
	{
		mSimMenu->Enable(ID_RANDOMIZE, true);
	}
}

void ZomFrame::OnRandomize(wxCommandEvent & event)
{
	for (unsigned i = 0; i < 10; i++)
	{
		auto newSurvivor = std::make_shared<MachineState>();
		// randomly set the direction
		int direction = rand() % 4;
		switch (direction)
		{
		case 0:
			newSurvivor->mFacing = MachineState::UP;
			break;
		case 1:
			newSurvivor->mFacing = MachineState::RIGHT;
			break;
		case 2:
			newSurvivor->mFacing = MachineState::DOWN;
			break;
		case 3:
			newSurvivor->mFacing = MachineState::LEFT;
			break;
		}
		int x = rand() % 20; // Get a number between 0 and 19
		int y = rand() % 20; // Get a number between 0 and 19
		while (!emptyTiles[x][y])	// if the tile is occupied, random again
		{
			x = rand() % 20;
			y = rand() % 20;
		}
		newSurvivor->SetX(x);
		newSurvivor->SetY(y);
		emptyTiles[x][y] = false;
		newSurvivor->SetWorld(mWorld);
		newSurvivor->SetSurvivorMachine(mSurvivorMachine);
		newSurvivor->SetZombieMachine(mZombieMachine);
		mSurvivorMachine->AddCharacter(newSurvivor);
		mSurvivorMachine->BindState(*newSurvivor);
		mWorld->AddSurvivor(newSurvivor);
	}

	// add zombies
	for (unsigned int i = 0; i < 20; i++)
	{
		auto newZombie = std::make_shared<MachineState>();
		// randomly set the direction
		int direction = rand() % 4;
		switch (direction)
		{
		case 0:
			newZombie->mFacing = MachineState::UP;
			break;
		case 1:
			newZombie->mFacing = MachineState::RIGHT;
			break;
		case 2:
			newZombie->mFacing = MachineState::DOWN;
			break;
		case 3:
			newZombie->mFacing = MachineState::LEFT;
			break;
		}
		int x = rand() % 20; // Get a number between 0 and 19
		int y = rand() % 20; // Get a number between 0 and 19
		while (!emptyTiles[x][y])	// if the tile is occupied, random again
		{
			x = rand() % 20;
			y = rand() % 20;
		}
		newZombie->SetX(x);
		newZombie->SetY(y);
		emptyTiles[x][y] = false;
		newZombie->SetWorld(mWorld);
		newZombie->SetSurvivorMachine(mSurvivorMachine);
		newZombie->SetZombieMachine(mZombieMachine);
		mZombieMachine->AddCharacter(newZombie);
		mZombieMachine->BindState(*newZombie);
		mWorld->AddZombie(newZombie);
	}
	mSimMenu->Enable(ID_SImSTART, true);
	mSimMenu->Enable(ID_RESET, true);
	mPanel->PaintNow();
}

void ZomFrame::OnReset(wxCommandEvent & event)
{
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			mWorld->RemoveDead(i, j);
		}
	}
	for (int i = 0; i < 20; i++)
	{
		emptyTiles[i].resize(20);
		for (int j = 0; j < 20; j++)
		{
			emptyTiles[i][j] = true;
		}
	}
	mWorld->SetMonth(0);
	mPanel->PaintNow();
	mSimMenu->Enable(ID_SImSTART, false);
	mSimMenu->Enable(ID_RESET, false);
	mTurnTimer->Stop();
}