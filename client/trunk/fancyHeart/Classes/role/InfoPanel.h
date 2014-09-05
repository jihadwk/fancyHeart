//
//  InfoPanel.h
//  fancyHeart
//
//  Created by doteyplay on 14-8-20.
//
//

#ifndef __fancyHeart__InfoPanel__
#define __fancyHeart__InfoPanel__

#include <iostream>
#include "cocos2d.h"
#include "Manager.h"
#include "BaseUI.h"
#include "XRoleData.h"
#include "XSkill.h"

using namespace ui;
using namespace cocos2d;
using namespace cocostudio;

class InfoPanel:public BaseUI
{
public:
    //status:0和1是升阶,2是进化，3是技能解锁，4:技能升级
    //0:品阶等级等于1、3、6、9
    //1:品阶等级等于2、4、5、7、8
	static InfoPanel* create(int status,PNpc pNpc,XSkill*xSkill = nullptr);
	virtual bool init(std::string fileName,int status,PNpc pNpc,XSkill*xSkill = nullptr);
	virtual void onEnter();
	virtual void onExit();

private:
    void touchBtnEvent(Ref *pSender, TouchEventType type);
    Widget*panel1;
    Widget*panel2;
    Widget*skillFrame;
    Text*nameLabel;

private: //私有属性
 	BaseUI* preUI;
    void setPanelVisible();
    void setAscend(int status);
    void setEvolve(int status);
    void setSkill(int status);
    Widget* imgBg;
    PNpc pNpc;
    XSkill*xSkill;
//    float getQualityData();

};
#endif /* defined(__fancyHeart__InfoPanel__) */