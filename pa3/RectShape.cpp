#include "RectShape.h"

RectShape::RectShape(const wxPoint & start) : Shape(start)
{

}

void RectShape::Draw(wxDC & dc) const
{
	wxRect mWXRect{ mTopLeft, mBotRight };
	dc.DrawRectangle(mWXRect);
}
