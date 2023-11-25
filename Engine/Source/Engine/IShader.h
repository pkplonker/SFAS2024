
#pragma once

class IShader
{
public:
	virtual ~IShader() = default;

	IShader() = default;
	virtual void Update() = 0;

protected: 
	
};

