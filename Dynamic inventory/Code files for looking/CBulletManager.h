#pragma once
#include <tgCSingleton.h>
#include	<tgMemoryDisable.h>
#include	<vector>
#include	<tgMemoryEnable.h>
#include    <tgCV3D.h>
#include    <tgCSphere.h>
struct SBullet
{
    tgCV3D      _Dir;
    tgFloat     _Speed;
    tgFloat     _Lifetime;
    tgCSphere   _Sphere;
};
class CBulletManager :
    public tgCSingleton<CBulletManager>
{
public:
    CBulletManager();
    ~CBulletManager();

    void NewBullet(SBullet* NewBullet);
    void Update(const tgFloat Deltatime);

private:

    std::vector<SBullet*> m_Bullets;

};

