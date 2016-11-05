//
//  ZomDrawPanel.cpp
//  zombiewx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#include "ZomDrawPanel.h"
#include <wx/dcclient.h>
#include <wx/sizer.h>
#include "Machine.h"

BEGIN_EVENT_TABLE(ZomDrawPanel, wxPanel)
	EVT_PAINT(ZomDrawPanel::PaintEvent)
END_EVENT_TABLE()

ZomDrawPanel::ZomDrawPanel(wxFrame* parent)
: wxPanel(parent)
{
	
}

void ZomDrawPanel::PaintEvent(wxPaintEvent & evt)
{
	wxPaintDC dc(this);
	Render(dc);
}

void ZomDrawPanel::PaintNow()
{
	wxClientDC dc(this);
	Render(dc);
	UpdateWorld(dc);
}

void ZomDrawPanel::UpdateWorld(wxDC & dc)
{
	for (unsigned int i = 0; i < mWorld->GetSurvivors().size(); i++)
	{
		wxPoint point1; // Facing direction
		wxPoint point2; // BottomLeft point
		wxPoint point3;	// BottomRight point
		int XCoordinate = (mWorld->GetSurvivors()[i]->GetX() * 30) + 10;
		int YCoordinate = (mWorld->GetSurvivors()[i]->GetY() * 30) + 10;
		switch (mWorld->GetSurvivors()[i]->mFacing)
		{
		case 0: // UP
			point1 = wxPoint((XCoordinate + 15), YCoordinate);
			point2 = wxPoint(XCoordinate, YCoordinate + 30);
			point3 = wxPoint(XCoordinate + 30, YCoordinate + 30);
			break;
		case 1: // RIGHT
			point1 = wxPoint(XCoordinate + 30, YCoordinate + 15);
			point2 = wxPoint(XCoordinate, YCoordinate);
			point3 = wxPoint(XCoordinate, YCoordinate + 30);
			break;
		case 2: // DOWN
			point1 = wxPoint(XCoordinate + 15, YCoordinate + 30);
			point2 = wxPoint(XCoordinate + 30, YCoordinate);
			point3 = wxPoint(XCoordinate, YCoordinate);
			break;
		case 3: // LEFT
			point1 = wxPoint(XCoordinate, YCoordinate + 15);
			point2 = wxPoint(XCoordinate + 30, YCoordinate + 30);
			point3 = wxPoint(XCoordinate + 30, YCoordinate);
			break;
		}
		wxPoint points[3] = { point1, point2, point3 }; // points used to draw the Polygon
		dc.SetBrush(*wxGREEN_BRUSH);
		dc.DrawPolygon(3, points);
	}
	for (unsigned int i = 0; i < mWorld->GetZombies().size(); i++)
	{
		wxPoint point1; // Facing direction
		wxPoint point2; // BottomLeft point
		wxPoint point3;	// BottomRight point
		int XCoordinate = (mWorld->GetZombies()[i]->GetX() * 30) + 10;
		int YCoordinate = (mWorld->GetZombies()[i]->GetY() * 30) + 10;
		switch (mWorld->GetZombies()[i]->mFacing)
		{
		case 0: // UP
			point1 = wxPoint((XCoordinate + 15), YCoordinate);
			point2 = wxPoint(XCoordinate, YCoordinate + 30);
			point3 = wxPoint(XCoordinate + 30, YCoordinate + 30);
			break;
		case 1: // RIGHT
			point1 = wxPoint(XCoordinate + 30, YCoordinate + 15);
			point2 = wxPoint(XCoordinate, YCoordinate);
			point3 = wxPoint(XCoordinate, YCoordinate + 30);
			break;
		case 2: // DOWN
			point1 = wxPoint(XCoordinate + 15, YCoordinate + 30);
			point2 = wxPoint(XCoordinate + 30, YCoordinate);
			point3 = wxPoint(XCoordinate, YCoordinate);
			break;
		case 3: // LEFT
			point1 = wxPoint(XCoordinate, YCoordinate + 15);
			point2 = wxPoint(XCoordinate + 30, YCoordinate + 30);
			point3 = wxPoint(XCoordinate + 30, YCoordinate);
			break;
		}
		wxPoint points[3] = { point1, point2, point3 }; // points used to draw the Polygon
		dc.SetBrush(*wxRED_BRUSH);
		dc.DrawPolygon(3, points);
	}

	dc.SetTextForeground(*wxRED);
	dc.DrawText("Zombies", wxPoint(620, 10));
	dc.DrawText("Program: " + mWorld->GetZombieFileName(), wxPoint(620, 25));
	dc.DrawText("Alive: " + std::to_string(mWorld->GetZombies().size()), wxPoint(620, 40));
	dc.SetTextForeground(*wxGREEN);
	dc.DrawText("Humans", wxPoint(620, 110));
	dc.DrawText("Program: " + mWorld->GetSurvivorFileName(), wxPoint(620, 125));
	dc.DrawText("Alive: " + std::to_string(mWorld->GetSurvivors().size()), wxPoint(620, 140));
	dc.SetTextForeground(*wxBLACK);
	dc.DrawText("Month: " + std::to_string(mWorld->GetMonth()), wxPoint(620, 220));
}

void ZomDrawPanel::Render(wxDC& dc)
{
	// Clear
	dc.SetBackground(*wxWHITE_BRUSH);
	dc.Clear();
	
	// Draw the grid
	DrawGrid(dc);
}

void ZomDrawPanel::DrawGrid(wxDC& dc)
{
	dc.SetPen(*wxBLACK_PEN);
	dc.DrawRectangle(10, 10, 600, 600);
	for (int i = 1; i < 20; i++)
	{
		dc.DrawLine(wxPoint(10 + (30 * i), 10), wxPoint(10 + (30 * i), 610));
		dc.DrawLine(wxPoint(10, 10 + (30 * i)), wxPoint(610, (30 * i) + 10));
	}
	UpdateWorld(dc);
}
