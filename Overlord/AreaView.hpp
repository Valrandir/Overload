#pragma once
#include "Rect.hpp"
#include "Size.hpp"
#include "Point.hpp"

/// <summary>
/// Control a view rectangle within a larger area / world rectangle.
/// The view can be larger or smaller then the world.
/// </summary>
class AreaView {
public:
	/// <param name="area">The size of the area</param>
	/// <param name="view">the size of the view</param>
	AreaView(Size area, Size view);

	/// <summary>
	/// Get the view position
	/// </summary>
	Point Position() const { return {zoomed_view.x, zoomed_view.y}; }

	/// <summary>
	/// Get the offset for drawing in the area, according to the position of the view
	/// </summary>
	Point Offset() const { return {-zoomed_view.x, -zoomed_view.y}; }

	/// <summary>
	/// Return the zoom factor
	/// </summary>
	float Zoom() const { return zoom; }

	/// <summary>
	/// Position the view in the center of the area.
	/// </summary>
	void CenterView();

	/// <summary>
	/// Set the zoom so that the view includes the whole area
	/// </summary>
	void ZoomToArea();

	/// <summary>
	/// Apply the specified zoom
	/// </summary>
	void Zoom(float factor);

private:
	void ApplyZoom() { Zoom(zoom); }

	Size area;
	Rect view;
	Rect zoomed_view;
	float zoom = 1.0f;
};
