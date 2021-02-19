#pragma once 
#include "ofMain.h"

class ofxJFA {
private:
	int numPasses = 11;
	float width, height;
	ofShader initShader, jumpfloodShader;
	ofFbo jfaData; 
public:
	void init(float w, float h);
	string getVertexShader();
	string getInitShader();
	string getJumpfloodShader();

	void update(ofTexture& src);
	ofTexture& getTexture();
	void drawQuad();
};