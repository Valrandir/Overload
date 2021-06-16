#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <exception>
#include "Random.hpp"

extern int ScenarioAreaView();
extern int ScenarioCaptureCompare();
extern int ScenarioPathOfExile();
extern int ScenarioSamplerDialog();
int (*CurrentScenario)() = ScenarioPathOfExile;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ INT nShowCmd)
{
	Random::Randomize();

	try {
		return CurrentScenario();
	} catch(std::exception& ex) {
		MessageBoxA(0, ex.what(), "Critical Exception", MB_OK | MB_ICONERROR);
	}

	return 1;
}
