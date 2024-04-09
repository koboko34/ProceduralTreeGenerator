// Armand Yilinkou 2024


#include "RandomNumberGenerator.h"

URandomNumberGenerator::URandomNumberGenerator()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void URandomNumberGenerator::Init()
{
	Engine = std::default_random_engine(RandomSeed);
}

int URandomNumberGenerator::GenerateNumber()
{
	return Distribution(Engine);
}
