#include "PaintModel.h"
#include <algorithm>
#include <wx/dcmemory.h>

PaintModel::PaintModel()
{
	
}

// Draws any shapes in the model to the provided DC (draw context)
void PaintModel::DrawShapes(wxDC& dc, bool showSelection)
{
	for (auto& i : mShapes)
	{
		i->Draw(dc);
	}
}

// Clear the current paint model and start fresh
void PaintModel::New()
{
	// TODO
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
	AddShape(command->GetShape());
	return command;
}

void PaintModel::UpdateCommand(wxPoint point)
{
	mActiveCommand->Update(point);
}

void PaintModel::FinalizeCommand()
{
	mActiveCommand->Finalize(shared_from_this());
	mActiveCommand.reset();
}
