#pragma once

class MyTestShader
{
public:
	MyTestShader(unsigned int* programID);
	~MyTestShader();
	void doShaderStuff();
private:
	unsigned int* m_programID;
};