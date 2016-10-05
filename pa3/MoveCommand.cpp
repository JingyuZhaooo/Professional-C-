#include "MoveCommand.h"
#include "PaintModel.h"

MoveCommand::MoveCommand(const wxPoint& start, std::shared_ptr<Shape> shape) : Command(start, shape)
{
	mStartPos = start;
	mPreviousOffSet = shape->GetOffSet();
}

void MoveCommand::Update(const wxPoint & newPoint)
{
	mOffSet = mPreviousOffSet + (newPoint - mStartPos);
	mShape->SetOffSet(mOffSet);
}

void MoveCommand::Finalize(std::shared_ptr<PaintModel> model)
{

}

void MoveCommand::Undo(std::shared_ptr<PaintModel> model)
{
	mShape->SetOffSet(mPreviousOffSet);
}

void MoveCommand::Redo(std::shared_ptr<PaintModel> model)
{
	mShape->SetOffSet(mOffSet);
}