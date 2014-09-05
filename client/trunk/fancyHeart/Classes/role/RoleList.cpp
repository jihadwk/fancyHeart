//
//  RoleList.cpp
//  fancyHeart
//
//  Created by doteyplay on 14-8-12.
//
//

#include "RoleList.h"

RoleList* RoleList::create()
{
    RoleList* roleList=new RoleList();
    if (roleList && roleList->init("publish/roleList/roleList.ExportJson")) {
        roleList->autorelease();
        return roleList;
    }
    CC_SAFE_DELETE(roleList);
    return nullptr;
}

bool RoleList::init(std::string fileName)
{
	if(!BaseUI::init(fileName))
    {
        return false;
    }
    
	//如果需要对cocostudio 设计的ui进行调整
    Widget*itemPanel = static_cast<Widget*>(layout->getChildByName("item"));
    this->btnCall = static_cast<Button*>(layout->getChildByName("btnCall"));
    this->progress = static_cast<Widget*>(layout->getChildByName("progress"));
    Button* searchBtn = static_cast<Button*>(layout->getChildByName("searchBtn"));
    
    Size widgetSize = Director::getInstance()->getWinSize();
    this->rotateList = RotateList::create();
    this->rotateList->setSize(Size(widgetSize.width, widgetSize.height));
    this->rotateList->addEventListener(CC_CALLBACK_3(RoleList::rotateListCallback,this));

    //模版
    Widget* item=static_cast<Widget*>(layout->getChildByName("item"));
    item->removeFromParent();
    //传进去单个模版
    this->rotateList->setItemModel(item,widgetSize.width,widgetSize,widgetSize.width/7);
    this->addChild(this->rotateList);
    
    float x = widgetSize.width/2;
    float y =widgetSize.height-item->getContentSize().height;
    this->rotateList->setPosition(Vec2(0,y));
    
    //滚动条
    this->slider=static_cast<Slider*>(layout->getChildByName("slider"));
    this->rotateList->setSlider(this->slider);
    
    Button* returnBtn = static_cast<Button*>(layout->getChildByName("returnBtn"));
    Button* propInfoBtn = static_cast<Button*>(this->progress->getChildByName("propInfoBtn"));
    returnBtn->addTouchEventListener(CC_CALLBACK_2(RoleList::touchEvent, this));
    this->btnCall->addTouchEventListener(CC_CALLBACK_2(RoleList::touchEvent, this));
    searchBtn->setTouchEnabled(true);
    searchBtn->addTouchEventListener(CC_CALLBACK_2(RoleList::touchEvent, this));
    propInfoBtn->addTouchEventListener(CC_CALLBACK_2(RoleList::touchEvent, this));
    
    //表中所有可召唤的武将
    LoginResp* loginResp = Manager::getInstance()->getRoleData();
    XRole*xRole = XRole::record(Value(0));
    rapidjson::Value& xRoleDoc = xRole->doc;
    rapidjson::GenericValue<rapidjson::UTF8<> >::MemberIterator it = xRoleDoc.MemberonBegin();
    
    //对已被召唤的角色进行排序
    for (int i=0; i<loginResp->npclist_size(); i++) {
        XRole*xRole2 = XRole::record(Value(loginResp->npclist(i).spriteid()));
        if(xRole2->getCalled() == 1){
            this->listIds.push_back(loginResp->npclist(i).spriteid());
            log("loginResp->npclist(i).npcid():%lld",loginResp->npclist(i).npcid());
        }
    }
    sort(this->listIds.begin(),this->listIds.end(),this->sortCalled);
    
    //对未被召唤的角色进行排序
    std::vector<notCalledData > notCalledIds;
    for (; it != xRoleDoc.MemberonEnd(); it++ )
    {
        log("%s", it->name.GetString());
        //item：得到表中一条条数据
        rapidjson::Value& item=it->value;
        bool isHave = false;
        for (int j=0; j<this->listIds.size(); j++) {
            if (item["id"].GetInt() == this->listIds.at(j)) {
                isHave = true;
            }
        }
        if (isHave == false && item["called"].GetInt() == 1) {
            XRole*xRole1 = XRole::record(Value(item["id"].GetInt()));
            PItem* haveProp = Manager::getInstance()->getPropItem(xRole1->getPropId());
            int haveNum = haveProp == NULL?0:haveProp->itemnum();
            //需要召唤石的数量
            int needNum =xRole1->getCalledNum();
            notCalledIds.push_back(notCalledData{it->name.GetString(),haveNum == needNum?true:false});
        }
    }
    sort(notCalledIds.begin(), notCalledIds.end(), this->sortNotalled);
    //将排序好的已拥有的角色=》召唤进度已满的角色=》召唤进度未满的角色的id放到listIds中
    for (int j=0; j<notCalledIds.size(); j++) {
        this->listIds.push_back(std::atof(notCalledIds[j].npcId.c_str()));
    }
    //此处传进去的应该是表中可招募武将的长度
    this->rotateList->setNum(int(this->listIds.size()));
    
	return true;
}

