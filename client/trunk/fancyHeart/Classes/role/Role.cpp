//
//  Role.cpp
//  fancyHeart
//
//  Created by doteyplay on 14-8-12.
//
//

#include "Role.h"

Role* Role::create(PNpc itemData)
{
    Role* role=new Role();
    if (role && role->init("publish/roleItem/roleItem.ExportJson",itemData)) {
        role->autorelease();
        return role;
    }
    CC_SAFE_DELETE(role);
    return nullptr;
}

bool Role::init(std::string fileName,PNpc itemData)
{
	if(!BaseUI::init(fileName))
    {
        return false;
    }
    
    this->itemData = itemData;
	//如果需要对cocostudio 设计的ui进行调整
    Widget* imgBg = static_cast<Widget*>(layout->getChildByName("imgBg"));
    this->propertyPanel = static_cast<Widget*>(imgBg->getChildByName("propertyPanel"));
    this->skillPanel = static_cast<Widget*>(imgBg->getChildByName("skillPanel"));
    this->rolePic = static_cast<Widget*>(imgBg->getChildByName("rolePic"));
    this->panel = static_cast<Widget*>(layout->getChildByName("panel"));
    this->equipPanel = static_cast<Widget*>(imgBg->getChildByName("equipPanel"));
    Button* btnReturn = static_cast<Button*>(panel->getChildByName("btnReturn"));
    Button*btnAscend = static_cast<Button*>(this->equipPanel->getChildByName("btnAscend"));
    this->progress = static_cast<Widget*>(this->panel->getChildByName("progress"));
    Button* propInfoBtn = static_cast<Button*>(this->progress->getChildByName("propInfoBtn"));
    this->changeBtn = static_cast<Button*>(this->panel->getChildByName("changeBtn"));
    this->changeBtn->addTouchEventListener(CC_CALLBACK_2(Role::touchBtnEvent, this));
    this->desc = static_cast<Text*>(this->panel->getChildByName("desc"));
    btnReturn->setTouchEnabled(true);
    propInfoBtn->setTouchEnabled(true);
    btnReturn->addTouchEventListener(CC_CALLBACK_2(Role::touchBtnEvent, this));
    propInfoBtn->addTouchEventListener(CC_CALLBACK_2(Role::touchBtnEvent, this));
    btnAscend->addTouchEventListener(CC_CALLBACK_2(Role::touchBtnEvent, this));
    //设置左边栏属性的显示
    this->setBtnVisible();
    this->rolePic->setVisible(true);
    this->desc->setVisible(false);
    
    //按钮分别为图鉴，属性，技能，派遣
    std::vector<Button*> buttons;
    std::vector<std::string> btnName={"handbookBtn","qualityBtn","skillBtn","sendBtn"};
    for (std::string name : btnName)
    {
        Button* btn=static_cast<Button*>(panel->getChildByName(name));
        btn->setTouchEnabled(true);
        btn->addTouchEventListener(CC_CALLBACK_2(Role::touchEvent,this));
        buttons.push_back(static_cast<Button*>(btn));
    }
    tabBar=TabBar::create(buttons);
    tabBar->retain();
    
    this->setData();
    
	return true;
}

void Role::onEnter()
{
    BaseUI::onEnter();
}

