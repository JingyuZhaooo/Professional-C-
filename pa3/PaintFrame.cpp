#include "PaintFrame.h"
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/log.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/filedlg.h>
#include <wx/toolbar.h>
#include <wx/image.h>
#include <wx/colordlg.h>
#include <wx/textdlg.h>
#include <wx/filedlg.h>
#include "PaintDrawPanel.h"
#include "PaintModel.h"
#include "DrawCommand.h"
#include <wx/wfstream.h>

wxBEGIN_EVENT_TABLE(PaintFrame, wxFrame)
	EVT_MENU(wxID_EXIT, PaintFrame::OnExit)
	EVT_MENU(wxID_NEW, PaintFrame::OnNew)
	EVT_MENU(ID_Import, PaintFrame::OnImport)
	EVT_TOOL(ID_Import, PaintFrame::OnImport)
	EVT_MENU(ID_Export, PaintFrame::OnExport)
	EVT_TOOL(ID_Export, PaintFrame::OnExport)
	EVT_MENU(wxID_UNDO, PaintFrame::OnUndo)
	EVT_TOOL(wxID_UNDO, PaintFrame::OnUndo)
	EVT_MENU(wxID_REDO, PaintFrame::OnRedo)
	EVT_TOOL(wxID_REDO, PaintFrame::OnRedo)
	EVT_MENU(ID_Unselect, PaintFrame::OnUnselect)
	EVT_MENU(ID_Delete, PaintFrame::OnDelete)
	EVT_MENU(ID_SetPenColor, PaintFrame::OnSetPenColor)
	EVT_MENU(ID_SetPenWidth, PaintFrame::OnSetPenWidth)
	EVT_MENU(ID_SetBrushColor, PaintFrame::OnSetBrushColor)
	// The different draw modes
	EVT_TOOL(ID_Selector, PaintFrame::OnSelectTool)
	EVT_TOOL(ID_DrawLine, PaintFrame::OnSelectTool)
	EVT_TOOL(ID_DrawEllipse, PaintFrame::OnSelectTool)
	EVT_TOOL(ID_DrawRect, PaintFrame::OnSelectTool)
	EVT_TOOL(ID_DrawPencil, PaintFrame::OnSelectTool)
wxEND_EVENT_TABLE()	

PaintFrame::PaintFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame(NULL, wxID_ANY, title, pos, size)
{
	// Initialize image handlers to support BMP, PNG, JPEG
	wxImage::AddHandler(new wxPNGHandler());
	wxImage::AddHandler(new wxJPEGHandler());

	SetupMenu();
	
	SetupToolbar();

	SetupModelAndView();

	Show(true);
	
	SetMinSize(GetSize());
	SetMaxSize(GetSize());
}

void PaintFrame::SetupMenu()
{
	// File menu
	mFileMenu = new wxMenu();
	mFileMenu->Append(wxID_NEW);
	mFileMenu->Append(ID_Export, "Export...",
		"Export current drawing to image file.");
	mFileMenu->AppendSeparator();
	mFileMenu->Append(ID_Import, "Import...",
		"Import image into file.");
	mFileMenu->Append(wxID_EXIT);

	// Edit menu
	mEditMenu = new wxMenu();
	mEditMenu->Append(wxID_UNDO);
	mEditMenu->Append(wxID_REDO);
	mEditMenu->AppendSeparator();
	mEditMenu->Append(ID_Unselect, "Unselect",
		"Unselect the current selection");
	mEditMenu->AppendSeparator();
	mEditMenu->Append(ID_Delete, "Delete\tDel",
		"Delete the current selection");
	
	mEditMenu->Enable(wxID_UNDO, false);
	mEditMenu->Enable(wxID_REDO, false);
	mEditMenu->Enable(ID_Unselect, false);
	mEditMenu->Enable(ID_Delete, false);

	// Colors menu
	mColorMenu = new wxMenu();
	mColorMenu->Append(ID_SetPenColor, "Pen Color...", "Set the pen color.");
	mColorMenu->Append(ID_SetPenWidth, "Pen Width...", "Set the pen width.");
	mColorMenu->AppendSeparator();
	mColorMenu->Append(ID_SetBrushColor, "Brush Color...", "Set brush color");

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(mFileMenu, "&File");
	menuBar->Append(mEditMenu, "&Edit");
	menuBar->Append(mColorMenu, "&Colors");
	SetMenuBar(menuBar);
	CreateStatusBar();
}

