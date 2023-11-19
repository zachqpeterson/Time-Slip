#include "Engine.hpp"
#include "Timeslip.hpp"

int main()
{
	Engine::Initialize("Timeslip", MakeVersionNumber(0, 1, 0), Timeslip::Initialize, Timeslip::Update, Timeslip::Shutdown);
}