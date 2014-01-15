#include "HelloWorldScene.h"
enum {
  kTagLabel1,
  kTagLabel2,
  kTagLabel3,
  kTagLabel4,
  
  kTagColor1,
  kTagColor2,
  kTagColor3,
};
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
    
    
    
    
    /*
     今joinするときに{name:join, args{join:join}}
     みたいなイベントをemitしてるとおもいますが、
     teamA
     {name:join, args{room:0}}
     
     teamB
     {name:join, args{room:1}}
     のような感じにしてください！
     
     
     
     startを押したときはこんなかんじでいいです
     
     {name:start, args{start:0}}
     
     */
  
  /////////////////////////////////
  // Join Room 画面
  UILayer* loginLayer = UILayer::create();
  auto loginLayout = GUIReader::shareReader()->widgetFromJsonFile("ui/DemoLogin/DemoLogin.json");
  loginLayer->addWidget(loginLayout);
  loginLayer->setPosition(Point(195 ,215));
  loginLayer->setZOrder(10);
  loginLayer->setTag(10010);
    
  this->addChild(loginLayer);
    auto roomAJoin  = (UILayout*)loginLayer->getWidgetByName("Button_43_0_0");
    roomAJoin->UIWidget::addTouchEventListener(this, toucheventselector(HelloWorld::roomAJoinCallback));
    
    auto roomBJoin  = (UILayout*)loginLayer->getWidgetByName("Button_43_0");
    roomBJoin->UIWidget::addTouchEventListener(this, toucheventselector(HelloWorld::roomBJoinCallback));
    
    auto roomClose  = (UILayout*)loginLayer->getWidgetByName("close_Button");
    roomClose->UIWidget::addTouchEventListener(this, toucheventselector(HelloWorld::roomCloseCallback));
    
  /////////////////////////////////
  // Start 画面
  UILayer* startLayer = UILayer::create();
  auto startLayout = GUIReader::shareReader()->widgetFromJsonFile("ui/Battle/Battle.json");
  startLayer->addWidget(startLayout);
  startLayer->setPosition(Point(195 ,215));
  startLayer->setZOrder(10);
  startLayer->setTag(10011);
  startLayer->setVisible(false);
  this->addChild(startLayer);
    
  auto startBattle  = (UILayout*)startLayer->getWidgetByName("Button_43_0");
  startBattle->UIWidget::addTouchEventListener(this, toucheventselector(HelloWorld::startBattleCallback));
    
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
  _sioClient->on("newcomer", CC_CALLBACK_2(HelloWorld::newcomerEvent, this));
  _sioClient->on("battleStart", CC_CALLBACK_2(HelloWorld::battleStartEvent, this));
  _sioClient->on("battleCast", CC_CALLBACK_2(HelloWorld::battleCastEvent, this));
  _sioClient->on("battleExec", CC_CALLBACK_2(HelloWorld::battleExecEvent, this));
  
}


void HelloWorld::roomAJoinCallback(cocos2d::Object *sender,TouchEventType type)
{
    std::string args = "";
    
    args="{\"room\":0}";
    
    if (type == TOUCH_EVENT_BEGAN){}
    if (type == TOUCH_EVENT_MOVED){}
    if (type == TOUCH_EVENT_ENDED)
    {
        if(_sioClient != NULL) _sioClient->emit("join",args);
        
        // Join Room画面を非表示する
        auto loginLayer = (UILayer*)getChildByTag(10010);
        loginLayer->setVisible(false);
        
        
        auto startLayer = (UILayer*)getChildByTag(10011);
        startLayer->setVisible(true);
        
        
    }
    
}

