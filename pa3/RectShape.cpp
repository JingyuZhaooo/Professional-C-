#include "RectShape.h"

RectShape::RectShape(const wxPoint & start) : Shape(start)
{

}

void RectShape::Draw(wxDC & dc) const
{
	dc.SetPen(mPen);
	dc.SetBrush(mBrush);
	wxRect mWXRect{ mTopLeft, mBotRight };
	dc.DrawRectangle(mWXRect);
}