void PaintFrame::SetupToolbar()
{
	// Create the toolbar
	mToolbar = CreateToolBar();
	mToolbar->AddTool(wxID_NEW, "New",
		wxBitmap("Icons/New.png", wxBITMAP_TYPE_PNG),
		"New");
	mToolbar->AddTool(ID_Export, "Export",
		wxBitmap("Icons/Save.png", wxBITMAP_TYPE_PNG),
		"Export");
	mToolbar->AddTool(ID_Import, "Import",
		wxBitmap("Icons/Import.png", wxBITMAP_TYPE_PNG),
		"Import");
	mToolbar->AddSeparator();

	mToolbar->AddTool(wxID_UNDO, "Undo",
		wxBitmap("Icons/Undo.png", wxBITMAP_TYPE_PNG),
		"Undo");
	mToolbar->AddTool(wxID_REDO, "Redo",
		wxBitmap("Icons/Redo.png", wxBITMAP_TYPE_PNG),
		"Redo");
	mToolbar->AddSeparator();

	mToolbar->AddTool(ID_Selector, "Selector",
		wxBitmap("Icons/Cursor.png", wxBITMAP_TYPE_PNG),
		"Selector", wxITEM_CHECK);
	mToolbar->AddTool(ID_DrawLine, "Draw Line",
		wxBitmap("Icons/Line.png", wxBITMAP_TYPE_PNG),
		"Draw Line", wxITEM_CHECK);
	mToolbar->AddTool(ID_DrawEllipse, "Draw Ellipse",
		wxBitmap("Icons/Ellipse.png", wxBITMAP_TYPE_PNG),
		"Draw Ellipse", wxITEM_CHECK);
	mToolbar->AddTool(ID_DrawRect, "Draw Rectangle",
		wxBitmap("Icons/Rectangle.png", wxBITMAP_TYPE_PNG),
		"Draw Rectangle", wxITEM_CHECK);
	mToolbar->AddTool(ID_DrawPencil, "Pencil",
		wxBitmap("Icons/Pencil.png", wxBITMAP_TYPE_PNG),
		"Pencil", wxITEM_CHECK);

	mToolbar->Realize();

	ToggleTool(ID_Selector);

	// Both undo and redo are disabled initially
	mToolbar->EnableTool(wxID_UNDO, false);
	mToolbar->EnableTool(wxID_REDO, false);
}

void PaintFrame::SetupModelAndView()
{
	// Prepare the draw panel and show this frame
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	mPanel = new PaintDrawPanel(this);
	sizer->Add(mPanel, 1, wxEXPAND);

	// Programatically bind the mouse events on the draw panel to us
	mPanel->Bind(wxEVT_LEFT_DOWN, &PaintFrame::OnMouseButton, this);
	mPanel->Bind(wxEVT_LEFT_UP, &PaintFrame::OnMouseButton, this);
	mPanel->Bind(wxEVT_MOTION, &PaintFrame::OnMouseMove, this);

	// Create the model
	mModel = std::make_shared<PaintModel>();
	mPanel->SetModel(mModel);
	SetSizer(sizer);

	SetAutoLayout(true);
}

void PaintFrame::OnExit(wxCommandEvent& event)
{
	Close(true);
}

void PaintFrame::OnNew(wxCommandEvent& event)
{
	mModel->New();
	mToolbar->EnableTool(wxID_UNDO, false);
	mEditMenu->Enable(wxID_UNDO, false);
	mToolbar->EnableTool(wxID_REDO, false);
	mEditMenu->Enable(wxID_REDO, false);
	mPanel->PaintNow();
}

