#pragma once
#include <memory>
#include <vector>
#include "Shape.h"
#include "Command.h"
#include <wx/bitmap.h>
#include <stack>

class PaintModel : public std::enable_shared_from_this<PaintModel>
{
public:
	PaintModel();
	
	// Draws any shapes in the model to the provided DC (draw context)
	void DrawShapes(wxDC& dc, bool showSelection = true);

	// Clear the current paint model and start fresh
	void New();

	// Add a shape to the paint model
	void AddShape(std::shared_ptr<Shape> shape);
	// Remove a shape from the paint model
	void RemoveShape(std::shared_ptr<Shape> shape);
	// Save the current active command in a shared_ptr to a Command

	void SaveActiveCommand(CommandType type, wxPoint start);
	bool HasActiveCommand();
	std::shared_ptr<Command> CreateCommand(CommandType type, wxPoint start);
	void UpdateCommand(wxPoint point);
	void FinalizeCommand();
	bool CanUndo();
	bool CanRedo();
	void Undo();
	void Redo();
	void ClearRedo(); // clear the Redo stack after adding a new command
	//Getters and Setters for wxPen and wxBrush
	int GetPenWidth() { return mPen.GetWidth(); };
	wxColour GetPenColor() { return mPen.GetColour(); };
	void SetPenWidth(int width) { mPen.SetWidth(width); };
	void SetPenColor(wxColour color) { mPen.SetColour(color); };
	wxPen GetPen() { return mPen; };
	wxColour GetBrushColor() { return mBrush.GetColour(); };
	void SetBrushColor(wxColour color) { mBrush.SetColour(color); };
	wxBrush GetBrush() { return mBrush; };
private:
	// Vector of all the shapes in the model
	std::vector<std::shared_ptr<Shape>> mShapes;
	std::shared_ptr<Command> mActiveCommand;
	std::stack<std::shared_ptr<Command>> mUndo;
	std::stack<std::shared_ptr<Command>> mRedo;
	wxPen mPen;
	wxBrush mBrush;
};
