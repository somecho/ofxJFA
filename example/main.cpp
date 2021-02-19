#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofGLFWWindowSettings setting;
	setting.setSize(1280, 720);
	setting.setGLVersion(4, 6);
	ofCreateWindow(setting);

	ofRunApp(new ofApp());

}
