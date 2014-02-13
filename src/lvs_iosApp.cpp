#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "engine/Common.h"
#include "engine/LVSEngine.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class Lvs : public AppNative {
  public:
	void setup();
    void mouseDown ( MouseEvent event );
    void mouseMove ( MouseEvent event );
    void mouseUp ( MouseEvent event );
    void mouseDrag ( MouseEvent event );
	void update();
	void draw();
    void prepareSettings( Settings *settings );
    void keyDown( KeyEvent event );
  private:
    LVSEngine *engine;
};

void Lvs::prepareSettings( Settings *settings )
{
    Window::Format fmt;
    fmt.setTitle( GAME_NAME );
    fmt.setSize( PLAYFIELD_W, PLAYFIELD_H );
    fmt.setResizable(false);
    settings->prepareWindow( fmt );
    
    //settings->setWindowSize( PLAYFIELD_W , PLAYFIELD_H );
    settings->setFrameRate( 30.0f );
    //settings->setResizable(false);
    settings->enableHighDensityDisplay();
    settings->enableMultiTouch( false );
    //settings->setTitle( GAME_NAME );
}


void Lvs::setup()
{
    gl::disableVerticalSync();
    gl::disableDepthRead();
    gl::disableDepthWrite();
    
    gl::enable( GL_LINE_SMOOTH );
    gl::enable(GL_BLEND);
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    gl::enableAlphaBlending();
    
    glLineWidth( toPixels(1.7f) );
    
    Configuration *config = new Configuration();
    config->tileSize = 50.0f;
    config->tileUpperLimit = 80.0f;
    config->tileGrid = 120.0f;
    config->toolbarHeight = 75;
    config->tileBorderSpacing = 10.0f;
    config->worldWidth = 5; // 5
    config->worldHeight = 13;
    config->padding_x = 40.0f;
    config->padding_y = 20.0f;
    config->numTileTypes = TILE_TYPES;
    config->solverDelayFrames = 2;
    
	// App::addAssetDirectory("content");
    
    engine = new LVSEngine();
    engine->setup(this, config);
}

void Lvs::mouseDown( MouseEvent event ) { engine->mouseDown ( event ); }
void Lvs::mouseMove( MouseEvent event ) { engine->mouseMove ( event ); }
void Lvs::mouseUp  ( MouseEvent event ) { engine->mouseUp   ( event ); }
void Lvs::mouseDrag( MouseEvent event ) { engine->mouseDrag ( event ); }

void Lvs::update() { engine->update(); }
void Lvs::draw() { engine->draw(); }
void Lvs::keyDown( KeyEvent event ) { engine->keyDown(event); }

CINDER_APP_NATIVE( Lvs, RendererGl )