void Role::setData()
{
    Text* rateRight = static_cast<Text*>(this->panel->getChildByName("rateRight"));
    Text* nameLabel = static_cast<Text*>(rolePic->getChildByName("nameLabel"));
    Text* levelLabel = static_cast<Text*>(rolePic->getChildByName("levelLabel"));
    //战力的显示(根据策划需求先写null，以后会有公式来算这个值)
//    static_cast<Text*>(panel->getChildByName("powerRight"))->setString("Null");
//    static_cast<Text*>(this->rolePic->getChildByName("powerLabel"))->setString("Null");
    
    nameLabel->setString(Value(itemData.npcname()).asString());
    static_cast<Text*>(panel->getChildByName("nameRight"))->setString(Value(itemData.npcname()).asString());
    levelLabel->setString(Value(itemData.level()).asString());
    static_cast<Text*>(panel->getChildByName("levelRight"))->setString(Value(itemData.level()).asString());
    
    std::vector<Widget*> stars;//星级
    std::vector<Widget*> starsRight;//右边星级
    for (int i=0; i<6; i++) {
        ImageView* star=static_cast<ImageView*>(this->rolePic->getChildByName("star"+Value(i+1).asString()));
        stars.push_back(static_cast<Widget*>(star));
        //右边显示的星级
        ImageView* star1=static_cast<ImageView*>(this->panel->getChildByName("starRight"+Value(i+1).asString()));
        starsRight.push_back(static_cast<Widget*>(star1));
        if (i <this->itemData.star()) {
            star->loadTexture("star_2.png");
            star1->loadTexture("star_2.png");
        }else{
            star->loadTexture("star_1.png");
            star1->loadTexture("star_1.png");
        }
    }
    
    ImageView*posPic = static_cast<ImageView*>(this->rolePic->getChildByName("posPic"));
    ImageView*posPicRight = static_cast<ImageView*>(this->panel->getChildByName("posPicRight"));
    //站位
    XRole*xRole = XRole::record(Value(this->itemData.spriteid()));
    posPic->loadTexture("grade_icon_"+Value(xRole->getPos()).asString()+".png");
    posPicRight->loadTexture("grade_icon_"+Value(xRole->getPos()).asString()+".png");
    
    //人物颜色框
    std::vector<std::string> colors = {"card_1.png","card_2.png","card_2.png","card_3","card_3.png",
        "card_3.png","card_4.png","card_4.png","card_4.png","card_5.png"};
    ImageView* roleFrame=static_cast<ImageView*>(this->rolePic->getChildByName("roleFrame"));
    roleFrame->loadTexture(colors.at(this->itemData.quality()));
    //角色属性表中数据
    this->xRoleData = XRoleData::record(Value(Value(this->itemData.spriteid()).asString() +Value(this->itemData.quality()).asString()));
    //装备
    for (int m=0; m<6; m++) {
        Sprite*sp = Sprite::create();
        this->equips.push_back(sp);
    }
    //设置装备位装备的排列
    this->setEquips();
    //设置进度条进度以及按钮的显示
    this->setProgressData();
    //经验
    LoadingBar*expProgressBar = static_cast<LoadingBar*>(this->panel->getChildByName("expProgressBar"));
//    XExp*xExp = XExp::record(Value(this->itemData.level()));
//    rateRight->setString(Value(this->itemData.exp()).asString() +"/"+ Value(xExp->getExp()).asString());
    //进度条进度
//    expProgressBar->setPercent(float(this->itemData.exp()*100/xExp->getExp()));
}

void Role::setProgressData()
{
    //召唤石数量
    Text* rateLabel = static_cast<Text*>(progress->getChildByName("rateLabel"));
    LoadingBar* progressBar = static_cast<LoadingBar*>(this->progress->getChildByName("progressBar"));
    XRole*xRole = XRole::record(Value(this->itemData.spriteid()));
    XRoleStar* xRoleStar = XRoleStar::record(Value(Value(this->itemData.spriteid()).asString()+Value(this->itemData.star()).asString()));

    //已有召唤石的数量，到背包中查看,如果为null，说明背包中没有此道具
    int aad = xRole->getPropId();
    PItem* haveProp = Manager::getInstance()->getPropItem(xRole->getPropId());
    int haveNum = haveProp == NULL?0:haveProp->itemnum();
    //进化需要召唤石的数量
    int needNum =xRoleStar->getItemNum();//xRole->getCalledNum();
    //进化需要花费
    int cost =xRoleStar->getCost();
    rateLabel->setString(Value(haveNum).asString()+"/"+Value(needNum).asString());
    //如果已有召唤石数量等于需要召唤石数量，那么进度条消失，召唤石按钮出现
    if (haveNum < needNum) {
        this->progress->setVisible(true);
        this->changeBtn->setVisible(false);
    }else{
        this->progress->setVisible(false);
        this->changeBtn->setVisible(true);
        //当召唤石足够的时候，显示的是进化按钮，品阶为【紫+2】及星级为6星的角色才能进行变异并且召唤石是否足够，不足够则显示召唤石进度
        if (this->itemData.quality() >= 8 && this->itemData.star() == 6) {
            this->changeBtn->setTitleText("变异");
            this->equipPanel->setVisible(false);
            this->status = 1;
        }else if (this->itemData.star() < 6){
            this->changeBtn->setTitleText("进化");
            this->status = 0;
        }else if(this->itemData.quality() < 8 &&this->itemData.star()== 6){
            this->desc->setVisible(true);
            //让按钮置灰并且不可点击
            this->changeBtn->setTouchEnabled(false);
        }
    }
    if (needNum != 0) {
        progressBar->setPercent(float(haveNum*100/needNum));
    }
}

