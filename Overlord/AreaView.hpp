#pragma once
#include "Rect.hpp"
#include "Size.hpp"
#include "Point.hpp"

class AreaView {
public:
	AreaView() :
		area{}, view{}, camera{}, camera_nozoom{} {}

	AreaView(Size area, Size view);

	Size Area() const { return area; }
	Size View() const { return view; }
	Rect Camera() const { return camera; }
	float Zoom() const { return zoom; }

	void OffsetCameraPosition(int x, int y);
	void OffsetCameraZoom(float zoom_offset);
	void ResetZoom();

private:
	Size area, view;
	Rect camera;
	Size camera_nozoom;
	float zoom = 1.f;
};