void RoleList::onEnter()
{
    BaseUI::onEnter();
}

bool RoleList::sortCalled(int id1,int id2)
{
//    PNpc* pNpc1=Manager::getInstance()->getNpc(id1);
//    PNpc* pNpc2=Manager::getInstance()->getNpc(id2);
    PNpc* pNpc1=getNpc(id1);
    PNpc* pNpc2=getNpc(id2);
    if (pNpc1->level()!=pNpc2->level()) {
        return pNpc1->level()>pNpc2->level();
    }else if (pNpc1->quality()!=pNpc2->quality()){
        return pNpc1->quality()>pNpc2->quality();
    }else if (pNpc1->star()!=pNpc2->star()){
        return pNpc1->star()>pNpc2->star();
    }else{
        return pNpc1->npcid()>pNpc2->npcid();
    }
}

bool RoleList::sortNotalled(notCalledData data1,notCalledData data2)
{
    if (data1.isEnoughProp != data2.isEnoughProp) {
        return data1.isEnoughProp == true?true:false;
    }else{
        return data1.npcId>data2.npcId;
    }
    
    return  true;
}

PNpc* RoleList::getNpc(int spriteId)
{
    LoginResp* loginResp = Manager::getInstance()->getRoleData();
    for (int i=0;i<loginResp->npclist_size();i++)
    {
        PNpc* pnpc=loginResp->mutable_npclist(i);
        if (pnpc->spriteid()==spriteId) {
            return pnpc;
            break;
        }
    }
    return nullptr;
    
}

//设置单个模版的数据显示
void RoleList::setItemData(Widget* item,XRole* itemData,int index)
{
    PNpc calldeItem;
    Text* powerLabel=static_cast<Text*>(item->getChildByName("powerLabel"));
    Text* levelLabel=static_cast<Text*>(item->getChildByName("levelLabel"));
    Text* nameLabel=static_cast<Text*>(item->getChildByName("nameLabel"));
    ImageView*icon = static_cast<ImageView*>(item->getChildByName("icon"));
    ImageView* roleFrame=static_cast<ImageView*>(item->getChildByName("roleFrame"));
    
    //遮罩
    Mask* mask=static_cast<Mask*>(item->getChildByName("mask"));
    if (!mask) {
        mask=Mask::create(item->getSize());
        mask->setName("mask");
        item->addChild(mask);
        mask->setTouchEnabled(true);
        mask->setEnabled(false);
    }
    //此角色是否被召唤
    bool isCalled = false;
    LoginResp* loginResp = Manager::getInstance()->getRoleData();
    for (int j =0; j<loginResp->npclist_size(); j++) {
        if (itemData->getId() == loginResp->npclist(j).spriteid()) {
            isCalled = true;
            calldeItem = loginResp->npclist(j);
        }
    }
    //如果此数据不包含在服务器给的数据中，那么模版显示
    if (isCalled == false) {
        mask->show();
        levelLabel->setVisible(false);
        
    }//如果此数据包含在服务器给的数据中，那么模版不显示
    else{
        mask->hide();
        levelLabel->setVisible(true);
        levelLabel->setString(Value(calldeItem.level()).asString());
    }
    
    //人物颜色框
    std::vector<std::string> colors = {"card_1.png","card_2.png","card_2.png","card_3","card_3.png",
        "card_3.png","card_4.png","card_4.png","card_4.png","card_5.png"};
    int qualityNum =isCalled?calldeItem.quality():0;//默认品质为0
    roleFrame->loadTexture(colors.at(qualityNum));
    icon->loadTexture("grade_icon_"+Value(itemData->getPos()).asString()+".png");
    nameLabel->setString(Value(itemData->getName()).asString());
    //星级的显示
    for (int i = 0; i<6; i++) {
        ImageView*star = static_cast<ImageView*>(item->getChildByName("star"+Value(i+1).asString()));
        if (!isCalled) {
            star->loadTexture("star_1.png");
        }else {
            if (i<calldeItem.star()){
                star->loadTexture("star_2.png");
            }else{
                star->loadTexture("star_1.png");
            }
        }
    }
    //战力的显示(根据策划需求先写null，以后会有公式来算这个值)
//    powerLabel->setString("Null");
}