void Role::setEquips()
{
    //存储需要装备的id
    this->equipsData = {this->xRoleData->getEquipPos1(),this->xRoleData->getEquipPos2(),this->xRoleData->getEquipPos3(),
                                    this->xRoleData->getEquipPos4(),this->xRoleData->getEquipPos5(),this->xRoleData->getEquipPos6()};
    Sprite*newEquip;
    //判断装备位的状态以及显示已经穿上的装备－－共五种状态：无装备，未装备，可合成，可装备，已装备
    for (int i = 0; i<6; i++) {
        Widget* equipPos = static_cast<Widget*>(this->equipPanel->getChildByName("equipPos"+Value(i+1).asString()));
        equipPos->setTouchEnabled(true);
        equipPos->addTouchEventListener(CC_CALLBACK_2(Role::touchEvent, this));
        XItem* xItem = XItem::record(Value(equipsData.at(i)));
        //合成表
        XCraft*xCraft = XCraft::record(Value(equipsData.at(i)));
        bool isHaveEquip = false;
        for (int j = 0; j<this->itemData.equiplist_size(); j++) {
            //表中的装备id和道具表中的id相等时，显示道具
            if (equipsData.at(i) == this->itemData.equiplist(j).itemid()) {
                isHaveEquip = true;
                this->setEquipToPos(2,i+1);
                this->equipStatus.push_back(2);
                //显示装备
                createIconBox(equipsData.at(i),"equipPos"+Value(i+1).asString(),i+1);
            }
        }
        //当装备位上没有穿装备
        if (!isHaveEquip) {
            PItem* haveProp = Manager::getInstance()->getPropItem(equipsData.at(i));
            //当背包中没有此装备的时候分为两种情况，一种是有足够的材料可合成一种是材料不够不足以合成
            if (haveProp == nullptr) {
                std::vector<int> materialsId;
                //可合成－－当角色未穿上装备及玩家的背包内没有所需的装备道具，但背包中有足以合成装备道具的材料的时候
                if (xCraft->doc[Value(equipsData.at(i)).asString().c_str()].IsNull() == false) {
                    bool isMayCompose = true;
                    if (xCraft->getItem1()!=0){
                        isMayCompose = isMayCompose = propIsEnough(xCraft->getItem1(),xCraft->getNum1());
                        if (isMayCompose == false) {
                            this->setEquipToPos(1,i+1);
                            this->equipStatus.push_back(1);
                            continue;
                        }
                    }if (xCraft->getItem2()!=0){
                        isMayCompose = isMayCompose = propIsEnough(xCraft->getItem1(),xCraft->getNum2());
                        if (isMayCompose == false) {
                            this->setEquipToPos(1,i+1);
                            this->equipStatus.push_back(1);
                            continue;
                        }
                    }if (xCraft->getItem3()!=0){
                        isMayCompose = isMayCompose = propIsEnough(xCraft->getItem1(),xCraft->getNum3());
                        if (isMayCompose == false) {
                            this->setEquipToPos(1,i+1);
                            this->equipStatus.push_back(1);
                            continue;
                        }
                    }if (xCraft->getItem4()!=0){
                        isMayCompose = isMayCompose = propIsEnough(xCraft->getItem1(),xCraft->getNum4());
                        if (isMayCompose == false) {
                            this->setEquipToPos(1,i+1);
                            this->equipStatus.push_back(1);
                            continue;
                        }
                    }if (xCraft->getItem5()!=0){
                        isMayCompose = isMayCompose = propIsEnough(xCraft->getItem1(),xCraft->getNum5());
                        if (isMayCompose == false) {
                            this->setEquipToPos(1,i+1);
                            this->equipStatus.push_back(1);
                            continue;
                        }
                    }
                    this->setEquipToPos(3,i+1);
                    this->equipStatus.push_back(3);
                }//无装备--当角色未穿上装备，且玩家的背包内没有所需的装备道具时
                else{
                    this->setEquipToPos(1,i+1);
                    this->equipStatus.push_back(1);
                }
            }//未装备－－当玩家的背包内有所需的装备道具，但角色因等级小于装备等级限制，而尚未装备该道具时
            else if (haveProp!= nullptr && xItem->getMaxLv()>=itemData.level()){
                this->setEquipToPos(5,i+1);
                this->equipStatus.push_back(5);
            }//可装备－－当玩家的背包内有所需的装备道具，且角色等级与装备等级限制限制相当，且尚未装备该道具时
            else if (haveProp!= nullptr && xItem->getMaxLv()<itemData.level()){
                this->setEquipToPos(4,i+1);
                this->equipStatus.push_back(4);
            }
        }
    }
    //判断各装备位状态，如果都有装备，则升阶按钮可点击
    this->isMayPress = true;
    Button*btnAscend = static_cast<Button*>(this->equipPanel->getChildByName("btnAscend"));
    for (int m = 0; m<this->equipStatus.size(); m++) {
        if(this->equipStatus.at(m)!= 2){
            this->isMayPress = false;
            break;
        }
    }
}
bool Role::propIsEnough(int itemId,int num)
{
    PItem*item = Manager::getInstance()->getPropItem(itemId);
    if (item != nullptr) {
        if (item->itemnum() >= num) {
            return true;
        }
    }
    return false;
    
}

