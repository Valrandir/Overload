#include "DemoDialog.hpp"
#include "WindowGdi.hpp"
#include "DemoUserControl.hpp"

int ScenarioUserControl()
{
	//DemoDialog::ShowDialog();

	WindowGdi wnd(TEXT("User Control"), 300, 400);
	wnd.Show();

	DemoUserControl demo_user_control[3];

	int w{}, h{};
	int y = 5;

	for(auto& d : demo_user_control) {
		d.Initialize(wnd.GetHandle(), 15, y += h + 10);
		d.GetSize(w, h);
	}

	wnd.UpdateLoop();

	return 0;
}
