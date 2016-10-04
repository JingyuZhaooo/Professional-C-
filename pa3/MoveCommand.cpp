#include "MoveCommand.h"
#include "PaintModel.h"

MoveCommand::MoveCommand(const wxPoint& start, std::shared_ptr<Shape> shape) : Command(start, shape)
{

}

void MoveCommand::Update(const wxPoint & newPoint)
{

}

void MoveCommand::Finalize(std::shared_ptr<PaintModel> model)
{
}

void MoveCommand::Undo(std::shared_ptr<PaintModel> model)
{
}

void MoveCommand::Redo(std::shared_ptr<PaintModel> model)
{

}