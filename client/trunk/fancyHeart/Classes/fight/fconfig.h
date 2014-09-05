//
//  fconfig.h
//  fancyHeart
//
//  Created by 秦亮亮 on 14-8-14.
//
//

#ifndef fancyHeart_fconfig_h
#define fancyHeart_fconfig_h

#define RANDOM_0_1() ((float)rand()/RAND_MAX)
#define HERO_DIE_TAG 2000
#define GRID_SIZE 40.0
#define ACTION_RUN_TAG 100
#define ACTION_SHOOT_TAG 101


#define BEAN_NUM 2

enum fstate{
    idle, //空闲
    run,
    start, //开始攻击
    spell, //吟唱
    buildup, //蓄力
//    throwing, //仍东西
    cast,  //正式攻击
    //    attacked,
    die, //死亡
    over //结束战斗
};



#endif