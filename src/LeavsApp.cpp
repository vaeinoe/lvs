#include "engine/Common.h"
#include "engine/LVSEngine.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class LeavsApp : public AppBasic {
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
    void resize	( ResizeEvent event ) { };
private:
    LVSEngine *engine;
};

void LeavsApp::prepareSettings( Settings *settings )
{
    settings->setWindowSize( PLAYFIELD_W , PLAYFIELD_H );
    settings->setFrameRate( 30.0f );
    settings->setResizable(false);
    settings->setTitle("ASDF");
}

void LeavsApp::setup()
{
    gl::disableVerticalSync();
    gl::disableDepthRead();
    gl::disableDepthWrite();

    // setFullScreen(true);

    gl::enable( GL_LINE_SMOOTH );
    gl::enable(GL_BLEND);
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
    config->numTileTypes = TILE_TYPES;
    config->solverDelayFrames = 2;
    
    for (int i = 0; i < config->numTileTypes; i++) {
        config->tileLevels[i] = 0;
    }

    engine = new LVSEngine();
    engine->setup(config);    
}

void LeavsApp::shutdown()
{
    engine->shutdown();
    delete engine;
}

void LeavsApp::mouseDown( MouseEvent event ) { engine->mouseDown ( event ); }
void LeavsApp::mouseMove( MouseEvent event ) { engine->mouseMove ( event ); }
void LeavsApp::mouseUp  ( MouseEvent event ) { engine->mouseUp   ( event ); }
void LeavsApp::mouseDrag( MouseEvent event ) { engine->mouseDrag ( event ); }


void LeavsApp::update() { engine->update(); }
void LeavsApp::draw() { engine->draw(); }
void LeavsApp::keyDown( KeyEvent event ) { engine->keyDown(event); }

CINDER_APP_BASIC( LeavsApp, RendererGl )
