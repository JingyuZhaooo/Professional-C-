//
//  DNADrawPanel.cpp
//  DNAwx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#include "DNADrawPanel.h"
#include <wx/dcclient.h>
#include <wx/sizer.h>
#include <array>
#include <sstream>
#include <iomanip>

BEGIN_EVENT_TABLE(DNADrawPanel, wxPanel)
	EVT_PAINT(DNADrawPanel::PaintEvent)
END_EVENT_TABLE()

DNADrawPanel::DNADrawPanel(wxFrame* parent)
: wxPanel(parent)
{
	
}

void DNADrawPanel::PaintEvent(wxPaintEvent & evt)
{
	wxPaintDC dc(this);
	Render(dc);
}

void DNADrawPanel::PaintNow()
{
	wxClientDC dc(this);
	Render(dc);
}

void DNADrawPanel::Render(wxDC& dc)
{
	// Clear
	dc.SetBackground(*wxWHITE_BRUSH);
	dc.Clear();
	
	// TODO: Draw histogram, if one has been generated
	if (mAminoAcids.size() != 0)
	{
		unsigned i = 0;
		while (i < mAminoAcids.size())
		{
			dc.SetPen(*wxBLACK_PEN);
			if (i % 3 == 0)
			{
				dc.SetTextForeground(*wxRED);
				dc.SetBrush(*wxRED_BRUSH);
				dc.DrawRectangle(wxCoord(50), wxCoord(90), wxCoord(mAminoAcids[i].count * 10), wxCoord(30));
				dc.DrawText(mAminoAcids[i].name, wxPoint(50, 90));
			}
			i += 1;
		}
	}
	
	
}

void DNADrawPanel::PassInData(std::vector<AminoAcid> aminoAcids)
{
	mAminoAcids = aminoAcids;
}
