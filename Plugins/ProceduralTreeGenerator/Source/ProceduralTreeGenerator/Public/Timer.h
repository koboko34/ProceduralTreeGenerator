#pragma once

#include <chrono>

#include "Containers/UnrealString.h"
#include "Logging/LogMacros.h"

class Timer
{
public:
	Timer(const FString& Name)
	{
		Start = std::chrono::high_resolution_clock::now();
		this->Name = Name;
	}

	~Timer()
	{
		End = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> Duration = End - Start;

		UE_LOG(LogTemp, Warning, TEXT("%s took %f seconds."), *Name, Duration.count());
	}

private:
	FString Name;
	std::chrono::steady_clock::time_point Start;
	std::chrono::steady_clock::time_point End;
};