void Role::setEquipToPos(int index,int posIndex)
{
    Widget* equipPos = static_cast<Widget*>(this->equipPanel->getChildByName("equipPos"+Value(posIndex).asString()));
    ImageView*addPic = static_cast<ImageView*>(equipPos->getChildByName("addPic"));
    ImageView*statuesPic = static_cast<ImageView*>(equipPos->getChildByName("statuesPic"));

    statuesPic->loadTexture("equipStatus_"+ Value(index).asString() +".png");
    if (index == 1 || index == 5) {
        addPic->loadTexture("addition_1.png");
    }else if(index == 3 || index == 4){
        addPic->loadTexture("addition_2.png");
    }
}

void Role::setBtnVisible()
{
    this->propertyPanel->setVisible(false);
    this->skillPanel->setVisible(false);
    this->rolePic->setVisible(false);
}

void Role::touchEvent(Ref *pSender, TouchEventType type)
{
    Button* btn=static_cast<Button*>(pSender);
    
    if(type!=TouchEventType::ENDED){
        return;
    }
    if (btn->getTag() == 12066) {//图鉴按钮
        //设置按钮选中状态
        tabBar->setIndex(0);
        setBtnVisible();
        this->rolePic->setVisible(true);
    }else if (btn->getTag() == 12067){//属性按钮
        tabBar->setIndex(1);
        setBtnVisible();
        this->propertyPanel->setVisible(true);
        
        static_cast<Text*>(this->propertyPanel->getChildByName("hpLabel1"))->setString(Value(this->xRoleData->getHp()).asString());
        static_cast<Text*>(this->propertyPanel->getChildByName("hpRateLabel1"))->setString(Value(this->xRoleData->getHpRate()).asString());
        static_cast<Text*>(this->propertyPanel->getChildByName("atkLabel1"))->setString(Value(this->xRoleData->getAtk()).asString());
        static_cast<Text*>(this->propertyPanel->getChildByName("atkRateLabel1"))->setString(Value(this->xRoleData->getAtkRate()).asString());
        static_cast<Text*>(this->propertyPanel->getChildByName("dfLabel1"))->setString(Value(this->xRoleData->getDf()).asString());
        static_cast<Text*>(this->propertyPanel->getChildByName("dfRateLabel1"))->setString(Value(this->xRoleData->getDfRate()).asString());
        static_cast<Text*>(this->propertyPanel->getChildByName("mDfLabel1"))->setString(Value(this->xRoleData->getMDf()).asString());
        static_cast<Text*>(this->propertyPanel->getChildByName("mDfRateLabel1"))->setString(Value(this->xRoleData->getMDfRate()).asString());
        static_cast<Text*>(this->propertyPanel->getChildByName("missLabel1"))->setString(Value(this->xRoleData->getMiss()).asString());
        static_cast<Text*>(this->propertyPanel->getChildByName("crhLabel1"))->setString(Value(this->xRoleData->getCrh()).asString());
        static_cast<Text*>(this->propertyPanel->getChildByName("healLabel1"))->setString(Value(this->xRoleData->getHeal()).asString());
        
        static_cast<Text*>(this->propertyPanel->getChildByName("hpLabel"))->setString("生命：");
        static_cast<Text*>(this->propertyPanel->getChildByName("hpRateLabel"))->setString("成长：");
        static_cast<Text*>(this->propertyPanel->getChildByName("atkLabel"))->setString("攻击：");
        static_cast<Text*>(this->propertyPanel->getChildByName("atkRateLabel"))->setString("成长：");
        static_cast<Text*>(this->propertyPanel->getChildByName("dfLabel"))->setString("物防：");
        static_cast<Text*>(this->propertyPanel->getChildByName("dfRateLabel"))->setString("成长：");
        static_cast<Text*>(this->propertyPanel->getChildByName("mDfLabel"))->setString("法防：");
        static_cast<Text*>(this->propertyPanel->getChildByName("mDfRateLabel"))->setString("成长：");
        static_cast<Text*>(this->propertyPanel->getChildByName("missLabel"))->setString("闪避：");
        static_cast<Text*>(this->propertyPanel->getChildByName("crhLabel"))->setString("爆击：");
        static_cast<Text*>(this->propertyPanel->getChildByName("healLabel"))->setString("生命恢复速度：");
    }else if (btn->getTag() == 12068){//技能按钮
        this->tabBar->setIndex(2);
        setBtnVisible();
        std::vector<Widget*> skills;//技能图标
        for (int i=0; i<7; i++) {
            Widget* skill=static_cast<Widget*>(skillPanel->getChildByName("skill"+Value(i+1).asString()));
            skills.push_back(static_cast<Widget*>(skill));
            Text*levelLabel =static_cast<Text*>(skill->getChildByName("levelLabel"));
            bool boo = this->itemData.skillidlist_size()>i;
            levelLabel->setVisible(boo);
            if (boo) {
                int skillId = this->itemData.skillidlist(i);
                levelLabel->setString(Value(skillId%100).asString());  //还需要显示技能图标
            }
        }
        this->skillPanel->setVisible(true);
    }else if (btn->getTag() == 12069){//派遣按钮
        this->tabBar->setIndex(3);
    }else if (btn->getTag() == 12079){//装备位1
        this->openInfoPanel(0);
    }else if (btn->getTag() == 12082){//装备位2
        this->openInfoPanel(1);
    }else if (btn->getTag() == 12078){//装备位3
        this->openInfoPanel(2);
    }else if (btn->getTag() == 12083){//装备位4
        this->openInfoPanel(3);
    }else if (btn->getTag() == 12081){//装备位5
        this->openInfoPanel(4);
    }else if (btn->getTag() == 12080){//装备位6
        this->openInfoPanel(5);
    }
}

