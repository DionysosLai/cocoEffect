/**
*@file SpriteBlur.cpp
*@brief 
*
*ÏêÏ¸¸ÅÊö
*
*@author DionysosLai£¬email: 906391500@qq.com
*@version 1.0
*@data 2014-7-17 11:19
*/
#include "SpriteBlur.h"

USING_NS_CC;

SpriteBlur::~SpriteBlur()
{
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, EVENT_COME_TO_FOREGROUND);
}

SpriteBlur* SpriteBlur::create(const char *pszFileName)
{
	SpriteBlur* pRet = new SpriteBlur();
	if (pRet && pRet->initWithFile(pszFileName))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

void SpriteBlur::listenBackToForeground(CCObject *obj)
{
	setShaderProgram(NULL);
	initProgram();
}

bool SpriteBlur::initWithTexture(CCTexture2D* texture, const CCRect& rect)
{
	if( CCSprite::initWithTexture(texture, rect) ) 
	{
		CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
			callfuncO_selector(SpriteBlur::listenBackToForeground),
			EVENT_COME_TO_FOREGROUND,
			NULL);

		CCSize s = getTexture()->getContentSizeInPixels();

		blur_ = ccp(1/s.width, 1/s.height);
		sub_[0] = sub_[1] = sub_[2] = sub_[3] = 0;

		this->initProgram();

		return true;
	}

	return false;
}

void SpriteBlur::initProgram()
{
	GLchar * fragSource = (GLchar*) CCString::createWithContentsOfFile(
		CCFileUtils::sharedFileUtils()->fullPathForFilename("Shaders/example_Blur.fsh").c_str())->getCString();
	CCGLProgram* pProgram = new CCGLProgram();
	pProgram->initWithVertexShaderByteArray(ccPositionTextureColor_vert, fragSource);
	setShaderProgram(pProgram);
	pProgram->release();

	CHECK_GL_ERROR_DEBUG();

	getShaderProgram()->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
	getShaderProgram()->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
	getShaderProgram()->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);

	CHECK_GL_ERROR_DEBUG();

	getShaderProgram()->link();

	CHECK_GL_ERROR_DEBUG();

	getShaderProgram()->updateUniforms();

	CHECK_GL_ERROR_DEBUG();

	subLocation = glGetUniformLocation( getShaderProgram()->getProgram(), "substract");
	blurLocation = glGetUniformLocation( getShaderProgram()->getProgram(), "blurSize");

	CHECK_GL_ERROR_DEBUG();
}

void SpriteBlur::draw()
{
	ccGLEnableVertexAttribs(kCCVertexAttribFlag_PosColorTex );
	ccBlendFunc blend = getBlendFunc();
	ccGLBlendFunc(blend.src, blend.dst);

	getShaderProgram()->use();
	getShaderProgram()->setUniformsForBuiltins();
	getShaderProgram()->setUniformLocationWith2f(blurLocation, blur_.x, blur_.y);
	getShaderProgram()->setUniformLocationWith4fv(subLocation, sub_, 1);

	ccGLBindTexture2D( getTexture()->getName());

	//
	// Attributes
	//
#define kQuadSize sizeof(m_sQuad.bl)
	long offset = (long)&m_sQuad;

	// vertex
	int diff = offsetof( ccV3F_C4B_T2F, vertices);
	glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*) (offset + diff));

	// texCoods
	diff = offsetof( ccV3F_C4B_T2F, texCoords);
	glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));

	// color
	diff = offsetof( ccV3F_C4B_T2F, colors);
	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));


	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	CC_INCREMENT_GL_DRAWS(1);
}

void SpriteBlur::setBlurSize(float f)
{
	CCSize s = getTexture()->getContentSizeInPixels();

	blur_ = ccp(1/s.width, 1/s.height);
	blur_ = ccpMult(blur_,f);
}

SpriteBlur* SpriteBlur::createWithTexture(CCTexture2D *pTexture)
{
	CCAssert(pTexture != NULL, "Invalid texture for sprite");

	CCRect rect = CCRectZero;
	rect.size = pTexture->getContentSize();

	SpriteBlur* pRet = new SpriteBlur();
	if (pRet && pRet->initWithTexture(pTexture,rect))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

SpriteBlur* SpriteBlur::createWithTexture( cocos2d::CCTexture2D *pTexture, const char* pFile )
{
	CCAssert(pTexture != NULL, "Invalid texture for sprite");

	CCRect rect = CCRectZero;
	rect.size = pTexture->getContentSize();

	SpriteBlur* pRet = new SpriteBlur();
	if (pRet && pRet->initWithTexture(pTexture,rect))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}
