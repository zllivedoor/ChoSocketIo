#include "HelloWorldScene.h"

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
    
    //设置为单点响应
    setTouchMode(Touch::DispatchMode::ALL_AT_ONCE);
    
    
    // 加载场景资源
    /*
    UILayer* uiLayer = UILayer::create();
    auto myLayout = cocostudio::GUIReader::shareReader()->widgetFromJsonFile("SkillMenu_1.json"); //alpha1中使用
    myLayout->getChildByTag(3)->addTouchEventListener(this, toucheventselector(HelloWorld::menuAttackCallback));

    uiLayer->addWidget(myLayout);
    */
    
    auto node = SceneReader::getInstance()->createNodeWithSceneFile("FightScene.json");
    if(node)
    {
       
        //node->addChild(uiLayer, 100);
        this->addChild(node);
    }
   
    /*
    /////////////////////////////////
    auto child = node->getChildByTag(10014);
    auto reader = (ComRender*)child->getComponent("GUIComponent");
    auto layer = (UILayer*)reader->getNode();
    m_layout = (UILayout*)layer->getWidgetByName("ui_hp");
    //m_layout->UIWidget::setAnchorPoint(Point(0,1));
    m_layout->UIWidget::setScaleX(0.5);// remainHP/maxHP をここに設定します。
    */
    
    /////////////////////////////////
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
    
    nodeServerCon();
    
    return true;
}
 
    
void HelloWorld::nodeServerCon()
{
    //平井さんのサーバー
    _sioClient = SocketIO::connect(*this, "ws://10.13.165.59:3000");
    
    //僕のサーバー
   // _sioClient = SocketIO::connect(*this, "ws://10.13.197.156:8080");
    
    _sioClient->on("battleCast", CC_CALLBACK_2(HelloWorld::testevent, this));
}

void HelloWorld::menuAttackCallback(cocos2d::Object *sender,TouchEventType type, int skill_id)
{
    
    
    std::string args = "{\"sequence\":[{\"action\":1,\"seqId\":1},{\"action\":2,\"seqId\":2},{\"action\":3,\"seqId\":3},{\"action\":1,\"seqId\":4}],\"target\":1,\"targetGroup\":0,\"user\":123,\"userGroup\":0}";
    
    if (type == TOUCH_EVENT_BEGAN)
    {
       
    }
    if (type == TOUCH_EVENT_MOVED)
    {
        
    }
    if (type == TOUCH_EVENT_ENDED)
    {
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
void HelloWorld::testevent(SIOClient *client, const std::string& data) {
    
    //cocos2d-xでJSONを使う http://nirasan.hatenablog.com/entry/2013/10/24/232905
	log("きゃすと: %s", data.c_str());
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
    double castTime = o2["castTime"].get<double>();
    double target = o2["target"].get<double>();
    double targetGroup = o2["targetGroup"].get<double>();
    double user = o2["user"].get<double>();
    double userGroup = o2["userGroup"].get<double>();
    
    //log("###########: %f", attribute);
   // log("###########: %f", castId);
    log("###########: %f", castTime);
    log("###########: %f", target);
    log("###########: %f", targetGroup);
    log("###########: %f", user);
    log("###########: %f", userGroup);
    
    
}

void HelloWorld::echotest(SIOClient *client, const std::string& data) {
    log("だめーじ:xxxxx");
	//log("SocketIOTestLayer::echotest called with data: %s", data.c_str());
    
	//std::stringstream s;
	//s << client->getTag() << " received event echotest with data: " << data.c_str();
    
	//_sioClientStatus->setString(s.str().c_str());
    
}


