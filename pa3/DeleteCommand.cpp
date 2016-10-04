#include "DeleteCommand.h"
#include "PaintModel.h"

DeleteCommand::DeleteCommand(const wxPoint& start, std::shared_ptr<Shape> shape) : Command(start, shape)
{

}

void DeleteCommand::Update(const wxPoint & newPoint)
{

}

void DeleteCommand::Finalize(std::shared_ptr<PaintModel> model)
{
	model->RemoveShape(this->GetShape());
	mShape->Finalize();
}

void DeleteCommand::Undo(std::shared_ptr<PaintModel> model)
{
	model->AddShape(this->GetShape());
}

void DeleteCommand::Redo(std::shared_ptr<PaintModel> model)
{
	
	model->RemoveShape(this->GetShape());
}