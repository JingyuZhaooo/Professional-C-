#pragma once
#include <wx/dc.h>

// Abstract base class for all Shapes
class Shape
{
public:
	Shape(const wxPoint& start);
	// Tests whether the provided point intersects
	// with this shape
	bool Intersects(const wxPoint& point) const;
	// Update shape with new provided point
	virtual void Update(const wxPoint& newPoint);
	// Finalize the shape -- when the user has finished drawing the shape
	virtual void Finalize();
	// Returns the top left/bottom right points of the shape
	void GetBounds(wxPoint& topLeft, wxPoint& botRight) const;
	// Draw the shape
	virtual void Draw(wxDC& dc) const = 0;
	virtual ~Shape() { }	
	//Getters and Setters for wxPen and wxBrush
	int GetPenWidth() { return mPen.GetWidth(); };
	wxColour GetPenColor() { return mPen.GetColour(); };
	void SetPenWidth(int width) { mPen.SetWidth(width); };
	void SetPenColor(wxColour color) { mPen.SetColour(color); };
	wxPen GetPen() { return mPen; };
	wxColour GetBrushColor() { return mBrush.GetColour(); };
	void SetBrushColor(wxColour color) { mBrush.SetColour(color); };
	wxBrush GetBrush() { return mBrush; };
	void SetPen(wxPen pen) { mPen = pen; };
	void SetBrush(wxBrush brush) { mBrush = brush; };
	void DrawSelection(wxDC& dc);
protected:
	// Starting point of shape
	wxPoint mStartPoint;
	// Ending point of shape
	wxPoint mEndPoint;
	// Top left point of shape
	wxPoint mTopLeft;
	// Bottom right point of shape
	wxPoint mBotRight;

	wxPen mPen;
	wxBrush mBrush;
};
