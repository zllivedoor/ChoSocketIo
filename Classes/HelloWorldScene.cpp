fd#include "HelloWorldScene.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace network;
using namespace extension;


static bool isClickGo;


HelloWorld::HelloWorld(void)
: _sioClient(NULL)
, _sioEndpoint(NULL)
{}

HelloWorld::~HelloWorld(void)
{
   
}

Scene* HelloWorld::createScene()
{
    isClickGo = true;
    
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    /*
    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
    
    
    auto attackButton = MenuItemImage::create(
                                           "attacka.png",
                                           "attacka.png",
                                           CC_CALLBACK_1(HelloWorld::menuAttackCallback, this));
    
	attackButton->setPosition(Point(origin.x + visibleSize.width - attackButton->getContentSize().width/2 ,
                                origin.y+ visibleSize.height - attackButton->getContentSize().height/2));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);
    
    auto menu2 = Menu::create(attackButton, NULL);
    menu2->setPosition(Point::ZERO);
    this->addChild(menu2, 1);
     */
    
    
    /*
    
    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(Point(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("bg.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    */
    
    
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    
    
    /////////////////////////////////
    // CocoStudio　の資源をついかする
    auto node = SceneReader::getInstance()->createNodeWithSceneFile("FightScene.json");
    if(node)
    {
        this->addChild(node);
    }
    
    
    /////////////////////////////////
    // Join Room 画面
    UILayer* loginLayer = UILayer::create();
    auto loginLayout = GUIReader::shareReader()->widgetFromJsonFile("ui/DemoLogin/DemoLogin.json");
    loginLayer->addWidget(loginLayout);
    loginLayer->setPosition(Point(195 ,215));
    loginLayer->setZOrder(10);
    loginLayer->setTag(10010);
    this->addChild(loginLayer);
    
    
    auto joinRoom  = (UILayout*)loginLayer->getWidgetByName("Button_43");
    joinRoom->UIWidget::addTouchEventListener(this, toucheventselector(HelloWorld::joinRoomCallback));
    
    
    
    
    
    
    
    
    /////////////////////////////////
    // Target Status情報
    auto targetStatus = node->getChildByTag(10014);
    auto ts_reader = (ComRender*)targetStatus->getComponent("GUIComponent");
    auto ts_layer = (UILayer*)ts_reader->getNode();
    ui_targetStatus = (UILayout*)ts_layer->getWidgetByName("ui_hp");
    ui_targetStatus->UIWidget::setScaleX(0.5);// remainHP/maxHP をここに設定します。
    
    
    /////////////////////////////////
    // Skill メニュー
    auto child = node->getChildByTag(10015);
    auto reader = (ComRender*)child->getComponent("GUIComponent");
    auto layer = (UILayer*)reader->getNode();
    
    ui_seq_1 = (UILayout*)layer->getWidgetByName("ui_seq_1");
    ui_seq_1->UIWidget::addTouchEventListener(this, toucheventselector(HelloWorld::menuAttackCallback));
    
    ui_seq_2 = (UILayout*)layer->getWidgetByName("ui_seq_2");
    ui_seq_2->UIWidget::addTouchEventListener(this, toucheventselector(HelloWorld::menuAttackCallback));
    
    ui_seq_3 = (UILayout*)layer->getWidgetByName("ui_seq_3");
    ui_seq_3->UIWidget::addTouchEventListener(this, toucheventselector(HelloWorld::menuAttackCallback));
    
    ui_seq_4 = (UILayout*)layer->getWidgetByName("ui_seq_4");
    ui_seq_4->UIWidget::addTouchEventListener(this, toucheventselector(HelloWorld::menuAttackCallback));
    
    ui_seq_5 = (UILayout*)layer->getWidgetByName("ui_seq_5");
    ui_seq_5->UIWidget::addTouchEventListener(this, toucheventselector(HelloWorld::menuAttackCallback));
    
    ui_seq_6 = (UILayout*)layer->getWidgetByName("ui_seq_6");
    ui_seq_6->UIWidget::addTouchEventListener(this, toucheventselector(HelloWorld::menuAttackCallback));
    
    ui_seq_start = (UILayout*)layer->getWidgetByName("ui_seq_start");
    ui_seq_start->UIWidget::addTouchEventListener(this, toucheventselector(HelloWorld::menuAttackCallback));
    
   
    //setCoolDownEffect(layer,ui_seq_2->getPosition());
    

    

   
    nodeServerCon();
    
    return true;
}
 
    
void HelloWorld::nodeServerCon()
{
    //平井さんのサーバー
    _sioClient = SocketIO::connect(*this, "ws://10.13.165.59:3000");
    
    //僕のサーバー
   // _sioClient = SocketIO::connect(*this, "ws://10.13.197.156:8080");
    
    _sioClient->on("battleCast", CC_CALLBACK_2(HelloWorld::battleCastEvent, this));
    _sioClient->on("battleExec", CC_CALLBACK_2(HelloWorld::battleExecEvent, this));
    _sioClient->on("battleStart", CC_CALLBACK_2(HelloWorld::battleStartEvent, this));
}

