/**
*@file SpriteBlur.h
*@brief 实现高斯模糊
*
*详细概述
*
*@author DionysosLai，email: 906391500@qq.com
*@version 1.0
*@data 2014-7-17 11:16
*/
#ifndef __SPRITE_BLUR_H__
#define __SPRITE_BLUR_H__

#include "cocos2d.h"

class SpriteBlur : public cocos2d::CCSprite
{
public:
	~SpriteBlur();
	void setBlurSize(float f);
	bool initWithTexture(cocos2d::CCTexture2D* texture, const cocos2d::CCRect&  rect);
	static SpriteBlur* createWithTexture(cocos2d::CCTexture2D *pTexture);
	static SpriteBlur* createWithTexture(cocos2d::CCTexture2D *pTexture, const char* pFile);
	void draw();
	void initProgram();
	void listenBackToForeground(CCObject *obj);

	static SpriteBlur* create(const char *pszFileName);

	cocos2d::CCPoint blur_;
	GLfloat    sub_[4];

	GLuint    blurLocation;
	GLuint    subLocation;
};

#endif	///<(__SPRITE_BLUR_H__)