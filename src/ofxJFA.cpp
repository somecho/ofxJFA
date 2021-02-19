#include "ofxJFA.h"

void ofxJFA::init(float w, float h) {
	//INITIALIZE SHADERS
	string vertShader = getVertexShader();
	initShader.setupShaderFromSource(GL_VERTEX_SHADER, vertShader);
	initShader.setupShaderFromSource(GL_FRAGMENT_SHADER, getInitShader());
	initShader.bindDefaults();
	initShader.linkProgram();

	jumpfloodShader.setupShaderFromSource(GL_VERTEX_SHADER, vertShader);
	jumpfloodShader.setupShaderFromSource(GL_FRAGMENT_SHADER, getJumpfloodShader());
	jumpfloodShader.bindDefaults();
	jumpfloodShader.linkProgram();
	

	//INITIALIZE PARAMS
	width = w;
	height = h;

	//ALLOCATE BUFFERS
	jfaData.allocate(w, h, GL_RGBA32F);
}

string ofxJFA::getVertexShader() {
	return R"(
		#version 460
		uniform mat4 modelViewProjectionMatrix;
		in vec4 position; 

		void main(){
		gl_Position = modelViewProjectionMatrix * position;
		}
	)";
}

string ofxJFA::getInitShader() {
	return R"(
		#version 460

		uniform sampler2DRect src;

		vec4 empty = vec4(1.,1.,1.,1.);


		void main(){
			vec2 uv = gl_FragCoord.xy;
			vec4 col = texture(src,uv);
			vec4 outcol;
			//CHECK IF SEED
			if(col != empty){
				//store coordinates if not empty
				outcol = vec4(uv.x,uv.y,0.,1.);
			} else {
				outcol = empty;
			}
			gl_FragColor = outcol;
		}
	)";
}

string ofxJFA::getJumpfloodShader() {
	return R"(
		#version 460

		uniform sampler2DRect src;
		uniform int pass;
		uniform int numPasses;
		
		vec4 EMPTY = vec4(1.,1.,1.,1.);
		
		void main(){
			vec2 uv = vec2(gl_FragCoord.xy);
			vec4 outCol = EMPTY;
			int stepsize = int(exp2(numPasses-pass));
			float minDist = 9999.;
			for(int y = -1; y <= 1; y++){
				for(int x = -1; x <=1; x++){
					vec2 duv = vec2(x,y) * stepsize;
					vec4 col = texture(src, uv+duv);
					if(col.x != 0.0 && col.y != 0.0){
						float d = length(col.xy-uv);
						if(d < minDist){
							minDist = d;
							outCol = vec4(col.xy,d,1.);
						}
					}
				}
			}
			gl_FragColor = outCol;
		}
	)";
}

void ofxJFA::update(ofTexture& src) {
	//INIT STEP
	jfaData.begin();
	initShader.begin();
	initShader.setUniformTexture("src", src, 0);
	drawQuad();
	initShader.end();
	jfaData.end();

	for (int i = 0; i < numPasses; i++) {
		jfaData.begin();
		jumpfloodShader.begin();
		jumpfloodShader.setUniformTexture("src", jfaData.getTexture(), 0);
		jumpfloodShader.setUniform1i("numPasses", numPasses);
		jumpfloodShader.setUniform1i("pass", i);
		drawQuad();
		jumpfloodShader.end();
		jfaData.end();
	}
}

ofTexture& ofxJFA::getTexture() {
	return jfaData.getTexture();
}

void ofxJFA::drawQuad() {
	ofDrawRectangle(0, 0, width, height);

}
