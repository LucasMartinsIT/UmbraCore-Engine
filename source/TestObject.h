#pragma once
#include<eng.h>
#include<memory>

class TestObject : public eng::GameObject
{
public:
	TestObject();

	void Update(float deltaTime) override;

private:
};