void HelloWorld::roomBJoinCallback(cocos2d::Object *sender,TouchEventType type)
{
    std::string args = "";
    
    args="{\"room\":1}";
    
    if (type == TOUCH_EVENT_BEGAN){}
    if (type == TOUCH_EVENT_MOVED){}
    if (type == TOUCH_EVENT_ENDED)
    {
        if(_sioClient != NULL) _sioClient->emit("join",args);
        
        // Join Room画面を非表示する
        auto loginLayer = (UILayer*)getChildByTag(10010);
        loginLayer->setVisible(false);
        
        
        auto startLayer = (UILayer*)getChildByTag(10011);
        startLayer->setVisible(true);
        
        
    }
    
}


//{name:start, args{start:0}}
void HelloWorld::startBattleCallback(cocos2d::Object *sender,TouchEventType type)
{
    std::string args = "";
    
    args="{\"start\":0}";
    
    if (type == TOUCH_EVENT_BEGAN){}
    if (type == TOUCH_EVENT_MOVED){}
    if (type == TOUCH_EVENT_ENDED)
    {
        if(_sioClient != NULL) _sioClient->emit("start",args);
        
        // Join Room画面を非表示する
        auto loginLayer = (UILayer*)getChildByTag(10010);
        loginLayer->setVisible(false);
        
        
        auto startLayer = (UILayer*)getChildByTag(10011);
        startLayer->setVisible(false);
        
        
    }
    
}




void HelloWorld::roomCloseCallback(cocos2d::Object *sender,TouchEventType type)
{
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
      addAction(1);
    }else if(sender->isEqual(ui_seq_2)){
      addAction(2);
    }else if(sender->isEqual(ui_seq_3)){
      addAction(3);
    }else if(sender->isEqual(ui_seq_4)){
      addAction(4);
    }else if(sender->isEqual(ui_seq_5)){
      addAction(5);
    }else if(sender->isEqual(ui_seq_6)){
      addAction(6);
    }else if(sender->isEqual(ui_seq_start)){
      sendMsg["sequence"] = picojson::value((picojson::object)actionData);
      
      setCoolDownEffect(ui_seq_start);
      
      std::string args = ((picojson::value)(sendMsg)).serialize();
      
      if(_sioClient != NULL) _sioClient->emit("battleAction",args);
      sendMsg.clear();
      actionData.clear();
      currentAction = 0;
      this->removeChildByTag(901);
      this->removeChildByTag(902);
      this->removeChildByTag(903);
      this->removeChildByTag(904);
      
      sendMsg["target"] = picojson::value((std::string)target);
      sendMsg["targetGroup"] = picojson::value((std::string)"0");
      sendMsg["user"] = picojson::value((std::string)userId);
      sendMsg["userGroup"] = picojson::value((std::string)"0");
    }
    
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

