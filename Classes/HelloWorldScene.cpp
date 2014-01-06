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
  
  auto roomJoin  = (UILayout*)loginLayer->getWidgetByName("Button_43");
  roomJoin->UIWidget::addTouchEventListener(this, toucheventselector(HelloWorld::roomJoinCallback));
  
  auto roomClose  = (UILayout*)loginLayer->getWidgetByName("close_Button");
  roomClose->UIWidget::addTouchEventListener(this, toucheventselector(HelloWorld::roomCloseCallback));
  
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
  _sioClient = SocketIO::connect(*this, "ws://10.13.164.201:3000");
  
  //僕のサーバー
  // _sioClient = SocketIO::connect(*this, "ws://10.13.197.156:8080");
  _sioClient->on("user", CC_CALLBACK_2(HelloWorld::userEvent, this));
  _sioClient->on("battleCast", CC_CALLBACK_2(HelloWorld::battleCastEvent, this));
  _sioClient->on("battleExec", CC_CALLBACK_2(HelloWorld::battleExecEvent, this));
  _sioClient->on("battleStart", CC_CALLBACK_2(HelloWorld::battleStartEvent, this));
  
}

void HelloWorld::roomJoinCallback(cocos2d::Object *sender,TouchEventType type)
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

void HelloWorld::roomCloseCallback(cocos2d::Object *sender,TouchEventType type)
{
  roomEnemyId = 10006;
  addPlayer(roomEnemyId);
  
  auto loginLayer = (UILayer*)getChildByTag(10010);
  loginLayer->setVisible(false);
  
}


