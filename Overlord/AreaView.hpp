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
	AreaView() :
		area{}, view{}, zoomed_view{} {}

	/// <param name="area">The size of the area</param>
	/// <param name="view">the size of the view</param>
	AreaView(Size area, Size view);

	/// <summary>
	/// Get the size of the area
	/// </summary>
	Size AreaSize() const { return area;}

	/// <summary>
	/// Get the view rectangle
	/// </summary>
	Rect ViewSize() const { return zoomed_view; }

	/// <summary>
	/// Get the view position
	/// </summary>
	Point Position() const { return {zoomed_view.x, zoomed_view.y}; }

	/// <summary>
	/// Get the offset for drawing in the area, according to the position of the view
	/// </summary>
	Point Offset() const { return {-zoomed_view.x, -zoomed_view.y}; }

	/// <summary>
	/// Get the offset for drawing in the area, according to the position of the view
	/// </summary>
	void Offset(Point offset);

	/// <summary>
	/// Return the zoom factor
	/// </summary>
	float Zoom() const { return zoom; }

	/// <summary>
	/// Apply the specified zoom
	/// </summary>
	void Zoom(float factor);

	/// <summary>
	/// Change the current zoom
	/// </summary>
	/// <param name="offset_factor">How much to change the current zoom</param>
	void OffsetZoom(float offset_factor);

	/// <summary>
	/// Position the view in the center of the area.
	/// </summary>
	void CenterView();

	/// <summary>
	/// Set the zoom so that the view includes the whole area
	/// </summary>
	void ZoomToArea();

	/// <summary>
	/// Calculate and return the two rectangle representing the minimap, offset for left-bottom display
	/// </summary>
	/// <param name="out_miniarea">Return the outer rect</param>
	/// <param name="out_miniview">Return the inner rect</param>
	void GetMinimapInfo(Rect& out_miniarea, Rect& out_miniview);

private:
	void ApplyZoom() { Zoom(zoom); }

	Size area;
	Rect view;
	Rect zoomed_view;
	float zoom = 1.0f;
};
