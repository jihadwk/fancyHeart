//
//  InfoPanel.cpp
//  fancyHeart
//
//  Created by doteyplay on 14-8-20.
//
//

#include "InfoPanel.h"

InfoPanel* InfoPanel::create(int status,PNpc pNpc,XSkill*xSkill)
{
    InfoPanel* infoPanel=new InfoPanel();
    if (infoPanel && infoPanel->init("publish/infoPanel/infoPanel.ExportJson",status,pNpc,xSkill)) {
        infoPanel->autorelease();
        return infoPanel;
    }
    CC_SAFE_DELETE(infoPanel);
    return nullptr;
}

bool InfoPanel::init(std::string fileName,int status,PNpc pNpc,XSkill*xSkill)
{
	if(!BaseUI::init(fileName))
    {
        return false;
    }
	//如果需要对cocostudio 设计的ui进行调整
    this->pNpc = pNpc;
    this->xSkill = xSkill;
    this->imgBg = static_cast<Widget*>(layout->getChildByName("imgBg"));
    this->panel1 = static_cast<Widget*>(imgBg->getChildByName("panel1"));
    this->panel2 = static_cast<Widget*>(imgBg->getChildByName("panel2"));
    this->skillFrame = static_cast<Widget*>(imgBg->getChildByName("skillFrame"));
//    Widget* iconFrame2 = static_cast<Widget*>(panel1->getChildByName("iconFrame2"));
    
    this->nameLabel = static_cast<Text*>(imgBg->getChildByName("nameLabel"));
    Text* explainLabel = static_cast<Text*>(this->skillFrame->getChildByName("explainLabel"));
    Button*sureBtn = static_cast<Button*>(imgBg->getChildByName("sureBtn"));
    sureBtn->addTouchEventListener(CC_CALLBACK_2(InfoPanel::touchBtnEvent, this));
    
    this->setPanelVisible();
    if (status == 0||status == 1) {//升阶
        this->panel1->setVisible(true);
        this->setAscend(status);
    }else if(status == 2){//进化
        this->panel2->setVisible(true);
        this->setEvolve(status);
    }else if(status == 3||status == 4){//技能解锁和升级
        this->skillFrame->setVisible(true);
        setSkill(status);
    }
    
	return true;
}

void InfoPanel::onEnter()
{
    BaseUI::onEnter();
}