void Role::openInfoPanel(int index)
{
    XItem* xItem;
    int equipIndex;
    equipIndex = this->equipStatus.at(index);
    xItem = XItem::record(Value(this->equipsData.at(index)));
    EquipInfo* equipInfo = EquipInfo::create(equipIndex,xItem,this->itemData.spriteid());
    equipInfo->show();
}

void Role::touchBtnEvent(Ref *pSender, TouchEventType type)
{
    Button* btn=static_cast<Button*>(pSender);
    if (!btn) {
        return;
    }
    
    if (btn->getTag() == 12088) {//返回按钮
        this->clear(true);
    }else if (btn->getTag() == 12305){//点击后弹出召唤石获得途径窗口按钮
        
    }else if (btn->getTag() == 12548){//角色的升阶按钮
        //测试代码
        this->openAscendSuccess();
        if (this->isMayPress) {//可以升阶－－－－－－－－－－－－－－－－－－－－－－－升阶接口（此时若玩家身上的游戏币不足，则反馈消息“游戏币不足”）
            //向服务器发送请求——升阶
//            PAscend pAscend;
//            pAscend.set_heroid(this->itemData.npcid());
//            Manager::getInstance()->socket->send(C_ASCEND, &pAscend);
        }
    }else if(btn->getTag() == 14423){//判断是进化还是变异按钮
        //向服务器发送请求——进化
        if (this->status == 0) {//进化
//            PEvolve pEvolve;
//            pEvolve.set_heroid(this->itemData.npcid());
//            Manager::getInstance()->socket->send(C_EVOLVE, &pEvolve);
        }else if(this->status == 1){//变异
//            PMutation pMutation;
//            pMutation.set_heroid(this->itemData.npcid());
//            Manager::getInstance()->socket->send(C_MUTATION, &pMutation);
        }
    }
}

