#include "EllipseShape.h"

EllipseShape::EllipseShape(const wxPoint & start) : Shape(start)
{
	
}

void EllipseShape::Draw(wxDC & dc) const
{
	wxRect mWXRect{ mTopLeft, mBotRight };
	dc.DrawEllipse(mWXRect);
}