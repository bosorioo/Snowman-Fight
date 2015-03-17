#ifndef _GUI_COMPONENT_H
#define _GUI_COMPONENT_H

enum GuiComponentAnswer {
	GUI_ANS_PROPAGATE,
	GUI_ANS_STOP
};

class GuiComponent
{
public:

    virtual ~GuiComponent() {};
	virtual GuiComponentAnswer onKeyDown(unsigned char key) = 0;
	virtual GuiComponentAnswer onKeyUp(unsigned char key) = 0;
	virtual GuiComponentAnswer onSpecialKeyDown(int key) = 0;
	virtual GuiComponentAnswer onSpecialKeyUp(int key) = 0;
	virtual GuiComponentAnswer onMouseMove(int x, int y) = 0;
	virtual GuiComponentAnswer onMouseClick(int button, int state, int x, int y) = 0;
	virtual void onResize(int width, int height) = 0;
	virtual void onUpdate(float interval) = 0;
	virtual void onDraw() = 0;
};

class GuiComponentAdapter : public GuiComponent
{
public:

    ~GuiComponentAdapter() {};
	virtual GuiComponentAnswer onKeyDown(unsigned char key) {return GUI_ANS_PROPAGATE;};
	virtual GuiComponentAnswer onKeyUp(unsigned char key) {return GUI_ANS_PROPAGATE;};
	virtual GuiComponentAnswer onSpecialKeyDown(int key) {return GUI_ANS_PROPAGATE;};
	virtual GuiComponentAnswer onSpecialKeyUp(int key) {return GUI_ANS_PROPAGATE;};
	virtual GuiComponentAnswer onMouseMove(int x, int y) {return GUI_ANS_PROPAGATE;};
	virtual GuiComponentAnswer onMouseClick(int button, int state, int x, int y) {return GUI_ANS_PROPAGATE;};
	virtual void onResize(int width, int height) {};
	virtual void onUpdate(float interval) {};
	virtual void onDraw() {};
};


#endif
