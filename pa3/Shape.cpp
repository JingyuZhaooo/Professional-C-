#include "Shape.h"

Shape::Shape(const wxPoint& start)
	:mStartPoint(start)
	, mEndPoint(start)
	, mTopLeft(start)
	, mBotRight(start)
	, mOffSet(0,0)
{

}

// Tests whether the provided point intersects
// with this shape
bool Shape::Intersects(const wxPoint& point) const
{
	wxPoint topleft;
	wxPoint botright;
	GetBounds(topleft, botright);
	if (point.x >= topleft.x && point.x <= botright.x &&
		point.y >= topleft.y && point.y <= botright.y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Update shape with new provided point
void Shape::Update(const wxPoint& newPoint)
{
	mEndPoint = newPoint;

	// For most shapes, we only have two points - start and end
	// So we can figure out the top left/bottom right bounds
	// based on this.
	mTopLeft.x = std::min(mStartPoint.x, mEndPoint.x);
	mTopLeft.y = std::min(mStartPoint.y, mEndPoint.y);
	mBotRight.x = std::max(mStartPoint.x, mEndPoint.x);
	mBotRight.y = std::max(mStartPoint.y, mEndPoint.y);
}

void Shape::Finalize()
{
	// Default finalize doesn't do anything
}

void Shape::GetBounds(wxPoint& topLeft, wxPoint& botRight) const
{
	topLeft = mTopLeft + mOffSet;
	botRight = mBotRight + mOffSet;
}

void Shape::DrawSelection(wxDC& dc)
{
	dc.SetPen(*wxBLACK_DASHED_PEN);
	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	wxPoint topLeft(mTopLeft.x - 2 + mOffSet.x, mTopLeft.y - 2 + mOffSet.y);
	wxPoint bottomRight(mBotRight.x + 2 + mOffSet.x, mBotRight.y + 2 + mOffSet.y);
	wxRect wxRect{ topLeft, bottomRight };
	dc.DrawRectangle(wxRect);
}
