# ofxJFA

A simple Openframeworks addon for doing Jumpflood algorithm on GLSL. It takes in a source texture and outputs a JFA encoded texture. 

It is based on Ryan Kaplan's article, which you can read [here](https://www.rykap.com/graphics/skew/2016/02/25/voronoi-diagrams/). It, in turn, is based on [this paper](https://www.comp.nus.edu.sg/~tants/jfa/i3d06.pdf).

# Usage
### Include
```cpp
#include "ofxJFA.h"
```
### Declare and Init
```cpp
//ofApp.h
ofxJfa jfa;

//ofApp.cpp 
jfa.init(1280,720); //width and height of source
```
### Update 
```cpp
jfa.update(sourceTexture); //pass in source texture
```
### Drawing
The addon only does JFA encoding. It encodes the XY coordinates to the nearest seed and the distance to it in the RGB channels of a `GL_RGBA32F` texture. You need to write your own shader to render the data. Since it is a `GL_RGBA32F` shader, the coordinates and distance are not normalized in the encoding. To make use of the JFA encoding, pass the JFA data in to your own texture as well as the source texture. 
```cpp
shader.setUniformTexture("jfa", jfa.getTexture(), 0);
shader.setUniformTexture("src", sourceTexture(), 1);
```
In your shader, you can then access the data.
```glsl
vec4 loc = texture(jfa, gl_FragCoord.xy); //GET THE ENCODED DATA
gl_FragColor = texture(src, loc.xy); //USE ENCODED DATA TO GET COLOR DATA FROM SOURCE
```

