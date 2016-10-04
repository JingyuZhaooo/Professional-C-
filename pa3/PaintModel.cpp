#include "PaintModel.h"
#include <algorithm>
#include <wx/dcmemory.h>

PaintModel::PaintModel()
{
	mPen = *wxBLACK_PEN;
	mBrush = *wxWHITE_BRUSH;
}

// Draws any shapes in the model to the provided DC (draw context)
void PaintModel::DrawShapes(wxDC& dc, bool showSelection)
{
	for (auto& i : mShapes)
	{
		i->Draw(dc);
	}
	if (mSelected != nullptr)
	{
		mSelected->DrawSelection(dc);
	}
}

// Clear the current paint model and start fresh
void PaintModel::New()
{
	mShapes.clear();
	mActiveCommand = nullptr;
	while (!mUndo.empty())
	{
		mUndo.pop();
	}
	while (!mRedo.empty())
	{
		mRedo.pop();
	}
	mPen = *wxBLACK_PEN;
	mBrush = *wxWHITE_BRUSH;
	mSelected = nullptr;
}

// Add a shape to the paint model
void PaintModel::AddShape(std::shared_ptr<Shape> shape)
{
	mShapes.emplace_back(shape);
}

// Remove a shape from the paint model
void PaintModel::RemoveShape(std::shared_ptr<Shape> shape)
{
	auto iter = std::find(mShapes.begin(), mShapes.end(), shape);
	if (iter != mShapes.end())
	{
		mShapes.erase(iter);
	}
}

void PaintModel::SaveActiveCommand(CommandType type, wxPoint start)
{
	mActiveCommand = CreateCommand(type, start);
}

bool PaintModel::HasActiveCommand()
{
	if (mActiveCommand != nullptr)
	{
		return true;
	}
	return false;
}

std::shared_ptr<Command> PaintModel::CreateCommand(CommandType type, wxPoint start)
{
	auto command = CommandFactory::Create(shared_from_this(), type, start);
	switch (type)		// only add a shape when we actually draw a shape
	{
		case CM_DrawRect:
		case CM_DrawEllipse:
		case CM_DrawLine:
		case CM_DrawPencil:
		{
			AddShape(command->GetShape());
		}
	}
	return command;
}

void PaintModel::UpdateCommand(wxPoint point)
{
	mActiveCommand->Update(point);
}

void PaintModel::FinalizeCommand()
{
	mUndo.emplace(mActiveCommand); // When a command is finalized, push it onto the undo stack
	mActiveCommand->Finalize(shared_from_this());
	mActiveCommand.reset();
}

bool PaintModel::CanUndo() const
{
	if (mUndo.empty())
	{
		return false;
	}
	return true;
}

bool PaintModel::CanRedo() const
{
	if (mRedo.empty())
	{
		return false;
	}
	return true;
}

void PaintModel::Undo()
{
	if (CanUndo())
	{
		std::shared_ptr<Command> top = mUndo.top();
		mUndo.pop();
		top->Undo(shared_from_this());
		mRedo.emplace(top);
	}
	
}

void PaintModel::Redo()
{
	if (CanRedo())
	{
		std::shared_ptr<Command> top = mRedo.top();
		mRedo.pop();
		top->Redo(shared_from_this());
		mUndo.emplace(top);
	}
}

void PaintModel::ClearRedo()
{
	while (!mRedo.empty())
	{
		mRedo.pop();
	}
}

void PaintModel::Select(wxPoint point)
{
	for (int i = mShapes.size() - 1; i >= 0; i--)
	{
		if (mShapes[i]->Intersects(point))
		{
			mSelected = mShapes[i];
			return;
		}
	}
	mSelected = nullptr; //If you click where there isn¡¯t any shape, the selection should go away
}
