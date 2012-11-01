#include "Common.h"
#include "LVSEngine.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class LeavsApp : public AppBasic {
public:
	void setup();
	void update();
	void draw();
	void shutdown();
	void guiEvent( ciUIEvent *event );
	void prepareSettings( Settings *settings );
    void mouseDown ( MouseEvent event );
    void mouseMove ( MouseEvent event );
    void keyDown( KeyEvent event );
private:
    LVSEngine *engine;
};

void LeavsApp::prepareSettings( Settings *settings )
{
    settings->setWindowSize( 950 , 650 );
    settings->setFrameRate( 60.0f );
}

void LeavsApp::setup()
{
    // setFullScreen(true);

    gl::enable( GL_LINE_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );

    engine = new LVSEngine();
    engine->setup();
}

void LeavsApp::shutdown()
{
    engine->shutdown();
    delete engine;
}

void LeavsApp::mouseMove( MouseEvent event ) {
    engine->mouseMove ( event );
}

void LeavsApp::update()
{
	engine->update();
}

void LeavsApp::draw()
{
    float lightness = sin(getElapsedSeconds() / 10);
	gl::clear( Color( 0, 0, lightness * 0.4 ) ); 
    engine->draw();
}

void LeavsApp::keyDown( KeyEvent event ) {
    if( event.getCode() == KeyEvent::KEY_ESCAPE ) {
        exit(0);
    }
}

void LeavsApp::mouseDown( MouseEvent event ) {
    engine->mouseDown(event);
}

CINDER_APP_BASIC( LeavsApp, RendererGl )
