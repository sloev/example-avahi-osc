#include "testApp.h"



bool doLoadNextMovie = false;
void testApp::onVideoEnd(ofxOMXPlayerListenerEventData& e)
{
	ofLogVerbose(__func__) << " RECEIVED";
	doLoadNextMovie = true;
}


unsigned long long skipTimeStart=0;
unsigned long long skipTimeEnd=0;
unsigned long long amountSkipped =0;
unsigned long long totalAmountSkipped =0;

//--------------------------------------------------------------
void testApp::setup()
{
	service.start("openframeworks", "_osc._udp", PORT);
	receiver.setup(PORT);
	current_msg_string = 0;
	
	//ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetVerticalSync(false);
	
	ofDirectory currentVideoDirectory(ofToDataPath("../../../video", true));
		if (currentVideoDirectory.exists()) 
		{
			currentVideoDirectory.listDir();
			currentVideoDirectory.sort();
			files = currentVideoDirectory.getFiles();
			if (files.size()>0) 
			{
				videoCounter = 0;
				settings.videoPath = files[videoCounter].path();
				settings.useHDMIForAudio = true;	//default true
				settings.enableLooping = false;		//default true
				settings.enableTexture = true;		//default true
				settings.listener = this;			//this app extends ofxOMXPlayerListener so it will receive events ;
				omxPlayer.setup(settings);
			}		
		}
/*	string videoPath = ofToDataPath("../../../video/Timecoded_Big_bunny_1.mov", true);
	
	//Somewhat like ofFboSettings we may have a lot of options so this is the current model
	ofxOMXPlayerSettings settings;
	settings.videoPath = videoPath;
	settings.useHDMIForAudio = true;	//default true
	settings.enableTexture = true;		//default true
	settings.enableLooping = true;		//default true
	settings.enableAudio = true;		//default true, save resources by disabling
	//settings.doFlipTexture = true;		//default false
	
	
	//so either pass in the settings
	omxPlayer.setup(settings);
	
	//or live with the defaults
	//omxPlayer.loadMovie(videoPath);
	
	*/
	
}


void testApp::loadNextMovie()
{
	if(videoCounter+1<files.size())
	{
		videoCounter++;
	}else
	{
		videoCounter = 0;
	}
	skipTimeStart = ofGetElapsedTimeMillis();
	omxPlayer.loadMovie(files[videoCounter].path());
	skipTimeEnd = ofGetElapsedTimeMillis();
	amountSkipped = skipTimeEnd-skipTimeStart;
	totalAmountSkipped+=amountSkipped;
}

//--------------------------------------------------------------
void testApp::update()
{
		// check for waiting messages
			while(receiver.hasWaitingMessages()){
				// get the next message
				ofxOscMessage m;
				receiver.getNextMessage(&m);

				// check for mouse moved message
				if(m.getAddress() == "/pause"){
					// both the arguments are int32's
					//tmp = m.getArgAsInt32(0);
					omxPlayer.setPaused(m.getArgAsInt32(0)==1);
				}
				if(m.getAddress() == "/step"){
					// both the arguments are int32's
					//tmp = m.getArgAsInt32(0);
					if(m.getArgAsInt32(0)==1){
						omxPlayer.stepFrameForward();
					}
				}
				if(m.getAddress() == "/loadNext"){
					// both the arguments are int32's
					//tmp = m.getArgAsInt32(0);
					if(m.getArgAsInt32(0)==1){
						loadNextMovie();
					}
				}

			}
			
}


//--------------------------------------------------------------
void testApp::draw(){

	if(!omxPlayer.isTextureEnabled)
	{
		return;
	}
	
	omxPlayer.draw(0, 0, ofGetWidth(), ofGetHeight());
	
	//draw a smaller version in the lower right
	int scaledHeight	= omxPlayer.getHeight()/4;
	int scaledWidth		= omxPlayer.getWidth()/4;
	omxPlayer.draw(ofGetWidth()-scaledWidth, ofGetHeight()-scaledHeight, scaledWidth, scaledHeight);

	ofDrawBitmapStringHighlight(omxPlayer.getInfo(), 60, 60, ofColor(ofColor::black, 90), ofColor::yellow);
}


