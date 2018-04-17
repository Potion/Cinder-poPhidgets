#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "poPhidgets/PhidgetDistanceSensor.h"

#define CI_MIN_LOG_LEVEL 4

using namespace ci;
using namespace ci::app;
using namespace std;

class DistanceSensorApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
    void keyDown(KeyEvent event) override;
	void update() override;
	void draw() override;
    
    
    po::phidget::DistanceSensorRef mDistanceInput;
};

void DistanceSensorApp::setup()
{
    mDistanceInput = po::phidget::DistanceSensor::create();
    mDistanceInput->setProperties(-1, 0, 100, 0, 3);
    mDistanceInput->createAndOpenChannel();
}

void DistanceSensorApp::mouseDown( MouseEvent event )
{
}

void DistanceSensorApp::keyDown( KeyEvent event )
{
    if (event.getChar() == 'q' || event.getChar() == 'Q') {
        quit();
    }
}

void DistanceSensorApp::update()
{
}

void DistanceSensorApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( DistanceSensorApp, RendererGl )