void HelloWorld::joinRoomCallback(cocos2d::Object *sender,TouchEventType type)
{
    std::string args = "";
    
    args="{\"join\":\"join\"}";
   
    if (type == TOUCH_EVENT_BEGAN){}
    if (type == TOUCH_EVENT_MOVED){}
    if (type == TOUCH_EVENT_ENDED)
    {
        if(_sioClient != NULL) _sioClient->emit("join",args);
    }
    
}

void HelloWorld::menuAttackCallback(cocos2d::Object *sender,TouchEventType type)
{
    std::string args = "";
    
    if (type == TOUCH_EVENT_BEGAN)
    {
        auto armature = (Armature*)getChildByTag(10005);
        armature->getAnimation()->play("attack");
        
        
        auto armatureB = (Armature*)getChildByTag(10006);
        armatureB->getAnimation()->play("death");
        
        
    }
    if (type == TOUCH_EVENT_MOVED)
    {
        
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        if(sender->isEqual(ui_seq_1)){
            setCoolDownEffect(ui_seq_1);
            args = "{\"sequence\":[{\"action\":1,\"seqId\":1},{\"action\":2,\"seqId\":2},{\"action\":3,\"seqId\":3},{\"action\":1,\"seqId\":4}],\"target\":1,\"targetGroup\":0,\"user\":123,\"userGroup\":0}";
        }else if(sender->isEqual(ui_seq_2)){
            setCoolDownEffect(ui_seq_2);
            args = "{\"sequence\":[{\"action\":1,\"seqId\":2},{\"action\":2,\"seqId\":2},{\"action\":3,\"seqId\":3},{\"action\":1,\"seqId\":4}],\"target\":1,\"targetGroup\":0,\"user\":123,\"userGroup\":0}";
        }else if(sender->isEqual(ui_seq_3)){
            setCoolDownEffect(ui_seq_3);
            args = "{\"sequence\":[{\"action\":1,\"seqId\":3},{\"action\":2,\"seqId\":2},{\"action\":3,\"seqId\":3},{\"action\":1,\"seqId\":4}],\"target\":1,\"targetGroup\":0,\"user\":123,\"userGroup\":0}";
        }else if(sender->isEqual(ui_seq_4)){
            setCoolDownEffect(ui_seq_4);
            args = "{\"sequence\":[{\"action\":1,\"seqId\":4},{\"action\":2,\"seqId\":2},{\"action\":3,\"seqId\":3},{\"action\":1,\"seqId\":4}],\"target\":1,\"targetGroup\":0,\"user\":123,\"userGroup\":0}";
        }else if(sender->isEqual(ui_seq_5)){
            setCoolDownEffect(ui_seq_5);
            args = "{\"sequence\":[{\"action\":1,\"seqId\":5},{\"action\":2,\"seqId\":2},{\"action\":3,\"seqId\":3},{\"action\":1,\"seqId\":4}],\"target\":1,\"targetGroup\":0,\"user\":123,\"userGroup\":0}";
        }else if(sender->isEqual(ui_seq_6)){
            setCoolDownEffect(ui_seq_6);
            args = "{\"sequence\":[{\"action\":1,\"seqId\":6},{\"action\":2,\"seqId\":2},{\"action\":3,\"seqId\":3},{\"action\":1,\"seqId\":4}],\"target\":1,\"targetGroup\":0,\"user\":123,\"userGroup\":0}";
        }else if(sender->isEqual(ui_seq_start)){
            
            setCoolDownEffect(ui_seq_start);
            args = "{\"sequence\":[{\"action\":1,\"seqId\":1},{\"action\":2,\"seqId\":2},{\"action\":3,\"seqId\":3},{\"action\":1,\"seqId\":4}],\"target\":1,\"targetGroup\":0,\"user\":123,\"userGroup\":0}";
            
        }
        
        if(_sioClient != NULL) _sioClient->emit("battleAction",args);
    }
        
}


void HelloWorld::menuCloseCallback(Object* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
   
}

void HelloWorld::onConnect(network::SIOClient* client)
{
	log("SocketIOTestLayer::onConnect called");
    
	std::stringstream s;
	s << client->getTag() << " connected!";
	//_sioClientStatus->setString(s.str().c_str());
    
}


void HelloWorld::onMessage(network::SIOClient* client, const std::string& data)
{
	log("SocketIOTestLayer::onMessage received: %s", data.c_str());
	
	std::stringstream s;
	s << client->getTag() << " received message with content: " << data.c_str();
	//_sioClientStatus->setString(s.str().c_str());
    
}

