#include "Engine.hpp"
#include "Time-Slip.hpp"

int main()
{
	Engine::Initialize("Time-Slip", MakeVersionNumber(0, 1, 0), TimeSlip::Initialize, TimeSlip::Update, TimeSlip::Shutdown);
}