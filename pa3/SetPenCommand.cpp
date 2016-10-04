#include "SetPenCommand.h"
#include "Shape.h"
#include "PaintModel.h"

SetPenCommand::SetPenCommand(const wxPoint& start, std::shared_ptr<Shape> shape) : Command(start, shape)
{
	mPreviousColor = shape->GetPenColor();
	mPreviousWidth = shape->GetPenWidth();
}

void SetPenCommand::Update(const wxPoint & newPoint)
{

}

void SetPenCommand::Finalize(std::shared_ptr<PaintModel> model)
{
	mColor = model->GetPenColor();
	mPenWidth = model->GetPenWidth();
	model->GetSelected()->SetPenColor(mColor);
	model->GetSelected()->SetPenWidth(mPenWidth);
}

void SetPenCommand::Undo(std::shared_ptr<PaintModel> model)
{
	// doing a->c, b->a, c->b
	wxColor tempColor = mColor;
	int tempWidth = mPenWidth;
	this->GetShape()->SetPenColor(mPreviousColor);
	this->GetShape()->SetPenWidth(mPreviousWidth);
	mColor = mPreviousColor;
	mPenWidth = mPreviousWidth;
	mPreviousColor = tempColor;
	mPreviousWidth = tempWidth;
}

void SetPenCommand::Redo(std::shared_ptr<PaintModel> model)
{
	// doing a->c, b->a, c->b
	wxColor tempColor = mColor;
	int tempWidth = mPenWidth;
	this->GetShape()->SetPenColor(mPreviousColor);
	this->GetShape()->SetPenWidth(mPreviousWidth);
	mColor = mPreviousColor;
	mPenWidth = mPreviousWidth;
	mPreviousColor = tempColor;
	mPreviousWidth = tempWidth;
}