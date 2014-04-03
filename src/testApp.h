#pragma once

#include "ofMain.h"
#include "ofxOMXPlayer.h"
#include "ofxAvahiClientService.h"
#include "ofxOsc.h"


#define PORT 6666
#define NUM_MSG_STRINGS 20

//class testApp : public ofBaseApp{
class testApp : public ofBaseApp, public ofxOMXPlayerListener{

	public:

		void setup();
		void update();
		void draw();
			
		ofxOMXPlayer omxPlayer;
		ofxAvahiClientService service;
		
		ofxOscReceiver receiver;

		int current_msg_string;
		string msg_strings[NUM_MSG_STRINGS];			
		float timers[NUM_MSG_STRINGS];
		
		void onVideoEnd(ofxOMXPlayerListenerEventData& e);
		void onVideoLoop(ofxOMXPlayerListenerEventData& e){ /*empty*/ };
		vector<ofFile> files;
				int videoCounter;
				ofxOMXPlayerSettings settings;
				
		void loadNextMovie();
		
		
	
};

