#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "network/SocketIO.h"
#include "spine/Json.h"

#include "picojson.h"


#include "cocostudio/CocoStudio.h"
#include "gui/CocosGUI.h"

#include <stdio.h>      /* puts, printf */
#include <time.h>


using namespace cocostudio;
using namespace gui;

USING_NS_CC;
USING_NS_CC_EXT;

class HelloWorld : public cocos2d::Layer, public network::SocketIO::SIODelegate
{
public:
    HelloWorld(void);
    virtual ~HelloWorld();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    virtual void onConnect(network::SIOClient* client);
    virtual void onMessage(network::SIOClient* client, const std::string& data);
    virtual void onClose(network::SIOClient* client);
    virtual void onError(network::SIOClient* client, const std::string& data);
    network::SIOClient *_sioClient, *_sioEndpoint;
    
    void battleCastEvent(network::SIOClient *client, const std::string& data);
    
	void echotest(network::SIOClient *client, const std::string& data);
    void nodeServerCon();
    
 
    // a selector callback
    void menuCloseCallback(Object* pSender);
    void menuAttackCallback(cocos2d::Object *sender,TouchEventType type);
    
    void setCoolDownEffect(Node *node,Point position);
    
    void skillCoolDownCallBack(CCNode* node);
    
private:
    UILayout* ui_targetStatus;
    
    
    UILayout* ui_seq_1;
    UILayout* ui_seq_2;
    UILayout* ui_seq_3;
    UILayout* ui_seq_4;
    UILayout* ui_seq_5;
    UILayout* ui_seq_6;
    UILayout* ui_seq_start;
    
    
    
    double castTime = 0;
    double target = 0;
    double targetGroup = 0;
    double user = 0;
    double userGroup = 0;
    
    
    
};

#endif // __HELLOWORLD_SCENE_H__
