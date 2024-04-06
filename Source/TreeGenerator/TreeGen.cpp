// Armand Yilinkou 2024


#include "TreeGen.h"

ATreeGen::ATreeGen()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	LSystem = CreateDefaultSubobject<ULSystem>("LSystem");
}