void HelloWorld::onClose(network::SIOClient* client)
{
	log("SocketIOTestLayer::onClose called");
    
	std::stringstream s;
	s << client->getTag() << " closed!";
	//_sioClientStatus->setString(s.str().c_str());
    
	//set the local pointer to NULL or connect to another client
	//the client object will be released on its own after this method completes
	if(client == _sioClient) {
		
		_sioClient = NULL;
	} else if(client == _sioEndpoint) {
		
		_sioEndpoint = NULL;
	}
    
    //Reconnet
    nodeServerCon();
    
}

void HelloWorld::onError(network::SIOClient* client, const std::string& data)
{
	log("SocketIOTestLayer::onError received: %s", data.c_str());
    
   	std::stringstream s;
	s << client->getTag() << " received error with content: " << data.c_str();
	//_sioClientStatus->setString(s.str().c_str());
    
    //Reconnet
    nodeServerCon();
}

//test event callback handlers, these will be registered with socket.io
void HelloWorld::battleCastEvent(SIOClient *client, const std::string& data) {
    
    //cocos2d-xでJSONを使う http://nirasan.hatenablog.com/entry/2013/10/24/232905
	log("きゃすと【battleCastEvent】: %s", data.c_str());
    std::string err;
    picojson::value val;
    
    picojson::parse(val, data.begin(), data.end(),&err );
    
    //{"name":"battleCast","args":["{\"attribute\":1,\"castId\":1,\"castTime\":1387338655,\"target\":1,\"targetGroup\":0,\"user\":1}"]}
    
    // 一番外側のobjectの取得
    picojson::object& o = val.get<picojson::object>();
    
    // bool値の取得
    //bool b1 = o["bool1"].get<bool>();
    
    // double値の取得
    //double d1 = o["castTime"].get<double>();
    
    // string値の取得
    std::string& s1 = o["name"].get<std::string>();
  
    
    // array値の取得
    picojson::array& a1 = o["args"].get<picojson::array>();
    // arrayの中の値をループで取得
    std::string* s2 ;
    for (picojson::array::iterator i = a1.begin(); i != a1.end(); i++) {
        s2 = &i->get<std::string>();
    }
    
    picojson::value val2;
    picojson::parse(val2, s2->begin(), s2->end(),&err );
    // 一番外側のobjectの取得
    picojson::object& o2 = val2.get<picojson::object>();
   // double attribute = o2["attribute"].get<double>();
    //double castId = o2["castId"].get<double>();
    castTime = o2["castTime"].get<double>();
    target = o2["target"].get<double>();
    targetGroup = o2["targetGroup"].get<double>();
    user = o2["user"].get<double>();
    userGroup = o2["userGroup"].get<double>();
    
    
    
    // Skillをキャストする
    
    // キャラの動作を追加する
    
    
    //log("###########: %f", attribute);
   // log("###########: %f", castId);
    log("###########: %f", castTime);
    log("###########: %f", target);
    log("###########: %f", targetGroup);
    log("###########: %f", user);
    log("###########: %f", userGroup);
    
    
}


void HelloWorld::battleExecEvent(SIOClient *client, const std::string& data) {
    
    //cocos2d-xでJSONを使う http://nirasan.hatenablog.com/entry/2013/10/24/232905
	log("Att OVER【battleExecEvent】: %s", data.c_str());
    
    std::string err;
    picojson::value val;
    
    picojson::parse(val, data.begin(), data.end(),&err );
    
    
    //cocos2d: きゃすと【battleCastEvent】: {"name":"battleExec","args":["{\"name\":\"マルチウェイ\",\"target\":1,\"targetGroup\":0,\"user\":123,\"userGroup\":0,\"value\":880}"]}
    
    // 一番外側のobjectの取得
    picojson::object& o = val.get<picojson::object>();
    
    // bool値の取得
    //bool b1 = o["bool1"].get<bool>();
    
    // double値の取得
    //double d1 = o["castTime"].get<double>();
    
    // string値の取得
    std::string& s1 = o["name"].get<std::string>();
    
    
    // array値の取得
    picojson::array& a1 = o["args"].get<picojson::array>();
    // arrayの中の値をループで取得
    std::string* s2 ;
    for (picojson::array::iterator i = a1.begin(); i != a1.end(); i++) {
        s2 = &i->get<std::string>();
    }
    
    picojson::value val2;
    picojson::parse(val2, s2->begin(), s2->end(),&err );
    // 一番外側のobjectの取得
    picojson::object& o2 = val2.get<picojson::object>();
    // double attribute = o2["attribute"].get<double>();
    //double castId = o2["castId"].get<double>();
    
    
    nameExec = o2["name"].get<std::string>();
    targetExec = o2["target"].get<double>();
    targetGroupExec = o2["targetGroup"].get<double>();
    userExec = o2["user"].get<double>();
    userGroupExec = o2["userGroup"].get<double>();
    valueExec = o2["value"].get<double>();
    
    
    // Skillをキャストする
    
    // キャラの動作を追加する
    
    
    
    
    
    log("###########: %s", nameExec.c_str());
    log("###########: %f", targetExec);
    log("###########: %f", targetGroupExec);
    log("###########: %f", userExec);
    log("###########: %f", userGroupExec);
    log("###########: %f", valueExec);
    
    
}