void HelloWorld::newcomerEvent(SIOClient *client, const std::string& data) {
  
  picojson::object obj = HelloWorld::getArgs(data);
  std::string team = obj["team"].get<std::string>();
  FadeIn *fadeIn = FadeIn::create(0.2);
  ScaleTo* skaleTo = ScaleTo::create(0.1f, 1.5f);
	Sequence* removeAction = Sequence::create(DelayTime::create(3),
                                                fadeIn->reverse(),
                                                NULL);
  
  auto newcomer = LabelTTF::create("A new player joined the team " + team + " !!", "Abduction", 17);
  newcomer->setPosition(Point(435 ,450));
  newcomer->setTag(999);
  newcomer->setZOrder(1000);
  newcomer->setScale(0);
  this->addChild(newcomer);
	Sequence* seqActionArts = Sequence::create(skaleTo,
                                             removeAction,
                                             NULL);
  newcomer->runAction(seqActionArts);
  
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
  double castTime = obj["castTime"].get<double>();
  std::string target = obj["target"].get<std::string>();
  std::string targetGroup = obj["targetGroup"].get<std::string>();
  std::string id = obj["user"].get<std::string>();
  std::string userGroup = obj["userGroup"].get<std::string>();
  
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
    int posX = 0;
    int posY = 0;
    if (pos < 10) {
      posX = pos * 50 + 150;
      posY = pos * 30 + 200;
    } else {
      posX = 590 - (pos - 10) * 50;
      posY = (pos - 10) * 30 + 230;
    }
    
    pt->setPercentage(mPercentage);
    pt->setMidpoint(Point(0,0));
    pt->setPosition(posX ,posY + 90);
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
  
  std::string name = obj["name"].get<std::string>();
  std::string target = obj["target"].get<std::string>();
  std::string targetGroup = obj["targetGroup"].get<std::string>();
  std::string user = obj["user"].get<std::string>();
  std::string userGroup = obj["userGroup"].get<std::string>();
  std::string value = obj["value"].get<std::string>();

  int hp = 0;
  if( obj["hp"].is<double>()){
    hp = obj["hp"].get<double>();
  }else{
      hp = 1000;
  }
 
  int mhp = obj["maxhp"].get<double>();
  
  int targetPos = positionData[target].get<double>();
  int targetPosX = 0;
  int targetPosY = 0;
  if (targetPos < 10) {
    targetPosX = targetPos * 50 + 150;
    targetPosY = targetPos * 30 + 250;
  } else {
    targetPosX = 590 - (targetPos - 10) * 50;
    targetPosY = (targetPos - 10) * 30 + 230;
  }
  
  auto damage = LabelTTF::create(value, "Abduction", 76);
  damage->enableStroke(ccBLACK,2.0,true);
  damage->setPosition(Point(targetPosX + 30 ,targetPosY + 50));
  damage->setTag(1000);
  damage->setZOrder(1000);
  damage->setScale(0);
  this->addChild(damage);
  
  DelayTime *delay = DelayTime::create(0.45);
  FadeIn *fadeIn = FadeIn::create(0.2);
  ScaleTo* skaleTo = ScaleTo::create(0.1f, 1.5f);
	MoveTo* moveAction = MoveTo::create(2.0, Point(targetPosX + 30 ,targetPosY + 150));
	Sequence* removeAction = Sequence::create(DelayTime::create(1),
                                            fadeIn->reverse(),
                                            NULL);
  Spawn* Spawn = Spawn::create(moveAction,
                               removeAction,
                               NULL);
	Sequence* seqAction = Sequence::create(DelayTime::create(0.45),
                                         skaleTo,
                                         Spawn,
                                         NULL);
  damage->runAction(seqAction);
  int pos = positionData[user].get<double>();
  int posX = 0;
  int posY = 0;
  if (pos < 10) {
    posX = pos * 50 + 150;
    posY = pos * 30 + 200;
  } else {
    posX = 590 - (pos - 10) * 50;
    posY = (pos - 10) * 30 + 230;
  }
  FadeIn *artsfadeIn = FadeIn::create(0.2);
  ScaleTo* artsskaleTo = ScaleTo::create(0.1f, 1.5f);
	Sequence* artsremoveAction = Sequence::create(DelayTime::create(0.5),
                                                artsfadeIn->reverse(),
                                                NULL);
  
  auto artsName = LabelTTF::create(name, "Abduction", 48);
  artsName->enableStroke(ccBLACK,1.0,true);
  artsName->setPosition(Point(posX ,posY + 130));
  artsName->setTag(999);
  artsName->setZOrder(1000);
  artsName->setScale(0);
  this->addChild(artsName);
	Sequence* seqActionArts = Sequence::create(artsskaleTo,
                                             artsremoveAction,
                                             NULL);
  artsName->runAction(seqActionArts);
  
  // キャラの動作を追加する
  auto armature = (Armature*)getChildByTag(pos);
  armature->getAnimation()->play("attack");
  //TODO if hp == 0  death
  if (hp < 1) {
    auto armature = (Armature*)getChildByTag(targetPos);
    armature->getAnimation()->play("death");
  }
  log("###########: %s", name.c_str());
  log("###########: %s", target.c_str());
  log("###########: %s", targetGroup.c_str());
  log("###########: %s", user.c_str());
  log("###########: %s", userGroup.c_str());
  log("###########: %s", value.c_str());
  
}

