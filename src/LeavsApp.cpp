#include "engine/Common.h"
#include "engine/LVSEngine.h"
#include "LeavsApp.h"

using namespace ci;
using namespace ci::app;
using namespace std;

void Lvs::prepareSettings( Settings *settings )
{
    settings->setWindowSize( PLAYFIELD_W , PLAYFIELD_H );
    settings->setFrameRate( 30.0f );
    settings->setResizable(false);
    settings->enableHighDensityDisplay();
}

void Lvs::setup()
{
//Used for WIN32 ad hoc debugging
//AllocConsole();
//freopen("CONIN$", "r", stdin);
//freopen("CONOUT$", "w", stdout);
//freopen("CONOUT$", "w", stderr);

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

void Lvs::shutdown()
{
    engine->shutdown();
    delete engine;
}

void Lvs::mouseDown( MouseEvent event ) { engine->mouseDown ( event ); }
void Lvs::mouseMove( MouseEvent event ) { engine->mouseMove ( event ); }
void Lvs::mouseUp  ( MouseEvent event ) { engine->mouseUp   ( event ); }
void Lvs::mouseDrag( MouseEvent event ) { engine->mouseDrag ( event ); }


void Lvs::update() { engine->update(); }
void Lvs::draw() { engine->draw(); }
void Lvs::keyDown( KeyEvent event ) { engine->keyDown(event); }

CINDER_APP_BASIC( Lvs, RendererGl )