void HelloWorld::battleStartEvent(SIOClient *client, const std::string& data) {
    
    //cocos2d-xでJSONを使う http://nirasan.hatenablog.com/entry/2013/10/24/232905
	log("##########Join Room Over#########【battleStartEvent】: %s", data.c_str());
    
    std::string err;
    picojson::value val;
    
    picojson::parse(val, data.begin(), data.end(),&err );
    
    
    //{"name":"battleStart","args":["{\"room\":\"battleRoom2\",\"enemy\":3,\"user\":4}"]}

    
    // 一番外側のobjectの取得
    picojson::object& o = val.get<picojson::object>();
    
    // bool値の取得
    //bool b1 = o["bool1"].get<bool>();
    
    // double値の取得
    //double d1 = o["castTime"].get<double>();
    
    // string値の取得
    std::string& s1 = o["name"].get<std::string>();
    
    
    // array値の取得
    picojson::array& a1 = o["args"].get<picojson::array>();
    // arrayの中の値をループで取得
    std::string* s2 ;
    for (picojson::array::iterator i = a1.begin(); i != a1.end(); i++) {
        s2 = &i->get<std::string>();
    }
    
    picojson::value val2;
    picojson::parse(val2, s2->begin(), s2->end(),&err );
    // 一番外側のobjectの取得
    picojson::object& o2 = val2.get<picojson::object>();
    // double attribute = o2["attribute"].get<double>();
    //double castId = o2["castId"].get<double>();
    
    roomName  = o2["room"].get<std::string>();
    roomEnemyId = o2["enemy"].get<double>();
    roomUserId = o2["user"].get<double>();
    
    
    // Join Room画面を非表示する
    auto loginLayer = (UILayer*)getChildByTag(10010);
    loginLayer->setVisible(false);

    
    // Playerを追加する
    /////////////////////////////////
    // PlayerA
    ArmatureDataManager::getInstance()->addArmatureFileInfo("ani/hero/girl.ExportJson");
    Armature* armaturePlayerA = Armature::create("girl");
    armaturePlayerA->setTag(10005);
    armaturePlayerA->setPosition(Point(195 ,215));
    this->addChild(armaturePlayerA);
    armaturePlayerA->setZOrder(3);
    armaturePlayerA->getAnimation()->play("loading");
    
    // PlayerB
    ArmatureDataManager::getInstance()->addArmatureFileInfo("ani/enemy/monster.ExportJson");
    Armature* armaturePlayerB = Armature::create("monster");
    armaturePlayerB->setTag(10006);
    armaturePlayerB->setPosition(Point(506 ,243));
    this->addChild(armaturePlayerB);
    armaturePlayerB->setZOrder(3);
    armaturePlayerB->getAnimation()->play("loading");
    
    
    
    
    
    
    log("###########: %s", roomName.c_str());
    log("###########: %f", roomEnemyId);
    log("###########: %f", roomUserId);

}



void HelloWorld::setCoolDownEffect(UILayout* layout)
{
    ProgressTimer* pt = ProgressTimer::create( Sprite::create("ui/SkillMenu_1/ui_seq_timer.png") );
    if ( pt != NULL )
    {
        float mPercentage = 100;    // 定义CD的显示百分比
        float cd_Time = 8.0f;      // 定义CD的时间
        pt->setPercentage(mPercentage);                          // 设置进度条最大百分比
        pt->setPosition(layout->getWorldPosition());   // 设置CD图样的位置
        pt->setType(ProgressTimer::Type::RADIAL);                 // 设置进度条动画类型
        pt->setOpacity(100);
        
        
        this->addChild(pt,4);
       
        
        
        ProgressFromTo *fromto = ProgressFromTo::create(cd_Time, mPercentage, 0);  // 设定CD时间与要到达的百分比
        
       // pt->runAction(fromto);                                       // 给进度条加上动画条件
        
        layout->setTouchEnabled(false);
      
        pt->runAction(CCSequence::create(fromto, CallFunc::create([=]() {
           layout->setTouchEnabled(true);
        }), NULL));
        
        
    }
}

void HelloWorld::skillCoolDownCallBack(UILayout* layout)
{
    // 设置蒙板不可见
    //mStencil->setVisible(false);
    
    // 进度条技能不可见
    //mProgressTimer->setVisible(false);
    
    // 按钮置为可用
    layout->setTouchEnabled(true);
}






