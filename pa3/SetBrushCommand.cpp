#include "SetBrushCommand.h"
#include "Shape.h"
#include "PaintModel.h"

SetBrushCommand::SetBrushCommand(const wxPoint& start, std::shared_ptr<Shape> shape) : Command(start, shape)
{
	mPreviousColor = shape->GetBrushColor();
}

void SetBrushCommand::Update(const wxPoint & newPoint)
{

}

void SetBrushCommand::Finalize(std::shared_ptr<PaintModel> model)
{
	mColor = model->GetBrushColor();
	model->GetSelected()->SetBrushColor(mColor);
}

void SetBrushCommand::Undo(std::shared_ptr<PaintModel> model)
{
	wxColour tempColor = mColor;
	this->GetShape()->SetBrushColor(mPreviousColor);
	mColor = mPreviousColor;
	mPreviousColor = tempColor;
}

void SetBrushCommand::Redo(std::shared_ptr<PaintModel> model)
{
	wxColour tempColor = mColor;
	this->GetShape()->SetBrushColor(mPreviousColor);
	mColor = mPreviousColor;
	mPreviousColor = tempColor;
}