#pragma once

class Lvs : public AppBasic {
public:
	void setup();
	void update();
	void draw();
	void shutdown();
//	void guiEvent( ciUIEvent *event );
	void prepareSettings( Settings *settings );
    void mouseDown ( MouseEvent event );
    void mouseMove ( MouseEvent event );
    void mouseUp ( MouseEvent event );
    void mouseDrag ( MouseEvent event );
    void keyDown( KeyEvent event );
    //void resize	( ResizeEvent event ) { };
private:
    LVSEngine *engine;
};
