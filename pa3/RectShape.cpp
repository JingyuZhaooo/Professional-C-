#include "RectShape.h"

RectShape::RectShape(const wxPoint & start) : Shape(start)
{

}

void RectShape::Draw(wxDC & dc) const
{
	dc.SetPen(mPen);
	dc.SetBrush(mBrush);
	wxRect mWXRect{ mTopLeft + mOffSet, mBotRight + mOffSet };
	dc.DrawRectangle(mWXRect);
}