void InfoPanel::setPanelVisible(){
    this->panel1->setVisible(false);
    this->panel2->setVisible(false);
    this->skillFrame->setVisible(false);
}
//设置升阶数据
void InfoPanel::setAscend(int status)
{
    Text* defAfterLabel = static_cast<Text*>(this->panel1->getChildByName("defAfterLabel"));
    Text* label1= static_cast<Text*>(defAfterLabel->getChildByName("label1"));
    Text* label2= static_cast<Text*>(defAfterLabel->getChildByName("label2"));
    Text* label3= static_cast<Text*>(defAfterLabel->getChildByName("label3"));
    Text* label4= static_cast<Text*>(defAfterLabel->getChildByName("label4"));
    Text* atkLabel= static_cast<Text*>(defAfterLabel->getChildByName("atkLabel"));
    Text* atkAfterLabel= static_cast<Text*>(defAfterLabel->getChildByName("atkAfterLabel"));
    Text* hpLabel= static_cast<Text*>(defAfterLabel->getChildByName("hpLabel"));
    Text* hpAfterLabel= static_cast<Text*>(defAfterLabel->getChildByName("hpAfterLabel"));
    Text* defLabel= static_cast<Text*>(defAfterLabel->getChildByName("defLabel"));
    Text* mDefLabel= static_cast<Text*>(defAfterLabel->getChildByName("mDefLabel"));
    Text* mDefAfterLabel= static_cast<Text*>(defAfterLabel->getChildByName("mDefAfterLabel"));
    Widget* skillFra= static_cast<Widget*>(this->panel1->getChildByName("skillFra"));
    Text* explainLabel= static_cast<Text*>(skillFra->getChildByName("explainLabel"));
    if(status == 0){//0:品阶等级等于1、3、6、9
        skillFra->setVisible(true);
        //技能属性的显示
//        explainLabel->setString(this->xSkill->getDesc());
    }else if(status == 1){//1:品阶等级等于2、4、5、7、8
        skillFra->setVisible(false);
    }
    this->nameLabel->setString("升阶成功");
    //人物颜色框
    std::vector<std::string> colors = {"1.png","2.png","2.png","3","3.png",
        "3.png","4.png","4.png","4.png","5.png"};
    
    ImageView* panel1=static_cast<ImageView*>(this->imgBg->getChildByName("panel1"));
    panel1->loadTexture("frame_"+Value(colors[this->pNpc.quality()-1]).asString(),TextureResType::PLIST);
    ImageView* iconFrame2 = static_cast<ImageView*>(this->panel1->getChildByName("iconFrame2"));
    iconFrame2->loadTexture("frame_"+Value(colors[this->pNpc.quality()]).asString(),TextureResType::PLIST);
    
    //升阶前数据
    XRoleData*xRoleDataBf = XRoleData::record(Value(Value(this->pNpc.spriteid()).asString()+Value(this->pNpc.quality()-1).asString()));
    XRoleData*xRoleDataAf = XRoleData::record(Value(Value(this->pNpc.spriteid()).asString()+Value(this->pNpc.quality()).asString()));
    label1->setString("攻击");
    atkLabel->setString(Value(xRoleDataBf->getAtk()).asString());
    atkAfterLabel->setString(Value(xRoleDataAf->getAtk()).asString());
    
    label2->setString("生命");
    hpLabel->setString(Value(xRoleDataBf->getHp()).asString());
    hpAfterLabel->setString(Value(xRoleDataAf->getHp()).asString());
    
    label3->setString("物防");
    defLabel->setString(Value(xRoleDataBf->getDf()).asString());
    defAfterLabel->setString(Value(xRoleDataAf->getDf()).asString());
    
    label4->setString("法防");
    mDefLabel->setString(Value(xRoleDataBf->getMDf()).asString());
    mDefAfterLabel->setString(Value(xRoleDataAf->getMDf()).asString());
    
}
//设置进化数据
void InfoPanel::setEvolve(int status)
{
    this->nameLabel->setString("进化成功");
    ImageView* frame1= static_cast<ImageView*>(this->panel2->getChildByName("frame1"));
    ImageView* frame2= static_cast<ImageView*>(this->panel2->getChildByName("frame2"));
    //人物颜色框
    std::vector<std::string> colors = {"1.png","2.png","2.png","3","3.png",
        "3.png","4.png","4.png","4.png","5.png"};
    
    frame1->loadTexture("frame_"+Value(colors[this->pNpc.quality()-1]).asString(),TextureResType::PLIST);
    frame2->loadTexture("frame_"+Value(colors[this->pNpc.quality()]).asString(),TextureResType::PLIST);
    
    XRoleData*xRoleDataBf = XRoleData::record(Value(Value(this->pNpc.spriteid()).asString()+Value(this->pNpc.quality()-1).asString()));
    XRoleData*xRoleDataAf = XRoleData::record(Value(Value(this->pNpc.spriteid()).asString()+Value(this->pNpc.quality()).asString()));
    Text* atkAdd= static_cast<Text*>(this->panel2->getChildByName("label1"));
    Text* hpAdd= static_cast<Text*>(this->panel2->getChildByName("label2"));
    Text* defAdd= static_cast<Text*>(this->panel2->getChildByName("label3"));
    Text* mDefAdd= static_cast<Text*>(this->panel2->getChildByName("label4"));
    Text* atk= static_cast<Text*>(this->panel2->getChildByName("atkLabel"));
    Text* atkAfter= static_cast<Text*>(this->panel2->getChildByName("atkAfterLabel"));
    Text* hp= static_cast<Text*>(this->panel2->getChildByName("hpLabel"));
    Text* hpAfter= static_cast<Text*>(this->panel2->getChildByName("hpAfterLabel"));
    Text* def= static_cast<Text*>(this->panel2->getChildByName("defLabel"));
    Text* defAfter = static_cast<Text*>(this->panel2->getChildByName("defAfterLabel"));
    Text* mDef= static_cast<Text*>(this->panel2->getChildByName("mDefLabel"));
    Text* mDefAfter= static_cast<Text*>(this->panel2->getChildByName("mDefAfterLabel"));
    atk->setString(Value(xRoleDataBf->getAtk()).asString());
    atkAfter->setString(Value(xRoleDataAf->getAtk()).asString());
    hp->setString(Value(xRoleDataBf->getHp()).asString());
    hpAfter->setString(Value(xRoleDataAf->getHp()).asString());
    def->setString(Value(xRoleDataBf->getDf()).asString());
    defAfter->setString(Value(xRoleDataAf->getDf()).asString());
    mDef->setString(Value(xRoleDataBf->getMDf()).asString());
    mDefAfter->setString(Value(xRoleDataAf->getMDf()).asString());
//    atkAdd->setString();
//    hpAdd->setString();
//    defAdd->setString();
//    mDefAdd->setString();
    
}
//设置技能解锁和升阶界面
void InfoPanel::setSkill(int status)
{
    if(status == 3){
        this->nameLabel->setString("觉醒技解锁");
    }else if (status == 4){
       this->nameLabel->setString("技能升级");
    }
    Widget* skillIcon = static_cast<Widget*>(this->skillFrame->getChildByName("skillIcon"));
    Text* skillInfo= static_cast<Text*>(this->skillFrame->getChildByName("skillInfo"));
    Text* levelPercentLabel= static_cast<Text*>(this->skillFrame->getChildByName("levelPercentLabel"));
    skillInfo->setString(Value(this->xSkill->getDesc()).asString());
    //技能等级的显示
//    levelPercentLabel->setString();
}

void InfoPanel::touchBtnEvent(Ref *pSender, TouchEventType type)
{
    Button* btn=static_cast<Button*>(pSender);
    if (!btn) {
        return;
    }
    
    if (btn->getTag() == 12725) {//确定按钮
        this->clear(true);
    }
}

//float InfoPanel::getQualityData(int baseNum,int equipAddNum)
//{
//    //本品质基础数据 + 装备 + 等级 X（本品质成长率+星级成长率）
//}

void InfoPanel::onExit()
{
    BaseUI::onExit();
}