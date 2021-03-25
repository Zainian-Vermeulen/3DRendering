#pragma once

class Time
{
public:
	int init();
	void update();
	void deInit();

	float getDeltaTime() const {return _deltaTime;}

private:
	float _deltaTime = 0.0f;
	float _lastTimestamp = 0.0f;
	float _totalTime = 0.0f;
};