void HelloWorld::menuAttackCallback(cocos2d::Object *sender,TouchEventType type)
{
  std::string args = "";
  
  if (type == TOUCH_EVENT_BEGAN)
  {
    
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
      args = "{\"sequence\":[{\"action\":1,\"seqId\":1},{\"action\":2,\"seqId\":2},{\"action\":3,\"seqId\":3},{\"action\":1,\"seqId\":4}],\"target\":1,\"targetGroup\":0,\"user\":\""+userId+"\",\"userGroup\":0}";
      
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


void HelloWorld::userEvent(SIOClient *client, const std::string& data) {
  picojson::object obj = HelloWorld::getArgs(data);
  userId = obj["userId"].get<std::string>();
}

picojson::object HelloWorld::getArgs(const std::string& data) {
  // JSONの読み込み
  std::string err;
  picojson::value val;
  picojson::parse(val, data.begin(), data.end(), &err);
  picojson::object& o = val.get<picojson::object>();
  // array値の取得
  picojson::array& a1 = o["args"].get<picojson::array>();
  std::string s = a1[0].get<std::string>();
  picojson::value argVal;
  picojson::parse(argVal, s.begin(), s.end(), &err);
  picojson::object& o2 = argVal.get<picojson::object>();
  return o2;
}
//test event callback handlers, these will be registered with socket.io
void HelloWorld::battleCastEvent(SIOClient *client, const std::string& data) {
  
  //cocos2d-xでJSONを使う http://nirasan.hatenablog.com/entry/2013/10/24/232905
	log("きゃすと【battleCastEvent】: %s", data.c_str());
  picojson::object obj = HelloWorld::getArgs(data);
  castTime = obj["castTime"].get<double>();
  target = obj["target"].get<double>();
  targetGroup = obj["targetGroup"].get<double>();
  std::string id = obj["user"].get<std::string>();
  userGroup = obj["userGroup"].get<double>();
  
  // Skillをキャストする
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double millisecs = (double)tv.tv_sec * 1000 + (double)tv.tv_usec  * 0.001;
  double castingTime = (castTime - millisecs) / 1000;
  
  // キャラの動作を追加する
  ProgressTimer* pt = ProgressTimer::create( Sprite::create("ui/SkillMenu_1/cast_bar.png") );
  if ( pt != NULL )
  {
    float mPercentage = 100;    // 定义CD的显示百分比
    float cd_Time = castingTime;      // 定义CD的时间
    int pos = positionData[id].get<double>();
    
    pt->setPercentage(mPercentage);
    pt->setMidpoint(Point(0,0));
    pt->setPosition(pos * 50 + 140 ,pos * 30 + 300);
    pt->setScaleX(200);
    pt->setScaleY(0.5);
    pt->setType(ProgressTimer::Type::BAR);
    pt->setBarChangeRate(Point(1,0));
    pt->setZOrder(30);
    //pt->setOpacity(100);
    this->addChild(pt);
    ProgressFromTo *fromto = ProgressFromTo::create(cd_Time, mPercentage, 0);
    
    pt->runAction(CCSequence::create(fromto, CallFunc::create([=]() {
    }), NULL));
  }
  
  //log("###########: %f", attribute);
  //log("###########: %f", castId);
  log("###########: %f", castTime);
  //log("###########: %f", target);
  //log("###########: %f", targetGroup);
  //log("###########: %f", user);
  //log("###########: %f", userGroup);
}


void HelloWorld::battleExecEvent(SIOClient *client, const std::string& data) {
  
  //cocos2d-xでJSONを使う http://nirasan.hatenablog.com/entry/2013/10/24/232905
	log("Att OVER【battleExecEvent】: %s", data.c_str());
  picojson::object obj = HelloWorld::getArgs(data);
  
  nameExec = obj["name"].get<std::string>();
  targetExec = obj["target"].get<double>();
  targetGroupExec = obj["targetGroup"].get<double>();
  std::string id = obj["user"].get<std::string>();
  userGroupExec = obj["userGroup"].get<double>();
  valueExec = obj["value"].get<double>();
  int hp = obj["hp"].get<double>();
  int mhp = obj["maxhp"].get<double>();
  
  // キャラの動作を追加する
  int pos = positionData[id].get<double>();
  auto armature = (Armature*)getChildByTag(pos);
  armature->getAnimation()->play("attack");
  auto armatureB = (Armature*)getChildByTag(roomEnemyId);
  //TODO if hp == 0  death
  if (hp < 1) {
    armatureB->getAnimation()->play("death");
  }
  log("###########: %s", nameExec.c_str());
  log("###########: %f", targetExec);
  log("###########: %f", targetGroupExec);
  //log("###########: %f", userExec);
  log("###########: %f", userGroupExec);
  log("###########: %f", valueExec);
  
}

void HelloWorld::battleStartEvent(SIOClient *client, const std::string& data) {
  
  //cocos2d-xでJSONを使う http://nirasan.hatenablog.com/entry/2013/10/24/232905
	log("##########Join Room Over#########【battleStartEvent】: %s", data.c_str());
  
  picojson::object obj = HelloWorld::getArgs(data);
  picojson::object userObj = obj["user"].get<picojson::object>();
  picojson::array  userArray = userObj["member"].get<picojson::array>();
  int userGroupId = userObj["groupId"].get<double>();
  int positionCount = 0;
  
  
  for (picojson::array::iterator it = userArray.begin();
       it != userArray.end(); it++
       ) {
    picojson::object& userParam = it->get<picojson::object>();
    // Playerを追加する
    std::string id = userParam["id"].get<std::string>();
    if (id == userId){
      positionData[id] = picojson::value((double)positionCount);
      addPlayer(positionCount);
      positionCount++;
    }
  }
  for (picojson::array::iterator it = userArray.begin();
       it != userArray.end(); it++
       ) {
    picojson::object& userParam = it->get<picojson::object>();
    // Playerを追加する
    std::string id = userParam["id"].get<std::string>();
    if (id != userId){
      positionData[id] = picojson::value((double)positionCount);
      addPlayer(positionCount);
      positionCount++;
    }
  }
  // Join Room画面を非表示する
  auto loginLayer = (UILayer*)getChildByTag(10010);
  loginLayer->setVisible(false);
  
  
  // Playerを追加する
  roomEnemyId = 10006;
  addPlayer(roomEnemyId);
  
  
  log("###########: %s", roomName.c_str());
  log("###########: %f", roomEnemyId);
  log("###########: %f", roomUserId);
  
}

// Playerを追加する
void HelloWorld::addPlayer(int pos)
{
  if (pos < 10000) {
    // PlayerA
    ArmatureDataManager::getInstance()->addArmatureFileInfo("ani/hero/girl.ExportJson");
    Armature* armaturePlayerA = Armature::create("girl");
    armaturePlayerA->setTag(pos);
    armaturePlayerA->setPosition(Point(pos * 50 + 100 ,pos * 30 + 200));
    armaturePlayerA->setZOrder(10 - pos);
    armaturePlayerA->getAnimation()->play("loading");
    
    if(this->getChildByTag(pos) != NULL){
      
    }else{
      this->addChild(armaturePlayerA);
    }
  }else{
    // PlayerB
    ArmatureDataManager::getInstance()->addArmatureFileInfo("ani/enemy/monster.ExportJson");
    Armature* armaturePlayerB = Armature::create("monster");
    armaturePlayerB->setTag(pos);
    armaturePlayerB->setPosition(Point(506 ,243));
    armaturePlayerB->setZOrder(3);
    armaturePlayerB->getAnimation()->play("loading");
    
    if(this->getChildByTag(pos) != NULL){
      
    }else{
      this->addChild(armaturePlayerB);
    }
  }
  
}

void HelloWorld::setCoolDownEffect(UILayout* layout)
{
  ProgressTimer* pt = ProgressTimer::create( Sprite::create("ui/SkillMenu_1/ui_seq_timer.png") );
  if ( pt != NULL )
  {
    float mPercentage = 100;
    float cd_Time = 8.0f;
    pt->setPercentage(mPercentage);
    pt->setPosition(layout->getWorldPosition());
    pt->setType(ProgressTimer::Type::RADIAL);
    pt->setOpacity(100);
    this->addChild(pt,4);
    ProgressFromTo *fromto = ProgressFromTo::create(cd_Time, mPercentage, 0);
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






