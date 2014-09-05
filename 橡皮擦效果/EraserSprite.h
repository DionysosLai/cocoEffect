/**
*@file 
*@brief 擦除效果实现
*
*
*
*@author DionysosLai，email: 906391500@qq.com
*@version 1.0
*@data 2014-8-22 15:58
*/

#ifndef __ERASER_SPRITE_H__
#define __ERASER_SPRITE_H__

#include "cocos2d.h"

class EraserSprite : public cocos2d::CCSprite, public cocos2d::CCTargetedTouchDelegate
{
public:
	EraserSprite();
	~EraserSprite();

///@brief 创建擦除精灵
///@param[in] pszFileName--精灵文件名, eraserImmediately---是否可以立即擦除 true---可以， false---不能
///@param[in] drawType---擦除形状， 0---正方形，1---圆形， 2---自定义形状(可用一张图片代替)
///@pre 
///@return 
	static EraserSprite* create(const char *pszFileName, const bool& eraserImmediately = true, 
		const unsigned int& drawType = 0);

	virtual bool init(const char *pszFileName, const bool& eraserImmediately = true, 
		const unsigned int& drawType = 0);

	virtual void onEnter();
	virtual void onExit();

	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

public:
	CC_PROPERTY(bool, m_bEraser, Eraser);			///< 设置是否可以擦除
	CC_PROPERTY(float, m_fDrawWidth, DrawWidth);	///< 擦除大小，默认为10.f;
	CC_PROPERTY(bool, m_bEraserOk, EraserOk);		///< 是否擦除完毕

///@brief 自定义擦除形状
///@param[in/out] 
///@pre 图片要求中间形状是镂空的。
///@return 
	void setDrawStencil(const char *pszFileName);	
private:
///@brief 利用混合模式，实现擦除效果
///@param[in/out] 
///@pre 注意，混合条件
///@return 
	void eraseByBlend(cocos2d::CCPoint& point);
	void eraseByColorMask();

private:
	void initData(const char *pszFileName, const bool& eraserImmediately = true, 
		const unsigned int& drawType = 0);

	cocos2d::CCSize spriteSize;				///< 精灵大小
	unsigned int m_uiDrawType;				///< 擦除形状 0---正方形， 1---圆形 2--自定义形状

	cocos2d::CCPoint m_touchPoint;			///< 触摸点
	cocos2d::CCTexture2D* m_drawTextture;	///< 自定义图片时，设置的擦除图片
	
	cocos2d::CCRenderTexture* m_pRTex;		///< 渲染纹理
	cocos2d::CCDrawNode* m_pEraser;			///< 渲染节点
};
	
#endif	///< __ERASER_SPRITE_H__