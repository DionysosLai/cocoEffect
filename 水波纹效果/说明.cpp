1. 这里添加了高斯模糊文件，后来发现水波纹效果不能与模糊效果叠加，一时也不想改代码了，因此将高斯模糊代码一起放进来；
2. 注意水波纹的图片中心点在左下角，这个与我们普通精灵中心点有区别，注意；
3. 通过函数void addRipple(const CCPoint& pos);可以在指定位置添加一道水波纹；当然也可以通过点击方式，添加水波纹；
4. 由于这里面的水波纹是通过sin等三角函数计算，因此性能不好，我将滑动效果全部关闭了，如果想要打开滑动效果，可以在函数：

bool pgeRippleSprite::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!this->isTouchInsideSprite(pTouch)) {
        return false;------这里讲将false---改成true即可(*^__^*)
    }
    
    this->ccTouchMoved(pTouch, pEvent);
    return false;
}