void HelloWorld::battleStartEvent(SIOClient *client, const std::string& data) {
  
  //cocos2d-xでJSONを使う http://nirasan.hatenablog.com/entry/2013/10/24/232905
	log("##########Join Room Over#########【battleStartEvent】: %s", data.c_str());
  
  picojson::object obj = HelloWorld::getArgs(data);

  picojson::object userObj = obj["user"].get<picojson::object>();
  picojson::array  userArray = userObj["member"].get<picojson::array>();
  int positionCount = 0;
  
  // 自分を追加
  for (picojson::array::iterator it = userArray.begin();
       it != userArray.end(); it++
       ) {
    picojson::object& userParam = it->get<picojson::object>();
    // Playerを追加する
    std::string id = userParam["id"].get<std::string>();
    if (id == userId){
      positionData[id] = picojson::value((double)positionCount);
      addPlayer(positionCount,true);
      positionCount++;
    }
  }
  // 味方を追加
  for (picojson::array::iterator it = userArray.begin();
       it != userArray.end(); it++
       ) {
    picojson::object& userParam = it->get<picojson::object>();
    // Playerを追加する
    std::string id = userParam["id"].get<std::string>();
    if (id != userId){
      positionData[id] = picojson::value((double)positionCount);
      addPlayer(positionCount,true);
      positionCount++;
    }
  }

  picojson::object enemyObj = obj["enemy"].get<picojson::object>();
  picojson::array  enemyArray = enemyObj["member"].get<picojson::array>();
  positionCount = 10;
  // 敵を追加
  for (picojson::array::iterator it = enemyArray.begin();
       it != enemyArray.end(); it++
       ) {
    picojson::object& enemyParam = it->get<picojson::object>();
    // Playerを追加する
    std::string id = enemyParam["id"].get<std::string>();
    target = id;
    if (id != userId){
      positionData[id] = picojson::value((double)positionCount);
      addPlayer(positionCount,false);
      positionCount++;
    }
  }
  
  sendMsg["target"] = picojson::value((std::string)target);
  sendMsg["targetGroup"] = picojson::value((std::string)"0");
  sendMsg["user"] = picojson::value((std::string)userId);
  sendMsg["userGroup"] = picojson::value((std::string)"0");
  
  // Join Room画面を非表示する
  auto loginLayer = (UILayer*)getChildByTag(10010);
  loginLayer->setVisible(false);
  
  log("######target#####: %s", target.c_str());
  log("###########: %s", roomName.c_str());
  
}

// Playerを追加する
void HelloWorld::addPlayer(int pos, bool Ally)
{
  if (Ally) {
    log("#####adding player###### %i", pos);
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
    int npos = pos - 10;
    log("#####adding enemy###### %i", npos);
    // PlayerB
    ArmatureDataManager::getInstance()->addArmatureFileInfo("ani/enemy/monster.ExportJson");
    Armature* armaturePlayerB = Armature::create("monster");
    armaturePlayerB->setTag(pos);
    armaturePlayerB->setPosition(Point(590 - npos * 30  ,npos * 30 + 230));
    armaturePlayerB->setZOrder(10 - npos);
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


// Actionを追加する
void HelloWorld::addAction(int actionId)
{
  if (currentAction < 4) {
    stringstream ss;
    ss << currentAction;
    picojson::object obj;
    obj["action"] = picojson::value((double)actionId);
    actionData[ss.str()] = picojson::value((picojson::object)obj);
    currentAction++;
    
    stringstream ss2;
    ss2 << actionId;
    auto action = LabelTTF::create(ss2.str(), "Abduction", 76);
    action->enableStroke(ccBLACK,2.0,true);
    action->setPosition(Point(50 * currentAction ,350));
    action->setTag(900+currentAction);
    action->setZOrder(1001);
    this->addChild(action);
    
    log("addAction action: %i", actionId);
  }
}



