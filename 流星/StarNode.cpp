/**
*@file 
*@brief 
*
*详细概述
*
*@author DionysosLai，email: 906391500@qq.com
*@version 1.0
*@data 2014-7-25 14:26
*/
#include "PageScene/StarNode.h"
#include "ParticalNode.h"
#include "SimpleAudioEngine.h"
#include "PageScene/PageMusicConst.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace CocosDenshion;


StarNode::StarNode()
{

}

StarNode::~StarNode()
{

}

StarNode* StarNode::create()
{
	StarNode* pRet = new StarNode();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool StarNode::init()
{
	if (!CCNode::init())
	{
		return false;
	}

	initData();
	return true;
}

void StarNode::initData()
{
	m_pSpStar = CCSprite::create("page9/PIC_XX.png");
	m_pSpStar->setPosition(CCPointZero);
	this->addChild(m_pSpStar);

	m_pSpStar->setOpacity(0);
	CCActionInterval*  action = CCFadeIn::create(1.5f);
	m_pSpStar->runAction(action);

	m_point = m_pSpStar->getPosition();

	m_bStarFly = false;

	m_pMoStar = CCMotionStreak::create(0.5f, 1, 7, ccWHITE, "page9/steak.png" );
	m_pMoStar->setPosition(CCPointZero);
	m_pMoStar->setVisible(false);
	addChild(m_pMoStar);

	setStarOver(false);

	this->scheduleOnce(schedule_selector(StarNode::autorunBeautyAction), CCRANDOM_0_1()*15 + 10.5f);
}

void StarNode::runBeautyAction( float delta )
{
	m_pMoStar->setVisible(true);
//	m_pMoStar->setPosition(CCPointZero);

	this->scheduleUpdate();
	this->unschedule(schedule_selector(StarNode::autorunBeautyAction));
}

void StarNode::autorunBeautyAction( float delta )
{
	m_pMoStar->setVisible(true);
	//	m_pMoStar->setPosition(CCPointZero);
	m_bStarFly = true;

	this->scheduleUpdate();
}

void StarNode::update( float delta )
{
	CCPoint pos = m_pSpStar->getPosition();
	pos.x -= 5;
	pos.y -= 5;

	if (ccpDistance(pos, m_point) > 150)
	{
		/// 加些粒子效果
		ParticalNode* partical = ParticalNode::create("page9/star.plist");
		partical->setPosition(pos);
		this->addChild(partical);
	}

	m_pSpStar->setPosition(pos);
	m_pMoStar->setPosition(pos);

	CCPoint posParent = this->getPosition();
	if (posParent.x + pos.x > 1280 || posParent.x + pos.x < 0 || 
		posParent.y + pos.y < 0)
	{
		this->unscheduleUpdate();
		setStarOver(true);
		m_bStarFly = false;
		m_pMoStar->setVisible(false);
	}
}


void StarNode::dealTouch()
{
	this->scheduleOnce(schedule_selector(StarNode::runBeautyAction), 0.1f);
	SimpleAudioEngine::sharedEngine()->playEffect(EN_PAGE_ME_MELON);
}

bool StarNode::ccTouchBegan( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent )
{
	CCPoint pos;
	pos = pTouch->getLocationInView();
	pos = CCDirector::sharedDirector()->convertToGL(pos);
	pos = this->convertToNodeSpace(pos);

	CCPoint starPos = m_pSpStar->getPosition();
	if (ccpDistance(pos, starPos) < 40 && !m_bStarFly)
	{
		this->dealTouch();
		m_bStarFly = true;
		return true;
	}
	
	return false;
}

void StarNode::ccTouchEnded( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent )
{

}

void StarNode::onEnter()
{
	CCNode::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

void StarNode::onExit()
{
	CCNode::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

void StarNode::setStarPosition( const cocos2d::CCPoint& point )
{
	m_pSpStar->setPosition(point);
	m_pMoStar->setPosition(point);
	setStarOver(false);
}

void StarNode::setStarOver(bool var)
{
	m_bStarOver = var;
}

bool StarNode::getStarOver()
{
	return m_bStarOver;
}