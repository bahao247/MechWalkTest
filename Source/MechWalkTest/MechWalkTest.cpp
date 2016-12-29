#include "MechWalkTest.h"

//-------------------------------------------------------------------------------------
MechWalkTest::MechWalkTest(void):
	mCameraAngle(0.0f),
	mOrbitRadius(150.0f),
	mOrbitIncrementRadians(Ogre::Math::PI/250),
	mPlaneSize(200)
{
}
//-------------------------------------------------------------------------------------
MechWalkTest::~MechWalkTest(void)
{
}

void MechWalkTest::createViewports(void)
{
    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));    
}

void MechWalkTest::createCamera(void)
{
    // create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");
    // set its position, direction  
	mCamera->setPosition(Ogre::Vector3(mOrbitRadius*Ogre::Math::Cos(mCameraAngle),200,mOrbitRadius*Ogre::Math::Sin(mCameraAngle)));
    mCamera->lookAt(Ogre::Vector3(0,0,0));
    // set the near clip distance
    mCamera->setNearClipDistance(5);
 
    mCameraMan = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
}

//-------------------------------------------------------------------------------------
void MechWalkTest::createScene(void)
{

	Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
    Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(16);

	mMech = new Mech("Mech", mSceneMgr, mPlaneSize);
	mOpponent = new OpponentMech("OpponentMech", mSceneMgr, mPlaneSize);

	Ogre::Entity* sculptureEntity = mSceneMgr->createEntity("Sculpture", "Sculpture.mesh");	
	Ogre::AxisAlignedBox sculptureBox = sculptureEntity->getBoundingBox();
    mSculptureNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    mSculptureNode->attachObject(sculptureEntity);
	mSculptureNode->setScale(4.0f, 17.0f, 4.0f);
	mSculptureNode->setPosition(0, -sculptureBox.getCorner(Ogre::AxisAlignedBox::FAR_LEFT_BOTTOM).y*17.0f, 0);
	sculptureEntity->setCastShadows(true);
 
    // Set ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.0, 0.0, 0.0));
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
	addSpotlight("spotLight1", 250.0, 0);
	addSpotlight("spotLight2", 0, -250.0);
	addSpotlight("spotLight3", 0, 250.0);
	addSpotlight("spotLight4", -250.0, 0);

	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
 
    Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane, mPlaneSize, mPlaneSize, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
 
    Ogre::Entity* entGround = mSceneMgr->createEntity("GroundEntity", "ground");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround);
 
    entGround->setMaterialName("Examples/BumpyMetal");
    entGround->setCastShadows(false);
}

void MechWalkTest::addSpotlight(const Ogre::String name, const Ogre::Real xPos, const Ogre::Real zPos) 
{
	Ogre::Light* spotLight = mSceneMgr->createLight(name);
    spotLight->setType(Ogre::Light::LT_SPOTLIGHT);
    spotLight->setDiffuseColour(1.0, 1.0, 1.0);
    spotLight->setSpecularColour(1.0, 1.0, 1.0);
	spotLight->setDirection(-xPos/xPos, -1, -zPos/zPos);
    spotLight->setPosition(xPos, 250.0, zPos);
	spotLight->setAttenuation(500.0f, 0.5f, 0.007f, 0.0f);
	spotLight->setSpotlightRange(Ogre::Degree(180), Ogre::Degree(180));
}

bool MechWalkTest::processUnbufferedInput(const Ogre::FrameEvent& evt)
{
	if (mTrayMgr->isDialogVisible()) return true;   // don't process any more keys if dialog is up

	if (mKeyboard->isKeyDown(OIS::KC_Z)) {
		if (mCameraAngle==2*Ogre::Math::PI) mCameraAngle = 0;
		mCameraAngle+=mOrbitIncrementRadians;
		mCamera->setPosition(Ogre::Vector3(mOrbitRadius*Ogre::Math::Cos(mCameraAngle),200,mOrbitRadius*Ogre::Math::Sin(mCameraAngle)));
		mCamera->yaw(Ogre::Radian(-mOrbitIncrementRadians));
	} else if (mKeyboard->isKeyDown(OIS::KC_X)) {		
		if (mCameraAngle==0) mCameraAngle = 2*Ogre::Math::PI;
		mCameraAngle-=mOrbitIncrementRadians;
        mCamera->setPosition(Ogre::Vector3(mOrbitRadius*Ogre::Math::Cos(mCameraAngle),200,mOrbitRadius*Ogre::Math::Sin(mCameraAngle)));
		mCamera->yaw(Ogre::Radian(mOrbitIncrementRadians));
    }

	if (mKeyboard->isKeyDown(OIS::KC_UP)) {
		mMech->accelerate();
	} else if (mKeyboard->isKeyDown(OIS::KC_DOWN)) {
		mMech->decelerate();
	}

	if (mKeyboard->isKeyDown(OIS::KC_RIGHT)) {
		mMech->turnRight();
	} else if (mKeyboard->isKeyDown(OIS::KC_LEFT))	{
		mMech->turnLeft();
	}

	if (mKeyboard->isKeyDown(OIS::KC_SPACE)) {
		mMech->fireLaser(mOpponent);
	} 

	if (mKeyboard->isKeyDown(OIS::KC_ESCAPE)) {
        mShutDown = true;
    }
    return true;
}

void MechWalkTest::createFrameListener(void)
{
	BaseApplication::createFrameListener();

	Ogre::StringVector items;
    items.push_back("Speed");
    mSpeedPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "speed", 200, items);
    mTrayMgr->moveWidgetToTray(mSpeedPanel, OgreBites::TL_TOPLEFT, 0);
    mSpeedPanel->show();
	mTrayMgr->toggleAdvancedFrameStats();
}

bool MechWalkTest::keyPressed( const OIS::KeyEvent &arg )
{
	if (arg.key == OIS::KC_SYSRQ) {
        mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
    }
	return true; //BaseApplication::keyPressed(arg);
}

//-------------------------------------------------------------------------------------
bool MechWalkTest::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if (!mMech->isActive()) {
		showResult("You Lose!");
	} else if (!mOpponent->isActive()) {
		showResult("You Win!");
	}
	mMech->move(evt.timeSinceLastFrame);
	mOpponent->move(evt.timeSinceLastFrame, mMech);
	mSpeedPanel->setParamValue(0, Ogre::StringConverter::toString(mMech->getSpeed()));
    bool ret = BaseApplication::frameRenderingQueued(evt);
	if (!processUnbufferedInput(evt)) return false;
    return ret;
}

void MechWalkTest::showResult(Ogre::String result)
{
	if (mTrayMgr->getWidget("result")==0) {
		Ogre::StringVector items2;
		items2.push_back("Result");
		mResultPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "result", 200, items2);
		mTrayMgr->moveWidgetToTray(mResultPanel, OgreBites::TL_CENTER, 0);
		mResultPanel->setParamValue(0, result);
		mResultPanel->show();
	}
}

bool MechWalkTest::mouseMoved(const OIS::MouseEvent &arg) {return true;}
bool MechWalkTest::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {return true;}
bool MechWalkTest::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {return true;}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        MechWalkTest app;

        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif
