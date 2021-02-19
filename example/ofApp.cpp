#include "ofApp.h"

ofShader shader;
ofFbo src;
ofxJFA jfa;

void ofApp::setup(){
	jfa.init(ofGetWidth(), ofGetHeight());
	src.allocate(ofGetWidth(), ofGetHeight());
	shader.setupShaderFromSource(GL_VERTEX_SHADER, R"(
		#version 460
		uniform mat4 modelViewProjectionMatrix;
		in vec4 position;
		void main(){
			gl_Position = modelViewProjectionMatrix * position;
		}
	)");

	shader.setupShaderFromSource(GL_FRAGMENT_SHADER, R"(
		#version 460
		uniform sampler2DRect jfa;
		uniform sampler2DRect src;
		uniform float t;

		void main(){
			vec2 uv = gl_FragCoord.xy;
			vec4 loc = texture(jfa, uv);
			vec4 col = texture(src, loc.xy);
			col.rgb *= sin(t + floor(loc.x*0.01) * 2.) * .5 +.5;
			gl_FragColor =  col;
		}
	)");
	shader.bindDefaults();
	shader.linkProgram();


	src.begin();
	ofSetColor(255);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	for (int i = 0; i < 20; i++) {
		ofSetColor(ofColor(ofRandom(255), ofRandom(255), ofRandom(255)));
		ofDrawCircle(ofRandomWidth(), ofRandomHeight(), 5);
	}
	src.end();

	jfa.update(src.getTexture());
}

void ofApp::update(){

}

void ofApp::draw(){
	shader.begin();
	shader.setUniformTexture("jfa", jfa.getTexture(), 0);
	shader.setUniformTexture("src", src.getTexture(), 1);
	shader.setUniform1f("t", ofGetElapsedTimef());
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	shader.end();
}
