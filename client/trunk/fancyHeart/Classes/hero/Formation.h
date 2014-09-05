//
//  Formation.h
//  fancyHeart
//
//  Created by zhai on 14-6-18.
//
//

#ifndef __fancyHeart__Formation__
#define __fancyHeart__Formation__

#include <iostream>
#include <map>
#include "cocos2d.h"
#include "Manager.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BaseUI.h"
#include "JValue.h"
#include "XGate.h"
#include "XNode.h"
#include "GateSelect.h"
#include "Mask.h"
#include "RotateList.h"
#include "NewPageView.h"
USING_NS_CC;
using namespace ui;
using namespace cocostudio;
struct FormationData{
    int64 npcId;
    int isFormation;
};
struct ColorData{
    int color;
    std::string text;
};
class Formation:public BaseUI
{
   
public:
	static Formation* create(BaseUI* delegate);
	virtual bool init(std::string fileName);
    virtual void resetUI();
    virtual void onEnter();
    virtual void onExit();
private:
    RotateList* rotateList;
    PageView* pageView;
	void touchEvent(Ref *pSender, TouchEventType type);
    void touchButtonEvent(Ref *pSender, TouchEventType type);
    void pageViewEvent(Ref *pSender, PageView::EventType type);
    void rotateListCallback(RotateList::EventType type,Widget*item,int index);
    void initNetEvent();
    void setListMask(int index);
    //临时组
    std::vector<std::vector<FormationData >> groups;
    std::vector<FormationData > npcs;
    //组变化npcId要上阵或者下阵的npcid，groupIndex当前要下阵的组 isFormation 上阵还是下阵
    void changeGroup(int64 npcId,int groupIndex,bool isFormation);
    //初始化group数据
    void initGroup(int groupIndex,bool isInit=true);
    void displayAction(Widget* widget,bool isDown);
    int currentPos=-1;
    void initList();//站位 0前排1中排2后排－1全部
    //排序
    static bool sortIds(FormationData fd1,FormationData fd2);
    //判断某npc是否在阵上
    bool isInFormation(int64 npcId);
    //更新组
    void updateGroups(const std::vector<std::vector<FormationData>> groups);
    bool isSave=false;
    int currentPageIndex=0;
    std::vector<ColorData> qualitys={
        ColorData{1,""},
        ColorData{2,""},ColorData{2,"+1"},
        ColorData{3,""},ColorData{3,"+1"},ColorData{3,"+2"},
        ColorData{4,""},ColorData{4,"+1"},ColorData{4,"+2"},
        ColorData{5,""}};//颜色0白 1绿 2绿+1 3蓝 4蓝+1 5蓝+2 6紫 7紫+1 8紫+2 9金
};
#endif /* defined(__fancyHeart__Formation__) */