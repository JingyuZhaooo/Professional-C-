#include "PencilShape.h"

PencilShape::PencilShape(const wxPoint & start) : Shape(start)
{
	mWXPoints.push_back(mStartPoint);
}

void PencilShape::Draw(wxDC & dc) const
{
	dc.SetPen(mPen);
	dc.SetBrush(mBrush);
	int size = mWXPoints.size(); // get the number of points
	const wxPoint* points = &(mWXPoints[0]); // convert the vector to array
	dc.DrawLines(size, points);
}

void PencilShape::Update(const wxPoint & newPoint)
{
	Shape::Update(newPoint);
	mWXPoints.push_back(newPoint);
}

void PencilShape::Finalize()
{
	int MinX = mWXPoints[0].x;
	int MinY = mWXPoints[0].y;
	int MaxX = mWXPoints[0].x;
	int MaxY = mWXPoints[0].y;
	for (auto i : mWXPoints)
	{
		if (i.x < MinX)
		{
			MinX = i.x;
		}
		if (i.x > MaxX)
		{
			MaxX = i.x;
		}
		if (i.y < MinY)
		{
			MinY = i.y;
		}
		if (i.y > MaxY)
		{
			MaxY = i.y;
		}
	}
	mTopLeft = wxPoint(MinX, MaxY);
	mBotRight = wxPoint(MaxX, MinY);
}