void PaintFrame::OnExport(wxCommandEvent& event)
{
	wxFileDialog saveFileDialog(this, _("Save .png file"), "", "Random Drawing",
		"BMP files (*.bmp)|*.bmp|PNG files (*.png)|*.png|JPEG and JPG files (*.jpeg;*.jpg)|*.jpeg;*.jpg", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (saveFileDialog.ShowModal() == wxID_CANCEL)
		return;     // the user changed idea...

					// save the current contents in the file;
					// this can be done with e.g. wxWidgets output streams:
	wxFileOutputStream output_stream(saveFileDialog.GetPath());
	if (!output_stream.IsOk())
	{
		wxLogError("Cannot save current contents in file '%s'.", saveFileDialog.GetPath());
		return;
	}
	std::string fileName = saveFileDialog.GetPath();
	mModel->Save(fileName, mPanel->GetSize());
}

void PaintFrame::OnImport(wxCommandEvent& event)
{
	wxFileDialog openFileDialog(this, _("Open XYZ file"), "", "", "XYZ files (*.xyz)|*.xyz", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
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
}

void PaintFrame::OnUndo(wxCommandEvent& event)
{
	mModel->Undo();
	mPanel->PaintNow();
	if (!mModel->CanUndo())
	{
		mToolbar->EnableTool(wxID_UNDO, false);
		mEditMenu->Enable(wxID_UNDO, false);
	}
	mToolbar->EnableTool(wxID_REDO, true);
	mEditMenu->Enable(wxID_REDO, true);
}

void PaintFrame::OnRedo(wxCommandEvent& event)
{
	mModel->Redo();
	mPanel->PaintNow();
	if (!mModel->CanRedo())
	{
		mToolbar->EnableTool(wxID_REDO, false);
		mEditMenu->Enable(wxID_REDO, false);
	}
	mToolbar->EnableTool(wxID_UNDO, true);
	mEditMenu->Enable(wxID_UNDO, true);
}

void PaintFrame::OnUnselect(wxCommandEvent& event)
{
	if (mModel->GetSelected() != nullptr)
	{
		mModel->SetSelected(nullptr);
		mEditMenu->Enable(ID_Unselect, false);
		mPanel->PaintNow();
	}
}

void PaintFrame::OnDelete(wxCommandEvent& event)
{
	if (mModel->GetSelected() != nullptr)
	{
		wxPoint x(0, 0);
		mModel->SaveActiveCommand(CM_Delete, x);


		mModel->SetSelected(nullptr);
		mEditMenu->Enable(ID_Delete, false);
		mPanel->PaintNow();
		mModel->FinalizeCommand();
	}
	
}

void PaintFrame::OnSetPenColor(wxCommandEvent& event)
{
	wxColourData data; 
	data.SetColour(mModel->GetPenColor());
	wxColourDialog dialog(this, &data); 
	if (dialog.ShowModal() == wxID_OK) 
	{ 
		wxColourData colorData = dialog.GetColourData();
		wxColour color = colorData.GetColour();
		mModel->SetPenColor(color);
	}
	wxPoint x(0, 0);
	if (mModel->GetSelected() != nullptr)
	{
		mModel->SaveActiveCommand(CM_SetPen, x);
		mPanel->PaintNow();
		mModel->FinalizeCommand();
	}
}

void PaintFrame::OnSetPenWidth(wxCommandEvent& event)
{
	wxTextEntryDialog textEntryDialog;
	textEntryDialog.Create(mPanel, "Enter value between 1 and 10", "Set the pen width", "", wxTextEntryDialogStyle, wxDefaultPosition);
	if (textEntryDialog.ShowModal() == wxID_OK)
	{
		std::string num = textEntryDialog.GetValue();
		if (num.length() == 1 && isdigit(num[0]))
		{
			int width = std::stoi(num);
			mModel->SetPenWidth(width);
			
		}
		else if (num.length() == 2 && isdigit(num[0]) && isdigit(num[1]))
		{
			int width = std::stoi(num);
			if (width == 10)
			{
				mModel->SetPenWidth(width);
			}
		}
	}
	wxPoint x(0, 0);
	if (mModel->GetSelected() != nullptr)
	{
		mModel->SaveActiveCommand(CM_SetPen, x);
		mPanel->PaintNow();
		mModel->FinalizeCommand();
	}
}

void PaintFrame::OnSetBrushColor(wxCommandEvent& event)
{
	wxColourData data;
	data.SetColour(mModel->GetBrushColor());
	wxColourDialog dialog(this, &data);
	if (dialog.ShowModal() == wxID_OK)
	{
		wxColourData colorData = dialog.GetColourData();
		wxColour color = colorData.GetColour();
		mModel->SetBrushColor(color);
	}
	wxPoint x(0, 0);
	if (mModel->GetSelected() != nullptr)
	{
		mModel->SaveActiveCommand(CM_SetBrush, x);
		mPanel->PaintNow();
		mModel->FinalizeCommand();
	}
}

void PaintFrame::OnMouseButton(wxMouseEvent& event)
{
	if (event.LeftDown())
	{
		switch (mCurrentTool)
		{
			case ID_DrawRect:
			{
				mModel->SaveActiveCommand(CM_DrawRect, event.GetPosition());
				mPanel->PaintNow();
				break;
			}
			case ID_DrawEllipse:
			{
				mModel->SaveActiveCommand(CM_DrawEllipse, event.GetPosition());
				mPanel->PaintNow();
				break;
			}
			case ID_DrawLine:
			{
				mModel->SaveActiveCommand(CM_DrawLine, event.GetPosition());
				mPanel->PaintNow();
				break;
			}
			case ID_DrawPencil:
			{
				mModel->SaveActiveCommand(CM_DrawPencil, event.GetPosition());
				mPanel->PaintNow();
				break;
			}
			case ID_Selector:
			{
				mModel->Select(event.GetPosition());
				if (mModel->GetSelected() != nullptr)
				{
					mEditMenu->Enable(ID_Unselect, true);
					mEditMenu->Enable(ID_Delete, true);
					mPanel->PaintNow();
					if (mModel->GetSelected()->Intersects(event.GetPosition())) // the cursor is over the selection and we click on it, we want to drag it
					{
						mModel->SaveActiveCommand(CM_Move, event.GetPosition());
						mPanel->PaintNow();
						break;
					}
				}
				else
				{
					mEditMenu->Enable(ID_Unselect, false);
					mEditMenu->Enable(ID_Delete, false);
					mPanel->PaintNow();
				}
				break;

			}
		}
	}
	else if (event.LeftUp())
	{
		switch (mCurrentTool)
		{
			case ID_DrawRect:
			case ID_DrawEllipse:
			case ID_DrawLine:
			case ID_DrawPencil:
			case ID_Selector:
			{
				if (mModel->HasActiveCommand())
				{
					mModel->UpdateCommand(event.GetPosition());
					mPanel->PaintNow();
					mModel->FinalizeCommand();
					break;
				}
			}
		}
		mModel->ClearRedo();
	}
	if (mModel->CanUndo())
	{
		mToolbar->EnableTool(wxID_UNDO, true);
		mEditMenu->Enable(wxID_UNDO, true);
	}
	else
	{
		mToolbar->EnableTool(wxID_UNDO, false);
		mEditMenu->Enable(wxID_UNDO, false);
	}
	if (mModel->CanRedo())
	{
		mToolbar->EnableTool(wxID_REDO, true);
		mEditMenu->Enable(wxID_REDO, true);
	}
	else
	{
		mToolbar->EnableTool(wxID_REDO, false);
		mEditMenu->Enable(wxID_REDO, false);
	}
}

void PaintFrame::OnMouseMove(wxMouseEvent& event)
{
	// TODO: This is when the mouse is moved inside the drawable area
	if (mModel->GetSelected() != nullptr)	// currently has a selection
	{
		if (mModel->GetSelected()->Intersects(event.GetPosition())) // the cursor is over the selection
		{
			this->SetCursor(CU_Move);

		}
		else
		{
			this->SetCursor(CU_Default);
		}
	}
	if (mModel->HasActiveCommand())
	{
		mModel->UpdateCommand(event.GetPosition());
		mPanel->PaintNow();
	}
}

void PaintFrame::ToggleTool(EventID toolID)
{
	// Deselect everything
	for (int i = ID_Selector; i <= ID_DrawPencil; i++)
	{
		mToolbar->ToggleTool(i, false);
	}

	// Select the new tool
	mToolbar->ToggleTool(toolID, true);

	mCurrentTool = toolID;
}

void PaintFrame::SetCursor(CursorType type)
{
	wxCursor* cursor = mCursors.GetCursor(type);
	if (cursor != nullptr)
	{
		mPanel->SetCursor(*cursor);
	}
}

void PaintFrame::OnSelectTool(wxCommandEvent& event)
{
	EventID id = static_cast<EventID>(event.GetId());
	ToggleTool(id);

	// Select appropriate cursor
	switch (id)
	{
	case ID_DrawLine:
	case ID_DrawEllipse:
	case ID_DrawRect:
		SetCursor(CU_Cross);
		break;
	case ID_DrawPencil:
		SetCursor(CU_Pencil);
		break;
	default:
		SetCursor(CU_Default);
		break;
	}
}
