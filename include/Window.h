#pragma once

class Window
{
public:
	Window(const std::string &title, unsigned int width, unsigned int height);
	bool isOpen() const;
	void startFrame() const;
	void endFrame() const;
	~Window();
private:
	GLFWwindow* window;
	unsigned int width, height;
};    