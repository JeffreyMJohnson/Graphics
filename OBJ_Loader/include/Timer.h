#pragma once

struct Timer
{
	float DeltaTime = 0;
	float CurrentTime = 0;
	float LastTime = 0;

	
	float Update(float currentTime)
	{
		CurrentTime = currentTime;
		DeltaTime = CurrentTime - LastTime;
		LastTime = CurrentTime;
		return DeltaTime;
	}
};
