#pragma once
#include "Shape.h"

class PencilShape : public Shape
{
public:
	PencilShape(const wxPoint& start);
	void Draw(wxDC& dc) const override;
	// Update shape with new provided point
	void Update(const wxPoint& newPoint) override;
	// Finalize the shape -- when the user has finished drawing the shape
	void Finalize() override;
private:
	std::vector<wxPoint> mWXPoints;
};