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
    settings->setFrameRate( 30.0f );
}

void LeavsApp::setup()
{
    gl::disableVerticalSync();
    gl::disableDepthRead();
    gl::disableDepthWrite();

    // setFullScreen(true);

    gl::enable( GL_LINE_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    
    Configuration *config = new Configuration();
    config->tileSize = 50.0f;
    config->tileUpperLimit = 80.0f;
    config->tileGrid = 120.0f;
    config->toolbarHeight = 75;
    config->tileBorderSpacing = 10.0f;
    config->worldWidth = 5;
    config->worldHeight = 11;
    config->padding = 10.0f;
    config->numTileTypes = 3;
    config->solverDelayFrames = 2;

    engine = new LVSEngine();
    engine->setup(config);
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
    engine->draw();
}

void LeavsApp::keyDown( KeyEvent event ) {
    engine->keyDown(event);
}

void LeavsApp::mouseDown( MouseEvent event ) {
    engine->mouseDown(event);
}

CINDER_APP_BASIC( LeavsApp, RendererGl )
