
#ifndef __MechWalkTest_h_
#define __MechWalkTest_h_

#include "BaseApplication.h"
#include "Mech.h"
#include "OpponentMech.h"
#include <stdio.h>
#include <math.h>
#if defined(WIN32)
#include <windows.h>
#include <conio.h>
inline void sleepSomeTime() { Sleep(100); }
#else
#include "../common/conio.h"
#endif

#include "MyFileFactory.h"
#include "irrKlang.h"
using namespace irrklang;

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

class MechWalkTest : public BaseApplication
{
public:
    MechWalkTest(void);
    virtual ~MechWalkTest(void);

protected:
	virtual void createFrameListener(void);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);	
    virtual void createScene(void);
	virtual void createCamera(void);
	virtual void createViewports(void);
	virtual bool keyPressed(const OIS::KeyEvent &arg);
	bool mouseMoved(const OIS::MouseEvent &arg);
	bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	Ogre::Real mOrbitRadius;
	Ogre::Real mOrbitIncrementRadians;
	Ogre::Real mCameraAngle;
	Ogre::Real mPlaneSize;
	Mech* mMech;
	OpponentMech* mOpponent;
	Ogre::SceneNode* mSculptureNode;
	OgreBites::ParamsPanel* mSpeedPanel;
	OgreBites::ParamsPanel* mResultPanel;
	// start the sound engine with default parameters
	ISoundEngine* engine;
	ISound* music;

private:
	bool processUnbufferedInput(const Ogre::FrameEvent& evt);
	void addSpotlight(const Ogre::String name, const Ogre::Real xPos, const Ogre::Real zPos);
	void showResult(Ogre::String result);
};

#endif // #ifndef __MechWalkTest_h_