void Role::initNetEvent(){
    auto listener = EventListenerCustom::create(NET_MESSAGE, [=](EventCustom* event){
        NetMsg* msg = static_cast<NetMsg*>(event->getUserData());
        switch (msg->msgId)
        {
            case C_COMMONMSG:
            {
                PCommonResp pCommonResp;
                pCommonResp.ParseFromArray(msg->bytes, msg->len);
                if(pCommonResp.resulttype()==C_ASCEND){//升阶
                    //status:0成功
                    if (pCommonResp.status()!=0) {
//                        Manager::getInstance()->showMsg("");
                    }
                }else if (pCommonResp.resulttype()==C_EVOLVE){//进化
                    if (pCommonResp.status()!=0) {
                        
                    }
                }else if (pCommonResp.resulttype()==C_MUTATION){//变异
                    if (pCommonResp.status()!=0) {
                        
                    }
                }
            }
                break;
//            case C_ASCEND://升阶
//            {
//                //升阶成功则弹出弹窗
//            }
//                break;
            case C_UPROLE://更新角色
            {
                int npcid = this->itemData.npcid();
                this->itemData = *Manager::getInstance()->getNpc(npcid);
                this->setData();
            }
                break;
//            case C_EVOLVE://进化
//            {
//                
//            }
//                break;
//            case C_MUTATION://变异
//            {
//                
//            }
//                brak;
//            default:
//                break;
        }
    });
    this->_eventDispatcher->addEventListenerWithFixedPriority(listener,1);
}
//升阶成功弹出弹窗
void Role::openAscendSuccess()
{
//    //////////////////
//    //测试代码
//    InfoPanel*infoPanel = InfoPanel::create(0,this->itemData);
//    infoPanel->show(this);
//    return;
//    //////////////////
    int quality = this->itemData.quality();
    //升阶成功后若品阶等级等于1、3、6、9时,弹出的弹窗
    if (quality == 1||quality == 3||quality == 6||quality == 9) {
        InfoPanel*infoPanel = InfoPanel::create(0,this->itemData);//传进去升级后的数据
        infoPanel->show(this);
    }//升阶成功后若品阶等级等于2、4、5、7、8时，弹出的弹窗
    else if (quality == 2||quality == 4||quality == 5||quality == 7||quality == 8){
        InfoPanel*infoPanel = InfoPanel::create(1,this->itemData);
        infoPanel->show(this);
    }
}

void Role::createIconBox(int itemId,string parentName,int index)
{
    Widget* img=this->equipPanel->getChildByName(parentName);
    Widget* iconBox=Widget::create();
    Sprite* sprite=Sprite::create("prop.png");
    Sprite* spriteFrame=Sprite::create("Damascene.png");
    Sprite* maskSprite=Utils::maskedSpriteWithSprite(sprite, spriteFrame);
    maskSprite->setAnchorPoint(Vec2(0, 0));
    iconBox->addChild(maskSprite);
    maskSprite->setPosition(Vec2(3, 3));
    ImageView* iconFrame=ImageView::create();
//    iconFrame->setName("iconFrame");
    iconBox->addChild(iconFrame);
    XItem* xItem=XItem::record(Value(itemId));
    iconFrame->loadTexture("hero_circle_"+Value(xItem->getRate()+1).asString()+".png");
    iconFrame->setAnchorPoint(Vec2(0, 0));
    iconFrame->setTag(img->getTag());//点击此物品和点击此装备位一样弹出信息弹窗
    iconFrame->addTouchEventListener(CC_CALLBACK_2(Role::touchEvent, this));
    
//    iconBox->setName("iconBox");
//    iconBox->setTag(itemId*1000+index);
    img->addChild(iconBox);
    img->setVisible(true);
}

void Role::onExit()
{
    this->tabBar->release();
    BaseUI::onExit();
}