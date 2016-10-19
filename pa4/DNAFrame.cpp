//
//  DNAFrame.cpp
//  DNAwx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#include "DNAFrame.h"
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/log.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/filedlg.h>
#include <wx/busyinfo.h>
#include "DNADrawPanel.h"
#include "Exceptions.h"
#include "DNAAlignDlg.h"
#include <wx/wfstream.h>


enum
{
	ID_AMINO_HIST = 1000,
	ID_PAIRWISE_ALIGNMENT = 1001
};

wxBEGIN_EVENT_TABLE(DNAFrame, wxFrame)
	EVT_MENU(wxID_EXIT, DNAFrame::OnExit)
	EVT_MENU(wxID_NEW, DNAFrame::OnNew)
	EVT_MENU(ID_AMINO_HIST, DNAFrame::OnAminoHist)
	EVT_MENU(ID_PAIRWISE_ALIGNMENT, DNAFrame::OnPairwiseAlignment)
wxEND_EVENT_TABLE()

DNAFrame::DNAFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame(NULL, wxID_ANY, title, pos, size)
{
	// File menu
	wxMenu* menuFile = new wxMenu;
	menuFile->Append(wxID_NEW);
	menuFile->Append(ID_AMINO_HIST, "Amino Acid Histogram...",
					 "Create a histogram from a FASTA file.");
	menuFile->Append(ID_PAIRWISE_ALIGNMENT, "Pairwise Sequence Alignment...");
	menuFile->Append(wxID_EXIT);
	
	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	SetMenuBar(menuBar);
	CreateStatusBar();
	
	// Prepare the draw panel and show this frame
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	mPanel = new DNADrawPanel(this);
	sizer->Add(mPanel, 1, wxEXPAND);
	
	SetSizer(sizer);
	SetAutoLayout(true);
	Show(true);
	
	SetMinSize(GetSize());
	SetMaxSize(GetSize());
}

void DNAFrame::OnExit(wxCommandEvent& event)
{
	Close(true);
}

void DNAFrame::OnNew(wxCommandEvent& event)
{
	// Doesn't do anything...
}

void DNAFrame::OnAminoHist(wxCommandEvent& event)
{
	// TODO: Implement (File>Amino Acid Histogram...)
	wxFileDialog openFileDialog(this, _("Open FASTA file"), "./data", "", "FASTA files (*.fasta)|*.fasta", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
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
	std::string fileName = openFileDialog.GetPath();
	mFASTAFile.Load(fileName);

	mAminoAcidHist = AminoAcidHist(mFASTAFile.GetHeader(), mFASTAFile.GetSequence());
	mAminoAcidHist.Translate();
	mPanel->PassInData(mAminoAcidHist.GetAminoAcids(), mFASTAFile.GetHeader());


	
	for (unsigned i = 0; i < mAminoAcidHist.GetAminoAcids().size(); i++)
	{
		std::cout << mAminoAcidHist.GetAminoAcids()[i].name << ": " << mAminoAcidHist.GetAminoAcids()[i].count << ": " << mAminoAcidHist.GetAminoAcids()[i].percentage << std::endl;
	}
	mPanel->PaintNow();
}

void DNAFrame::OnPairwiseAlignment(wxCommandEvent & event)
{
	DNAAlignDlg dNAAlignDlg;
	if (dNAAlignDlg.ShowModal() == wxID_OK) //if the user correctly selected two input FASTA files and one output file
	{
		std::string fileAName = dNAAlignDlg.GetInputAPath();
		std::string fileBName = dNAAlignDlg.GetInputBPath();
		std::string fileOutputName = dNAAlignDlg.GetOutputPath();
		FASTAFile FASTAFileA;
		FASTAFile FASTAFileB;
		wxBusyInfo info("Calculating pairwise match...", this); // display to the user as it¡¯s working:
		FASTAFileA.Load(fileAName);
		FASTAFileB.Load(fileBName);
		mNeedlemanWunsch = NeedlemanWunsch(FASTAFileA, FASTAFileB);
		mNeedlemanWunsch.Run();
	}
		
	
}