void RoleList::setBottomData()
{
    XRole*xRole = XRole::record(Value(middleItemData->getId()));
    Text* progressLabel = static_cast<Text*>(this->progress->getChildByName("progressLabel"));
    
    //进度条
    LoadingBar* progressBar = static_cast<LoadingBar*>(this->progress->getChildByName("progressBar"));
    //已有召唤石的数量，到背包中查看,如果为null，说明背包中没有此道具
    PItem* haveProp = Manager::getInstance()->getPropItem(xRole->getPropId());
    int haveNum = haveProp == NULL?0:haveProp->itemnum();
    //需要召唤石的数量
    int needNum =xRole->getCalledNum();
    progressLabel->setString(Value(haveNum).asString()+"/"+Value(needNum).asString());
    //如果已有召唤石数量等于需要召唤石数量，那么进度条消失，召唤石按钮出现
    if (haveNum >= needNum) {
        this->btnCall->setVisible(true);
        this->progress->setVisible(false);
    }else{
        this->btnCall->setVisible(false);
        this->progress->setVisible(true);
    }
    if (needNum != 0) {
        progressBar->setPercent(float(haveNum*100/needNum));
    }
}

void RoleList::touchEvent(Ref *pSender, TouchEventType type)
{
    Button* btn=static_cast<Button*>(pSender);
    if(type!=TouchEventType::ENDED){
        return;
    }
    if (btn->getTag() == 12161) {//返回按钮
        this->clear(true);
    }else if (btn->getTag() == 12183){//召唤按钮
        //记录当前点击的召唤的卡牌，成功后将其卡牌上的遮罩去掉
        this->currentCalledItem = this->middleItem;
        
        //向服务器发送请求
        PCallHero pCallHero;
        log("middleItemData->getId():%d",middleItemData->getId());
        pCallHero.set_heroid(middleItemData->getId());
        Manager::getInstance()->socket->send(C_CALLHERO, &pCallHero);
        
    }else if (btn->getTag() == 13223){//搜索按钮
        this->showRolePanel();
        
    }else if (btn->getTag() == 12547){//召唤石获得途径按钮
        
    }
}

void RoleList::showRolePanel(){
    Mask* mask=static_cast<Mask*>(this->middleItem->getChildByName("mask"));
    if (!mask->isVisible()) {
        LoginResp* loginResp = Manager::getInstance()->getRoleData();
        for (int j =0; j<loginResp->npclist_size(); j++) {
            if (middleItemData->getId() == int(loginResp->npclist(j).spriteid())) {
                PNpc pNpc = loginResp->npclist(j);
                Role*role = Role::create(loginResp->npclist(j));
                role->show(this);
                break;
            }
        }
    }
}

void RoleList::rotateListCallback(RotateList::EventType type,Widget*item,int index)
{
    int itemId = this->listIds.at(index);
    XRole* xRole;
    switch (type)
    {
        case RotateList::EventType::SCROLL_MIDDLE:
        {
            xRole = XRole::record(Value(itemId));
            this->middleItem = item;
            this->middleItemData = xRole;
            //如果此时的角色未召唤,并且召唤石足够，那么召唤石进度条变没，出现召唤按钮
            setBottomData();
            break;
        }
        case RotateList::EventType::TOUCH_ITEM:
        {
            this->showRolePanel();
            break;
        }
        case RotateList::EventType::SET_ITEM_DATA:
        {
            xRole = XRole::record(Value(itemId));
            this->setItemData(item,xRole,index);//传入数据
            break;
        }
        default:
            break;
    }
}

void RoleList::initNetEvent(){
    auto listener = EventListenerCustom::create(NET_MESSAGE, [=](EventCustom* event){
        NetMsg* msg = static_cast<NetMsg*>(event->getUserData());
        switch (msg->msgId)
        {
            case C_COMMONMSG:
            {
                PCommonResp pCommonResp;
                pCommonResp.ParseFromArray(msg->bytes, msg->len);
                if(pCommonResp.resulttype()==C_CALLHERO){
                    //status:0成功
                    if (pCommonResp.status()==1) {
                        Manager::getInstance()->showMsg("已经存在该宠物");
                    }else if (pCommonResp.status()==2) {
                        Manager::getInstance()->showMsg("不存在宠物");
                    }else if (pCommonResp.status()==3) {
                        Manager::getInstance()->showMsg("没有足够石头");
                    }else if (pCommonResp.status()==4) {
                        Manager::getInstance()->showMsg("系统忙碌");
                    }else if (pCommonResp.status()==5) {
                        Manager::getInstance()->showMsg("系统召唤失败");
                    }
                }
            }
                break;
            case C_ADDORREMOVENPC:
            {
                Mask* mask=static_cast<Mask*>(this->currentCalledItem->getChildByName("mask"));
                if(mask->isVisible()){
                    mask->hide();
                }
            }
                break;
            default:
                break;
        }
    });
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void RoleList::onExit()
{
    BaseUI::onExit();
}