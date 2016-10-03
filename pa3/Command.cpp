#include "Command.h"
#include "Shape.h"
#include "PaintModel.h"
#include "DrawCommand.h"
#include "RectShape.h"
#include "EllipseShape.h"
#include "LineShape.h"
#include "PencilShape.h"

Command::Command(const wxPoint& start, std::shared_ptr<Shape> shape)
	:mStartPoint(start)
	,mEndPoint(start)
	,mShape(shape)
{

}

// Called when the command is still updating (such as in the process of drawing)
void Command::Update(const wxPoint& newPoint)
{
	mEndPoint = newPoint;
}

std::shared_ptr<Command> CommandFactory::Create(std::shared_ptr<PaintModel> model,
	CommandType type, const wxPoint& start)
{
	std::shared_ptr<Command> retVal;
	// TODO: Actually create a command based on the type passed in
	switch (type)
	{
		case CM_DrawRect:
		{
			auto rect = std::make_shared<RectShape>(start);
			auto command = std::make_shared<DrawCommand>(start, rect);
			retVal = command;
			break;
		}
		case CM_DrawEllipse:
		{
			auto ellipse = std::make_shared<EllipseShape>(start);
			auto command = std::make_shared<DrawCommand>(start, ellipse);
			retVal = command;
			break;
		}
		case CM_DrawLine:
		{
			auto line = std::make_shared<LineShape>(start);
			auto command = std::make_shared<DrawCommand>(start, line);
			retVal = command;
			break;
		}
		case CM_DrawPencil:
		{
			auto pencil = std::make_shared<PencilShape>(start);
			auto command = std::make_shared<DrawCommand>(start, pencil);
			retVal = command;
			break;
		}
	}
	
	return retVal;
}
