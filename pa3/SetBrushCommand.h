#pragma once
#include "Command.h"
#include <wx/bitmap.h>

class SetBrushCommand : public Command
{

public:
	SetBrushCommand(const wxPoint& start, std::shared_ptr<Shape> shape);

	// Called when the command is still updating (such as in the process of drawing)
	void Update(const wxPoint& newPoint) override;
	// Called when the command is completed
	void Finalize(std::shared_ptr<PaintModel> model) override;
	void Undo(std::shared_ptr<PaintModel> model) override;
	void Redo(std::shared_ptr<PaintModel> model) override;
	wxColour GetPreviousColor() { return mPreviousColor; };
private:
	wxColour mColor;
	wxColour mPreviousColor;
};