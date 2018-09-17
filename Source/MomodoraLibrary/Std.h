#pragma once
#include <windows.h>
#include <cassert>
#include <tchar.h>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <cstdlib>
#include <cmath>
#include <sstream>
#include <fstream>
#include <array>

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"msimg32.lib")
#pragma comment(lib,"MomodoraLibrary.lib")
#pragma comment(lib,"fmod64_vc.lib")


#define new new(__FILE__,__LINE__)
#define PI 3.141592
#define DegreeToRadian(x) (x) * PI / 180.0f

struct MEMINFO
{
	std::string  filename;
	INT			 line;
	DWORD		 dwAllocateTime;
	void*		 addr;
};

using T_STR = std::basic_string<TCHAR>;
using MEM_MAP = std::map<void *, MEMINFO>;
using RECT_VECTOR = std::vector<RECT>;

enum class KEYSTATE : unsigned char
{
	KEY_FREE,
	KEY_PUSH,
	KEY_HOLD,
	KEY_UP
};
enum class SCENE : unsigned char
{
	LOBBY,
	GAME,
	END
};
enum class INVERSE : unsigned char
{
	LR_ROTATION,
	TB_ROTATION,
	LRTB_ROTATION,
	DEFAULT
};
enum class LOBBYSTATE : unsigned char
{
	DEFAULT,
	START,
	MAINMENU,
	SETTING,
	KEYSETTING,
	SELECT
};

template <typename K>
class Singleton
{
protected:
	Singleton() {};
public:
	static K& getInstance()
	{
		static K inst;
		return inst;
	}
};

struct FloatPoint
{
	FLOAT x, y;
};
struct Sphere
{
	FLOAT fRadius;
	POINT CenterPos;
};
struct FloatRect
{
	FLOAT left, top, right, bottom;
};


extern HWND					g_hWnd;
extern HINSTANCE            g_hInstance;
extern HDC					g_hOffScreenDC;
extern HDC					g_hScreenDC;
extern RECT					g_rtClient;
extern float				g_fSpeed;
extern float				g_fSecPerFrame;
extern bool					g_bActiveApp;

extern INT					g_HP;
extern INT					g_inewCount;
extern MEM_MAP				MemoryMap;
extern FLOAT				g_EffectVolume;
extern FLOAT				g_BGMVolume;

class UI;

extern UI *					g_UI;
extern UI *					g_HPBar;

class Player;

extern Player*				g_pPlayer;

//	KahoAttack1 7
//	KahoAttack2 7
//	KahoAttack3 11
//	AirAttack 7
//	LandAttack 7
//	Leaf1 7
//	Leaf2 7
//	Leaf3 9
//	AirLeaf 6
//	Bow 6
//	Airbow 6
//	CrouchBow 6
//	Run 10
//	Brake 7
//	Turn 3
//	Crouch 4
//	Rise 2
//	Death 24
//	Defensive 3
//	EndingDeath 35
//	EndingDeath2 28
//	Jump  3
//	Fall 5
//	ToPraying  6
//	Praying 4
//	FromPraying 9
//	HeadBob 4
//	HeadBobBack 4
//	Hurt 1
//	Idle 6
//	Look 14
//	Item 11
//	LadderUp 6
//	LadderDown 3
//	LadderEnter 6
//	LadderLeave 6
//	Roll 8
//	MiscEffect 3
//	Shine 1
//	Icon 1
//	ArrowS 1
//	ArrowL 1