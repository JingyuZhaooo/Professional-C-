//
//  ZomDrawPanel.h
//  zombiewx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#pragma once
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/frame.h>
#include <string>
#include "World.h"

class ZomDrawPanel : public wxPanel
{
 
public:
	ZomDrawPanel(wxFrame* parent);
	void PaintNow();
	void UpdateWorld(wxDC& dc);
	void SetWorld(std::shared_ptr<World> &world) { mWorld = world; }
protected:
	void PaintEvent(wxPaintEvent & evt);
	void Render(wxDC& dc);
	void DrawGrid(wxDC& dc);
 
	DECLARE_EVENT_TABLE()
	
public:
	
	// Variables here
	std::shared_ptr<World> mWorld = std::make_shared<World>();
};

