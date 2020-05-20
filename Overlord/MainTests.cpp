#include "CaptureWnd.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "CaptureDialog.hpp"
#include "CompareDialog.hpp"

void RecaptureCompareTest()
{
	CaptureSample cs1, cs2;
	cs1 = CaptureWnd::Capture();
	bool result = CaptureDialog::ShowDialog(&cs1);

	if(result) {
		cs2 = CaptureWnd::Recapture(cs1);
		result = CaptureDialog::ShowDialog(&cs2);
	}

	if(result) {
		CompareDialog::ShowDialog(cs1.image, cs2.image);
	}
}
