/**
*@file 
*@brief 擦除效果实现
*
*详细概述
*
*@author DionysosLai，email: 906391500@qq.com
*@version 1.0
*@data 2014-8-22 16:15
*/
#include "EraserSprite.h"

using namespace cocos2d;


EraserSprite::EraserSprite()
{

}

EraserSprite::~EraserSprite()
{
	m_pEraser->release();
}

EraserSprite* EraserSprite::create( const char *pszFileName, const bool& eraserImmediately, 
								   const unsigned int& drawType)
{
	EraserSprite* pRet = new EraserSprite();
	if (pRet && pRet->init(pszFileName, eraserImmediately, drawType))
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

bool EraserSprite::init( const char *pszFileName, const bool& eraserImmediately, 
						const unsigned int& drawType )
{
	if (!CCSprite::init())
	{
		return false;
	}

	initData(pszFileName, eraserImmediately, drawType);
	return true;
}

void EraserSprite::onEnter()
{
	CCSprite::onEnter();
	
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}


void EraserSprite::onExit()
{
	CCSprite::onExit();

	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool EraserSprite::ccTouchBegan( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent )
{
	if (m_bEraser)
	{
		CCPoint point = pTouch->getLocation(); 

		/// 注意加上spriteSize/2.f，这是因为我们之前见精灵中心点坐标设置在(0,0);
/*		m_pEraser->setPosition(-this->getPosition() + point + spriteSize/2.f);*/
		eraseByBlend(-this->getPosition() + point + spriteSize/2.f);
		m_touchPoint = point;

		return true;
	}

	return false;
}

void EraserSprite::ccTouchMoved( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent )
{
	if (m_bEraser)
	{
		CCPoint point = pTouch->getLocation(); 
		CCPoint normal = ccpNormalize(point-m_touchPoint);

		/// 处理一次移动过多，造成中间有遗漏，或者锯齿现象；
		while(1)
		{
			if (ccpDistance(point, m_touchPoint) < 1.f)
			{
				/*		m_pEraser->setPosition(-this->getPosition() + point + spriteSize/2.f);*/
				eraseByBlend(-this->getPosition() + point + spriteSize/2.f);
				break;
			}
			m_touchPoint = m_touchPoint + normal*1.f;
			
			/*		m_pEraser->setPosition(-this->getPosition() + m_touchPoint + spriteSize/2.f);*/
			eraseByBlend(-this->getPosition() + m_touchPoint + spriteSize/2.f);
		}

		m_touchPoint = point;
	}
}

void EraserSprite::initData( const char *pszFileName, const bool& eraserImmediately, 
							const unsigned int& drawType )
{
	m_bEraser		= eraserImmediately;
	m_uiDrawType	= drawType;
	m_fDrawWidth	= 10.f;


	CCSprite* sprite = CCSprite::create(pszFileName);
	spriteSize = sprite->getContentSize();
	/// 将精灵加入纹理后，其中心点坐标应该设置在（0,0）处， 这是由于纹理的中心点在（0,0），当然，可以通过设置其偏移坐标实现；
	sprite->setAnchorPoint(ccp(0.f, 0.f));
//	sprite->setPosition(ccp(spriteSize.width/2.f, spriteSize.height/2.f));

	m_pRTex = CCRenderTexture::create(spriteSize.width, spriteSize.height);
	m_pRTex->setPosition(CCPointZero);
	this->addChild(m_pRTex);

	m_pRTex->begin();
	sprite->visit();
	m_pRTex->end();

	m_pEraser = CCDrawNode::create();
	setDrawWidth(10.f);
	m_pEraser->retain();

	/// 自定义图片
	m_drawTextture = new cocos2d::CCTexture2D();
	m_drawTextture->retain();

}

void EraserSprite::setEraser(bool var)
{
	this->m_bEraser = var;
}

bool EraserSprite::getEraser()
{
	return m_bEraser;
}

///@brief 利用混合模式，实现擦除效果
///@param[in/out] 
///@pre 注意，混合条件为：源---1，目标---0
///@return 
///@author DionysosLai,906391500@qq.com 
///@retval  
///@post 
///@version 1.0 
///@data 2014-8-25 10:40
void EraserSprite::eraseByBlend(cocos2d::CCPoint& point)
{
	switch (m_uiDrawType)
	{
	case 2:
		{
			CCSprite* drawSprite = CCSprite::createWithTexture(m_drawTextture);
			drawSprite->setPosition(point);
			ccBlendFunc blendFunc = { GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA };	///< 设置混合模式, 源---1-alpha， 目标---alpha
			drawSprite->setBlendFunc(blendFunc);
			m_pRTex->begin();
			drawSprite->visit();
			m_pRTex->end();
		}
		break;
	default:
		{
			m_pEraser->setPosition(point);
			ccBlendFunc blendFunc = { GL_ONE, GL_ZERO };	///< 设置混合模式, 源---1， 目标---0
			m_pEraser->setBlendFunc(blendFunc);
			m_pRTex->begin();
			m_pEraser->visit();
			m_pRTex->end();
		}
		break;
	}
}

void EraserSprite::eraseByColorMask()
{
	m_pRTex->begin();
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
	m_pEraser->visit();
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	m_pRTex->end();
}

void EraserSprite::setDrawWidth(float var)
{
	this->m_fDrawWidth = var;

	switch (m_uiDrawType)
	{
	case 0:	///< 正方形模式
		{
			m_pEraser->drawDot(CCPointZero, m_fDrawWidth, ccc4f(0,0,0,0));
		}
		break;
	case 1:
		{
			/// 绘制圆形区域
			float fRadius		= m_fDrawWidth;							///< 圆的半径
			const int nCount	= 100;							///< 用正100边型来模拟园
			const float coef	= 2.0f * (float)M_PI/nCount;	///< 计算每两个相邻顶点与中心的夹角
			static CCPoint circle[nCount];						///< 顶点数组
			for(unsigned int i = 0;i <nCount; i++) {
				float rads = i*coef;							///< 弧度
				circle[i].x = fRadius * cosf(rads);				///< 对应顶点的x
				circle[i].y = fRadius * sinf(rads);				///< 对应顶点的y
			}
			m_pEraser->drawPolygon(circle, nCount, ccc4f(0, 0, 0, 0), 0, ccc4f(0, 0, 0, 0));//绘制这个多边形!
		}
		break;
	case 2:
		{
			/// 需要自定义擦除图片
		}
		break;
	default:
		break;
	}
}

float EraserSprite::getDrawWidth()
{
	return this->m_fDrawWidth;
}

void EraserSprite::setEraserOk(bool var)
{
	this->m_bEraserOk = var;
}

bool EraserSprite::getEraserOk()
{
	m_bEraserOk = false;

	CCImage* image = new CCImage();
	image = m_pRTex->newCCImage(true);

	int m = 3;
	if (image->hasAlpha())
	{
		m = 4;
	}

	unsigned char *data_= image->getData();
	int x = 0, y = 0;
	/// 这里要一点，即Opengl下，其中心点坐标在左上角
	for (x = 0; x < spriteSize.width; ++x)
	{
		for (y = 0 ; y < spriteSize.height; ++y)
		{

			unsigned char *pixel = data_ + (x + y * image->getWidth()) * m;

			// You can see/change pixels' RGBA value(0-255) here !
			unsigned int r = (unsigned int)*pixel;
			unsigned int g = (unsigned int)*(pixel + 1);
			unsigned int b = (unsigned int)*(pixel + 2) ;
			unsigned int a = (unsigned int)*(pixel + 3);

			if (r != 0 && g != 0 && b != 0 && a != 0)
			{
				m_bEraserOk = false;
				break;
			}
		}
		if (spriteSize.height != y)
		{
			break;
		}
	}
	if (x == spriteSize.width && y == spriteSize.height)
	{
		m_bEraserOk = true;
	}

	delete image;

	return this->m_bEraserOk;
}

///@brief 自定义擦除形状
///@param[in/out] 
///@pre 图片要求中间形状是镂空的。
///@return 
///@author DionysosLai,906391500@qq.com 
///@retval  
///@post 
///@version 1.0 
///@data 2014-8-27 12:13
void EraserSprite::setDrawStencil( const char *pszFileName )
{
	m_drawTextture = CCTextureCache::sharedTextureCache()->addImage(pszFileName);
}
