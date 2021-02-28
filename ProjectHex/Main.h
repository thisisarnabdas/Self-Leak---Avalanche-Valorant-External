#pragma once
/*
discord: heashey#0001
*/
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <tlhelp32.h>
#include <thread>

#include "Protect.h"
#include "config.h"
#include "Driver.h"

#include <dwmapi.h>
#include <comdef.h> 
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include <map>
#include <algorithm>
#include <random>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dwmapi.lib")

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_dx9.h"
#include "Imgui/imgui_impl_win32.h"

#include "font.h"

#include "Text.h"
#include "math.h"
#include "DrawEsp.h"

IDirect3D9Ex* p_Object = NULL;
IDirect3DDevice9Ex* p_Device = NULL;
D3DPRESENT_PARAMETERS p_Params = { NULL };

using namespace std;

const MARGINS Margin = { -1 };

#define M_Name L" "
HWND MyWnd = NULL;
HWND GameWnd = NULL;
RECT GameRect = { NULL };
MSG Message = { NULL };

DWORD ScreenCenterX = NULL;
DWORD ScreenCenterY = NULL;
bool showimgui = true;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define TopWindowGame 11
#define TopWindowMvoe 22

HRESULT DirectXInit(HWND hWnd);
LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
void SetupWindow();
WPARAM MainLoop();
void CleanuoD3D(); void ChangeClickability(bool canclick, HWND ownd);

int isTopwin();
void Render(int Topwin, bool showimgui);
void SetWindowToTarget();


struct FQuat
{
	float x;
	float y;
	float z;
	float w;
};
struct FTransform
{
	FQuat rot;
	Vector3 translation;
	char pad[4];
	Vector3 scale;
	char pad1[4];
	D3DMATRIX ToMatrixWithScale()
	{
		D3DMATRIX m;
		m._41 = translation.x;
		m._42 = translation.y;
		m._43 = translation.z;

		float x2 = rot.x + rot.x;
		float y2 = rot.y + rot.y;
		float z2 = rot.z + rot.z;

		float xx2 = rot.x * x2;
		float yy2 = rot.y * y2;
		float zz2 = rot.z * z2;
		m._11 = (1.0f - (yy2 + zz2)) * scale.x;
		m._22 = (1.0f - (xx2 + zz2)) * scale.y;
		m._33 = (1.0f - (xx2 + yy2)) * scale.z;

		float yz2 = rot.y * z2;
		float wx2 = rot.w * x2;
		m._32 = (yz2 - wx2) * scale.z;
		m._23 = (yz2 + wx2) * scale.y;

		float xy2 = rot.x * y2;
		float wz2 = rot.w * z2;
		m._21 = (xy2 - wz2) * scale.y;
		m._12 = (xy2 + wz2) * scale.x;

		float xz2 = rot.x * z2;
		float wy2 = rot.w * y2;
		m._31 = (xz2 + wy2) * scale.z;
		m._13 = (xz2 - wy2) * scale.x;

		m._14 = 0.0f;
		m._24 = 0.0f;
		m._34 = 0.0f;
		m._44 = 1.0f;

		return m;
	}
};
D3DMATRIX MatrixMultiplication(D3DMATRIX pM1, D3DMATRIX pM2)
{
	D3DMATRIX pOut;
	pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
	pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
	pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
	pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
	pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
	pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
	pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
	pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
	pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
	pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
	pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
	pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
	pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
	pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
	pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
	pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;

	return pOut;
}

FTransform GetBoneIndex(uint64_t mesh, int64_t index)
{
	uint64_t bonearray = Driver::read<uint64_t>(pid, mesh + 0x518);
	if (!bonearray) bonearray = Driver::read<uint64_t>(pid, mesh + 0x528);
	return Driver::read<FTransform>(pid, bonearray + (index * 0x30));
}
Vector3 GetBoneWithRotation(DWORD_PTR mesh, int id)
{
	FTransform bone = GetBoneIndex(mesh, id);
	FTransform ComponentToWorld = Driver::read<FTransform>(pid, mesh + 0x250);

	D3DMATRIX Matrix;
	Matrix = MatrixMultiplication(bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());

	return Vector3(Matrix._41, Matrix._42, Matrix._43);
}

#define M_PI	3.1415926535
D3DXMATRIX ToMatrix(Vector3 Rotation, Vector3 origin = Vector3(0, 0, 0));
D3DXMATRIX ToMatrix(Vector3 Rotation, Vector3 origin)
{
	float Pitch = (Rotation.x * float(M_PI) / 180.f);
	float Yaw = (Rotation.y * float(M_PI) / 180.f);
	float Roll = (Rotation.z * float(M_PI) / 180.f);

	float SP = sinf(Pitch);
	float CP = cosf(Pitch);
	float SY = sinf(Yaw);
	float CY = cosf(Yaw);
	float SR = sinf(Roll);
	float CR = cosf(Roll);

	D3DXMATRIX Matrix;
	Matrix._11 = CP * CY;
	Matrix._12 = CP * SY;
	Matrix._13 = SP;
	Matrix._14 = 0.f;

	Matrix._21 = SR * SP * CY - CR * SY;
	Matrix._22 = SR * SP * SY + CR * CY;
	Matrix._23 = -SR * CP;
	Matrix._24 = 0.f;

	Matrix._31 = -(CR * SP * CY + SR * SY);
	Matrix._32 = CY * SR - CR * SP * SY;
	Matrix._33 = CR * CP;
	Matrix._34 = 0.f;

	Matrix._41 = origin.x;
	Matrix._42 = origin.y;
	Matrix._43 = origin.z;
	Matrix._44 = 1.f;

	return Matrix;
}
Vector3 WorldToScreen(Vector3 world_location, Vector3 position, Vector3 rotation, float fov)
{
	Vector3 screen_location = Vector3(0, 0, 0);

	D3DMATRIX tempMatrix = ToMatrix(rotation);

	Vector3 vAxisX, vAxisY, vAxisZ;

	vAxisX = Vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
	vAxisY = Vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
	vAxisZ = Vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

	Vector3 vDelta = world_location - position;
	Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

	if (vTransformed.z < 1.f)
		vTransformed.z = 1.f;

	float FovAngle = fov;
	float ScreenCenterX = Width / 2.0f;
	float ScreenCenterY = Height / 2.0f;

	screen_location.x = ScreenCenterX + vTransformed.x * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;
	screen_location.y = ScreenCenterY - vTransformed.y * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;

	return screen_location;
}

typedef struct _TslEntity
{
	uint64_t pObjPointer;
	int ID;

	uint64_t Mesh;
	uint64_t PlayerController;
	uint64_t DamageController;
	uint64_t PlayerState;

}TslEntity;
vector<TslEntity> entityList;

uint64_t camera_manager;
uint64_t pawn;
Vector3 localrelative_component;
int local_teamindex;
uint64_t playercontroller;

char* drawBuff = (char*)malloc(1024);
D3DXVECTOR4 Rect;
float AimSpeed = 2.755;

bool Distance = false;
bool Skeleton = false;
bool Fov = false;
int AimFOV = 100;
bool Aimbot = false;
bool aimIng = false;
bool F6 = false;
bool show = false;
ImFont* SkeetFont;
bool menu_key = false;
static int keystatus = 0;
static int realkey = 0;

namespace aimbot
{
	bool aimbot = false;
	bool smooth = false;
	bool fovcircle = false;

	int smoothscale;
	float aimspeed;
	float aimfov;

	int hitbox = 0;
	int aimmode = 0;
}
namespace visuals
{
	bool name = false;

	bool box = false;
	bool skel = false;
	bool health = false;
	bool outline = false;
	bool lines = false;
	bool team = false;
	bool team_name = false;
	bool show_head = false;
	bool health_based = false;

	int boxMode = 0;
	int skelMode = 0;
	int lineMode = 0;
	int healthMode = 0;

	int NumberofEntity = 0;
	int SpectatorCount = 0;

	int MaxDistance = 1000;
}

namespace colors
{
	float chamscol[3] = { 1.0f , 1.0f , 1.0f };
	float espcol[3] = { 1.0f , 1.0f , 1.0f };
	float skelcol[3] = { 1.0f , 1.0f , 1.0f };
	float crosscol[3] = { 1.0f , 10.0f , 0.0f };

	float orecol[3] = { 1.0f , 1.0f , 1.0f };
	float collcol[3] = { 1.0f , 1.0f , 1.0f };
	float cratecol[3] = { 1.0f , 1.0f , 1.0f };
	float scientistcol[3] = { 1.0f , 1.0f , 1.0f };
	float itemscol[3] = { 1.0f , 1.0f , 1.0f };
}
namespace hotkeys
{
	int aimkey;
	int airstuckey;
}

enum Bones : int
{
	Pelvis = 3,
	Head = 8,
	Neck = 7
};

enum Gender : int
{
	Female = 0,
	Male = 1,
};

static const char* Aimmode[] =
{
	"Memory",
	"Memory v2",
};

static const char* Hitbox[] =
{
	"Head",
	"Neck",
	"Chest",
	"Dick"
};
static const char* boxStyle[] =
{
	"2D",
	"2D Filled",
	"2D Corner",
	"Avalanche Fill",
};
static const char* healthMode[] =
{
	"Left Bar",
	"Right Bar",
};
static const char* linesMode[] =
{
	"Top",
	"Middle",
	"Bottom",
	//"Aim Fov"
};

namespace menu
{
	float box_thick;
	float font_size;
}

int select_hitbox()
{
	int hitbox = 0;

	if (aimbot::hitbox == 0)
		hitbox = Bones::Head;
	else if (aimbot::hitbox == 1)
		hitbox = Bones::Neck;
	else if (aimbot::hitbox == 2)
		hitbox = Bones::Pelvis;
	return hitbox;
}

inline float Rad2Deg(float rad) {
	return rad * 180.0f / 3.1415927f;
}
Vector3 VectorAngles(D3DXVECTOR3 forward)
{
	float	tmp, yaw, pitch;

	if (forward.y == 0 && forward.x == 0)
	{
		yaw = 0;
		if (forward.z > 0)
			pitch = 270;
		else
			pitch = 90;
	}
	else
	{
		yaw = (atan2(forward.y, forward.x) * 180 / M_PI);
		if (yaw < 0)
			yaw += 360;

		tmp = sqrt(forward.x * forward.x + forward.y * forward.y);
		pitch = (atan2(-forward.z, tmp) * 180 / M_PI);
		if (pitch < 0)
			pitch += 360;
	}
	Vector3 angles = Vector3();
	angles.x = pitch;
	angles.y = yaw;
	angles.z = 0;
	return angles;
}
const auto hypotenusa = [](float in)
{
	__m128 reg = _mm_load_ss(&in);
	return _mm_mul_ss(reg, _mm_rsqrt_ss(reg)).m128_f32[0];
};
D3DXVECTOR3 CalcAngle(const D3DXVECTOR3& src, const D3DXVECTOR3& dst)
{
	D3DXVECTOR3 angles;
	D3DXVECTOR3 delta = src - dst;
	float hyp = hypotenusa(delta.x * delta.x + delta.y * delta.y);
	angles.x = -Rad2Deg(asinf(delta.z / hyp));
	angles.y = Rad2Deg(atanf(delta.y / delta.x));
	angles.z = 0.0f;
	return angles;
}
float RankTargetByAngleDistance(D3DXVECTOR3 localplayerpos, Vector3 localplayerangles, float targetdistance, D3DXVECTOR3 targetpos)
{
	D3DXVECTOR3 angleto = CalcAngle(localplayerpos, targetpos);
	float distance_weight = 0.40f;
	float angle_weight = 0.60f;

	return targetdistance * distance_weight + (localplayerangles.Distance(Vector3(angleto.x, angleto.y, angleto.z)) / 100.f) * angle_weight;
}
void SetViewAngles(D3DXVECTOR3 angle)
{
	Driver::write<D3DXVECTOR3>(pid, playercontroller + 0x3F0, angle);
}
void ClampVector(D3DXVECTOR3& Ang) {
	if (Ang.x < -89.f) Ang.x = -89.f;
	else if (Ang.x > 89.f) Ang.x = 89.f;
	while (Ang.y < -180.f) Ang.y += 360.f;
	while (Ang.y > 180.f) Ang.y -= 360.f;
	Ang.z = 0.f;
}
void SmoothAngleSet(D3DXVECTOR3 dest, D3DXVECTOR3 orig)
{
	if (aimbot::aimspeed > 0)
	{
		D3DXVECTOR3 delta;
		delta.x = orig.x - dest.x;
		delta.y = orig.y - dest.y;
		//ClampVector(delta);
		dest.x = orig.x - delta.x / (5.f * aimbot::aimspeed);
		dest.y = orig.y - delta.y / (5.f * aimbot::aimspeed);
	}
	SetViewAngles(dest);
}
void Normalize(D3DXVECTOR3 v)
{
	float size = (float)sqrt(v.x * v.x + v.y * v.y + v.z * v.z);

	if (!size)
	{
		v.x = v.y = v.z = 1;
	}
	else
	{
		v.x /= size;
		v.y /= size;
		v.z /= size;
	}
}

void cache()
{
	while (true)
	{
		vector<TslEntity> tmpList;

		uint64_t uworld = DecryptWorld(BaseAddr);
		//printf("UWorld ->%p\n", uworld);
		uint64_t persislevel = Driver::read<uint64_t>(pid, uworld + 0x38);
		//printf("persislevel ->%p\n", persislevel);
		uint64_t gameinstance = Driver::read<uint64_t>(pid, uworld + 0x190);
		//printf("gameinstance ->%p\n", gameinstance);
		uint64_t localplayer = Driver::read<uint64_t>(pid, gameinstance + 0x40);
		//printf("localplayer ->%p\n", localplayer);
		uint64_t localplayers = Driver::read<uint64_t>(pid, localplayer);
		//printf("localplayers ->%p\n", localplayers);

		playercontroller = Driver::read<uint64_t>(pid, localplayers + 0x38);
		//printf("playercontroller ->%p\n", playercontroller);

		camera_manager = Driver::read<uint64_t>(pid, playercontroller + 0x450);
		//printf("camera_manager ->%p\n", camera_manager);

		pawn = Driver::read<uint64_t>(pid, playercontroller + 0x410);

		uint64_t actor_array = Driver::read<uint64_t>(pid, persislevel + 0xA0);
		int actor_count = Driver::read<int>(pid, persislevel + 0xB8);

		uint64_t local_playerstate = Driver::read<uint64>(pid, pawn + 0x3A0);

		uint64_t localroot_component = Driver::read<uint64_t>(pid, pawn + 0x210);
		localrelative_component = Driver::read<Vector3>(pid, localroot_component + 0x15C);

		uint64_t local_teamcomponent = Driver::read<uint64_t>(pid, local_playerstate + 0x578);
		local_teamindex = Driver::read<int>(pid, local_teamcomponent + 0xF0);

		for (unsigned long i = 0; i < actor_count; ++i)
		{
			uint64_t actor = Driver::read<uint64_t>(pid, actor_array + i * 0x8);

			if (actor == 0x00)
			{
				continue;
			}

			int ID = Driver::read<int>(pid, actor + 0x18);

			TslEntity tslEntity{ };
			tslEntity.pObjPointer = actor;
			tslEntity.ID = ID;


			uint64_t mesh = Driver::read<uint64_t>(pid, actor + 0x3E0);
			uint32_t uniqueid = Driver::read<uint32_t>(pid, actor + 0x38);
			uint64_t damageController = Driver::read<uint64_t>(pid, actor + 0xA70);
			uint64_t playerstate = Driver::read<uint64>(pid, actor + 0x3A0);

			if (uniqueid != 0x11e0101)
				continue;

			if (mesh != 0x00 && damageController != 0x00 && playerstate != 0x00 && uniqueid != 0x00)
			{
				tslEntity.Mesh = mesh;
				tslEntity.DamageController = damageController;
				tslEntity.PlayerState = playerstate;

				tmpList.push_back(tslEntity);
			}
		}

		entityList = tmpList;

		Sleep(100);

	}
}

struct FMinimalViewInfo
{
	Vector3 Location;
	Vector3 Rotation;
	float FOV;
};
struct FCameraCacheEntry
{
	float TimeStamp;
	char UnknownData00[0xC];
	FMinimalViewInfo POV;
};
struct PlayerName
{
	wchar_t PlayerName[64];
};
bool IsVec3Valid(Vector3 vec3)
{
	return !(vec3.x == 0 && vec3.y == 0 && vec3.z == 0);
}

DWORD_PTR pSavedTarget;
bool LastFrameAimKey = false;
DWORD_PTR pTarget;
float Skelcolor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

class ProjectileWeapon {
public:
	virtual float get_projectile_speed() const = 0;
	virtual float get_projectile_gravity() const = 0;
	virtual D3DXVECTOR3 get_projectile_fire_setup(const D3DXVECTOR3& origin, const D3DXVECTOR3& target) const;
};
class WeaponXEntity : public ProjectileWeapon {
public:
	float get_projectile_speed() const;
	float get_projectile_gravity() const;

public:
	int32_t ammo_in_clip;
	int32_t ammo_in_stockpile;
	int32_t lifetime_shots;
	float time_weapon_idle;
	int32_t weap_state;
	bool discarded;
	bool in_reload;
	float cur_zoom_fov;
	float target_zoom_fov;
	float projectile_scale;
	float projectile_speed;
};
class TargetPredictor {
public:
	virtual D3DXVECTOR3 predict_position(float time) const = 0;
};
class LinearPredictor : public TargetPredictor {
public:
	inline LinearPredictor(D3DXVECTOR3 origin, D3DXVECTOR3 velocity) : origin(origin), velocity(velocity) {}
	D3DXVECTOR3 predict_position(float time) const override;

public:
	D3DXVECTOR3 origin;
	D3DXVECTOR3 velocity;
};
struct Solution {
	// Aim the weapon at these angles to hit the target.
	// NOTE! These angles are in radians and not normalized to the game's conventions!
	float pitch, yaw;
	// Projectile travel time.
	float time;
};
bool solve(const D3DXVECTOR3& origin, const ProjectileWeapon& weapon, const TargetPredictor& target, Solution& sol);
D3DXVECTOR3 ProjectileWeapon::get_projectile_fire_setup(const D3DXVECTOR3& origin, const D3DXVECTOR3& target) const {
	return target - origin;
}
D3DXVECTOR3 LinearPredictor::predict_position(float time) const {
	return origin + velocity * time;
}

static bool optimal(float x, float y, float v0, float g, float& pitch) {
	const float root = v0 * v0 * v0 * v0 - g * (g * x * x + 2.0f * y * v0 * v0);
	if (root < 0.0f) {
		return false;
	}
	pitch = atan((v0 * v0 - sqrt(root)) / (g * x));
	return true;
}
static bool solve2d(const D3DXVECTOR3& origin, const ProjectileWeapon& weapon, const D3DXVECTOR3& target, Solution& sol) {
	const auto v = weapon.get_projectile_fire_setup(origin, target);
	const float dx = sqrt(v.x * v.x + v.y * v.y);
	const float dy = v.z;
	const float v0 = weapon.get_projectile_speed();
	const float g = weapon.get_projectile_gravity();
	if (!optimal(dx, dy, v0, g, sol.pitch)) {
		return false;
	}
	sol.time = dx / (cos(sol.pitch) * v0);
	sol.yaw = atan2(v.y, v.x);
	return true;
}
bool solve(const D3DXVECTOR3& origin, const ProjectileWeapon& weapon, const TargetPredictor& target, Solution& sol) {
	static const float MAX_TIME = 1.0f;
	static const float TIME_STEP = 1.0 / 256.0f;
	for (float target_time = 0.0f; target_time <= MAX_TIME; target_time += TIME_STEP) {
		const auto target_pos = target.predict_position(target_time);
		if (!solve2d(origin, weapon, target_pos, sol)) {
			return false;
		}
		if (sol.time < target_time) {
			return true;
		}
	}
	return false;
}
float WeaponXEntity::get_projectile_speed() const {
	return projectile_speed;
}
float WeaponXEntity::get_projectile_gravity() const {
	return /*sv_gravity*/-980.f * projectile_scale;
}

INT64 lastTime = 0;
void usleep(unsigned int usec)
{
	HANDLE timer;
	LARGE_INTEGER ft;

	ft.QuadPart = -(10 * (__int64)usec);

	timer = CreateWaitableTimer(NULL, TRUE, NULL);
	SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
	WaitForSingleObject(timer, INFINITE);
	CloseHandle(timer);
}
int getRandomInt(int min, int max) {
	random_device rd;
	mt19937 mt(rd());
	return mt() % (max - min + 1) + min;
}

bool GetKey(int key)
{
	realkey = key;
	return true;
}
void ChangeKey(void* blank)
{
	keystatus = 1;
	while (true)
	{
		for (int i = 0; i < 0x87; i++)
		{
			if (GetKeyState(i) & 0x8000)
			{
				hotkeys::aimkey = i;
				keystatus = 0;
				return;
			}
		}
	}
}
static const char* keyNames[] =
{
	"",
	"Left Mouse",
	"Right Mouse",
	"Cancel",
	"Middle Mouse",
	"Mouse 5",
	"Mouse 4",
	"",
	"Backspace",
	"Tab",
	"",
	"",
	"Clear",
	"Enter",
	"",
	"",
	"Shift",
	"Control",
	"Alt",
	"Pause",
	"Caps",
	"",
	"",
	"",
	"",
	"",
	"",
	"Escape",
	"",
	"",
	"",
	"",
	"Space",
	"Page Up",
	"Page Down",
	"End",
	"Home",
	"Left",
	"Up",
	"Right",
	"Down",
	"",
	"",
	"",
	"Print",
	"Insert",
	"Delete",
	"",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"",
	"",
	"",
	"",
	"",
	"Numpad 0",
	"Numpad 1",
	"Numpad 2",
	"Numpad 3",
	"Numpad 4",
	"Numpad 5",
	"Numpad 6",
	"Numpad 7",
	"Numpad 8",
	"Numpad 9",
	"Multiply",
	"Add",
	"",
	"Subtract",
	"Decimal",
	"Divide",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
};
static bool Items_ArrayGetter(void* data, int idx, const char** out_text)
{
	const char* const* items = (const char* const*)data;
	if (out_text)
		*out_text = items[idx];
	return true;
}
void HotkeyButton(int aimkey, void* changekey, int status)
{
	const char* preview_value = NULL;
	if (aimkey >= 0 && aimkey < IM_ARRAYSIZE(keyNames))
		Items_ArrayGetter(keyNames, aimkey, &preview_value);

	std::string aimkeys;
	if (preview_value == NULL)
		aimkeys = "Select Key";
	else
		aimkeys = preview_value;

	if (status == 1)
	{
		aimkeys = "Press key to bind";
	}
	if (ImGui::Button(aimkeys.c_str(), ImVec2(125, 20)))
	{
		if (status == 0)
		{
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)changekey, nullptr, 0, nullptr);
		}
	}
}

Gender GetCharacterGender(uint64_t Mesh)
{
	uint64_t BoneArray = Driver::read<uint64_t>(pid, Mesh + 0x518);
	uint32_t BoneArrayCount = Driver::read<uint32_t>(pid, Mesh + 0x518 + 0x8);

	if (!BoneArray)
	{
		BoneArray = Driver::read<uint64_t>(pid, Mesh + 0x528);
		BoneArrayCount = Driver::read<uint32_t>(pid, Mesh + 0x528 + 0x8);
	}

	return BoneArrayCount == 99 ? Gender::Female : Gender::Male;
}

inline float NormalizeAngle(float flAng)
{
	if (flAng != flAng)
		flAng = 0.f;
	return fmod(flAng + 180.f, 360.f) - 180.f;
}
inline void ClampViewAngles(D3DXVECTOR3& vecAng)
{
	vecAng.x = NormalizeAngle(vecAng.x);
	vecAng.y = NormalizeAngle(vecAng.y);
	vecAng.z = 0.0f;

	if (vecAng.x < -89.0)
	{
		vecAng.x = -89.0;
	}
	else if (vecAng.x > 89.0)
	{
		vecAng.x = 89.0;
	}
}
D3DXVECTOR3 SmoothAim(D3DXVECTOR3 Camera_rotation, D3DXVECTOR3 Target, float SmoothFactor)
{
	D3DXVECTOR3 diff = Target - Camera_rotation;
	ClampViewAngles(diff);
	return Camera_rotation + diff / SmoothFactor;
}
inline void RCS(D3DXVECTOR3 Target, D3DXVECTOR3 Camera_rotation, float SmoothFactor) {
	// Camera 2 Control space
	D3DXVECTOR3 ConvertRotation = Camera_rotation;

	ClampViewAngles(ConvertRotation);
	// Calculate recoil/aimpunch
	auto ControlRotation = Driver::read<D3DXVECTOR3>(pid, playercontroller + 0x3F0);
	D3DXVECTOR3 DeltaRotation = ConvertRotation - ControlRotation;
	ClampViewAngles(DeltaRotation);

	// Remove aimpunch from CameraRotation
	ConvertRotation = Target - (DeltaRotation * SmoothFactor);
	ClampViewAngles(ConvertRotation);

	//Smooth the whole thing
	D3DXVECTOR3 Smoothed = SmoothAim(Camera_rotation, ConvertRotation, SmoothFactor);
	Smoothed -= (DeltaRotation / SmoothFactor);
	//printf("RCS: %f, %f\n", Smoothed.x, Smoothed.y);
	Driver::write(pid, playercontroller + 0x3F0, Smoothed);
}

void PredictAim(DWORD_PTR entity, float dist, D3DXVECTOR3 CameraLPos, D3DXVECTOR3 CameraLROT) {

	if (!aimbot::aimbot)
		return;

	float x = CameraLROT.x;
	float y = CameraLROT.y;

	uint64_t currentactormesh = Driver::read<uint64_t>(pid, entity + 0x3E0);

	uint64_t CurrentActorRootComponent = Driver::read<uint64_t>(pid, entity + 0x210);


	Vector3 E_Velocity = Vector3{ 0.f, 0.f, 0.f };

	Vector3 RootPos = GetBoneWithRotation(currentactormesh, select_hitbox());

	WeaponXEntity wxe;
	wxe.projectile_scale = 0.1;
	wxe.projectile_speed = 36000;
	const auto weapon = wxe;
	LinearPredictor predictor{ D3DXVECTOR3(RootPos.x, RootPos.y, RootPos.z - 5), D3DXVECTOR3(E_Velocity.x, E_Velocity.y, E_Velocity.z) };
	Solution sol;
	D3DXVECTOR3 rotpos;

	if (solve(D3DXVECTOR3(CameraLPos.x, CameraLPos.y, CameraLPos.z), weapon, predictor, sol)) {
		auto rootPos = D3DXVECTOR3{ Rad2Deg(sol.pitch),Rad2Deg(sol.yaw), 0.0f };

		if (aimbot::smooth) {

			D3DXVECTOR3 delta;
			delta = CameraLROT - rootPos;
			ClampVector(delta);
			rotpos = CameraLROT - delta;

		}

		RCS(rotpos, CameraLROT, aimbot::aimspeed);

		SetViewAngles(D3DXVECTOR3(rotpos.x, rotpos.y, 0));
	}
}
void ESP() {

	RGBA ESPColor = { colors::espcol[0] * 255, colors::espcol[1] * 255, colors::espcol[2] * 255, 255 };

	FCameraCacheEntry camera_cache = Driver::read<FCameraCacheEntry>(pid, camera_manager + 0x11E8);

	Vector3 Location = Driver::read<Vector3>(pid, camera_manager + 0x11B0);
	Vector3 Rotation = Driver::read<Vector3>(pid, camera_manager + 0x11BC);
	float FOV = Driver::read<float>(pid, camera_manager + 0x11C8);

	D3DXVECTOR3 LocationX = Driver::read<D3DXVECTOR3>(pid, camera_manager + 0x11B0);
	D3DXVECTOR3 RotationX = Driver::read<D3DXVECTOR3>(pid, camera_manager + 0x11BC);

	uint64_t closeentity = NULL;
	int iLockIndex = -1;
	float closestDistance = FLT_MAX;

	int revise = 0;
	float distance;

	auto entityListCopy = entityList;

	if (entityListCopy.empty())
	{
		return;
	}

	for (unsigned long i = 0; i < entityListCopy.size(); ++i)
	{
		TslEntity entity = entityListCopy[i];

		float PlayerHealth = Driver::read<float>(pid, entity.DamageController + 0x188);

		int dormant = Driver::read<int>(pid, entity.pObjPointer + 0xD8);
		Vector3 cache_life = Driver::read<Vector3>(pid, entity.DamageController + 0x188);

		if (entity.pObjPointer == pawn)
		{
			continue;
		}

		if (PlayerHealth > 100.0f || PlayerHealth <= 0.01f)
		{
			continue;
		}

		uint64_t playername = Driver::read<uint64_t>(pid, entity.PlayerState + 0x3A8);

		wchar_t* pchName = Driver::read<PlayerName>(pid, playername).PlayerName;

		auto ws = std::wstring(pchName);

		auto pNamePlate = std::string(ws.begin(), ws.end());

		uint64_t rootcomponent = Driver::read<uint64_t>(pid, entity.pObjPointer + 0x210);
		Vector3 relativelocation = Driver::read<Vector3>(pid, rootcomponent + 0x15C);
		Vector3 X = WorldToScreen(relativelocation, Location, Rotation, FOV);

		uint64_t mesh = entity.Mesh;

		distance = localrelative_component.Distance(relativelocation) / 100.f;

		uint64_t teamcomponent = Driver::read<uint64_t>(pid, entity.PlayerState + 0x578);

		if (teamcomponent == 0x00)
		{
			continue;
		}

		int teamindex = Driver::read<int>(pid, teamcomponent + 0xF0);

		uint32_t uniqueid = Driver::read<uint32_t>(pid, entity.pObjPointer + 0x38);

		if (uniqueid != 0x11e0101)
			continue;

		if (mesh != 0x00 && uniqueid != 0x00)
		{
			if (local_teamindex != teamindex && dormant == 0)
			{
				Vector3 vHeadBone = GetBoneWithRotation(mesh, 12);
				Vector3 vRootBone = GetBoneWithRotation(mesh, 0);

				Vector3 vHeadBoneOut = WorldToScreen(Vector3(vHeadBone.x, vHeadBone.y, vHeadBone.z), Location, Rotation, FOV);
				Vector3 vRootBoneOut = WorldToScreen(vRootBone, Location, Rotation, FOV);

				float BoxHeight = abs(vHeadBoneOut.y - vRootBoneOut.y);
				float BoxWidth = BoxHeight / 1.8f;

				if (visuals::name)
				{
					char name[64];
					sprintf_s(name, "%2.fm", distance);
					DrawString(menu::font_size, vHeadBoneOut.x, vHeadBoneOut.y - 5, &Col.white_, true, true, name);
				}

				if (visuals::health)
				{
					if (visuals::healthMode == 0 || visuals::healthMode == 3)
						DrawLeftProgressBar(X.x, vHeadBoneOut.y, BoxWidth, BoxHeight, 1, PlayerHealth);
					if (visuals::healthMode == 1 || visuals::healthMode == 4)
						DrawRightProgressBar(X.x, vHeadBoneOut.y, BoxWidth, BoxHeight, 1, PlayerHealth);
					if (visuals::healthMode == 2 || visuals::healthMode == 3 || visuals::healthMode == 4)
					{
						char vida[100];
						sprintf(vida, "H:%2.f", PlayerHealth);
						DrawString(12, X.x, X.y + 5, &Col.white, true, true, vida);
					}
				}

				if (visuals::box)
				{
					if (visuals::boxMode == 0 || visuals::boxMode == 1)
					{
						DrawNormalBox(vRootBoneOut.x - BoxWidth / 2 + 1, vHeadBoneOut.y, BoxWidth, BoxHeight, 1, &Col.black);
						DrawNormalBox(vRootBoneOut.x - BoxWidth / 2 - 1, vHeadBoneOut.y, BoxWidth, BoxHeight, 1, &Col.black);
						DrawNormalBox(vRootBoneOut.x - BoxWidth / 2, vHeadBoneOut.y + 1, BoxWidth, BoxHeight, 1, &Col.black);
						DrawNormalBox(vRootBoneOut.x - BoxWidth / 2, vHeadBoneOut.y - 1, BoxWidth, BoxHeight, 1, &Col.black);
						DrawNormalBox(vRootBoneOut.x - (BoxWidth / 2), vHeadBoneOut.y, BoxWidth, BoxHeight, 1, &ESPColor);
					}
					if (visuals::boxMode == 2)
					{
						DrawCornerBox(vRootBoneOut.x - BoxWidth / 2 + 1, vHeadBoneOut.y, BoxWidth, BoxHeight, 1, &Col.black);
						DrawCornerBox(vRootBoneOut.x - BoxWidth / 2 - 1, vHeadBoneOut.y, BoxWidth, BoxHeight, 1, &Col.black);
						DrawCornerBox(vRootBoneOut.x - BoxWidth / 2, vHeadBoneOut.y + 1, BoxWidth, BoxHeight, 1, &Col.black);
						DrawCornerBox(vRootBoneOut.x - BoxWidth / 2, vHeadBoneOut.y - 1, BoxWidth, BoxHeight, 1, &Col.black);
						DrawCornerBox(vRootBoneOut.x - (BoxWidth / 2), vHeadBoneOut.y, BoxWidth, BoxHeight, 1, &ESPColor);
					}
				}

				if (visuals::boxMode == 1 || visuals::boxMode == 3) menu::box_thick = 1,
					DrawFilledRect(vRootBoneOut.x - (BoxWidth / 2), vHeadBoneOut.y, BoxWidth, BoxHeight, &Col.filled);

				if (visuals::skel) {

					auto EnemyGender = GetCharacterGender(mesh);

					if (EnemyGender == Gender::Female)
					{
						Vector3 vHeadBone = GetBoneWithRotation(mesh, 8);
						Vector3 vHeadBoneOut = WorldToScreen(vHeadBone, Location, Rotation, FOV);

						Vector3 vHip = GetBoneWithRotation(mesh, 3);
						Vector3 vHipOut = WorldToScreen(vHip, Location, Rotation, FOV);

						Vector3 vNeck = GetBoneWithRotation(mesh, 7);
						Vector3 vNeckOut = WorldToScreen(vNeck, Location, Rotation, FOV);

						Vector3 vUpperArmLeft = GetBoneWithRotation(mesh, 21);
						Vector3 vUpperArmLeftOut = WorldToScreen(vUpperArmLeft, Location, Rotation, FOV);

						Vector3 vUpperArmRight = GetBoneWithRotation(mesh, 47);
						Vector3 vUpperArmRightOut = WorldToScreen(vUpperArmRight, Location, Rotation, FOV);

						Vector3 vLeftHand = GetBoneWithRotation(mesh, 22);
						Vector3 vLeftHandOut = WorldToScreen(vLeftHand, Location, Rotation, FOV);

						Vector3 vRightHand = GetBoneWithRotation(mesh, 48);
						Vector3 vRightHandOut = WorldToScreen(vRightHand, Location, Rotation, FOV);

						Vector3 vLeftHand1 = GetBoneWithRotation(mesh, 23);
						Vector3 vLeftHandOut1 = WorldToScreen(vLeftHand1, Location, Rotation, FOV);

						Vector3 vRightHand1 = GetBoneWithRotation(mesh, 49);
						Vector3 vRightHandOut1 = WorldToScreen(vRightHand1, Location, Rotation, FOV);

						Vector3 vRightThigh = GetBoneWithRotation(mesh, 82);
						Vector3 vRightThighOut = WorldToScreen(vRightThigh, Location, Rotation, FOV);

						Vector3 vLeftThigh = GetBoneWithRotation(mesh, 75);
						Vector3 vLeftThighOut = WorldToScreen(vLeftThigh, Location, Rotation, FOV);

						Vector3 vRightCalf = GetBoneWithRotation(mesh, 83);
						Vector3 vRightCalfOut = WorldToScreen(vRightCalf, Location, Rotation, FOV);

						Vector3 vLeftCalf = GetBoneWithRotation(mesh, 76);
						Vector3 vLeftCalfOut = WorldToScreen(vLeftCalf, Location, Rotation, FOV);

						Vector3 vLeftFoot = GetBoneWithRotation(mesh, 78);
						Vector3 vLeftFootOut = WorldToScreen(vLeftFoot, Location, Rotation, FOV);

						Vector3 vRightFoot = GetBoneWithRotation(mesh, 85);
						Vector3 vRightFootOut = WorldToScreen(vRightFoot, Location, Rotation, FOV);

						Vector3 Pelvis = GetBoneWithRotation(mesh, 3);
						Vector3 PelvisOut = WorldToScreen(Pelvis, Location, Rotation, FOV);

						DrawLine(vNeckOut.x, vNeckOut.y, vHeadBoneOut.x, vHeadBoneOut.y, &Col.white, menu::box_thick);
						DrawLine(PelvisOut.x, PelvisOut.y, vNeckOut.x, vNeckOut.y, &Col.white, menu::box_thick);

						DrawLine(vUpperArmLeftOut.x, vUpperArmLeftOut.y, vNeckOut.x, vNeckOut.y, &Col.white, menu::box_thick);
						DrawLine(vUpperArmRightOut.x, vUpperArmRightOut.y, vNeckOut.x, vNeckOut.y, &Col.white, menu::box_thick);

						DrawLine(vLeftHandOut.x, vLeftHandOut.y, vUpperArmLeftOut.x, vUpperArmLeftOut.y, &Col.white, menu::box_thick);
						DrawLine(vRightHandOut.x, vRightHandOut.y, vUpperArmRightOut.x, vUpperArmRightOut.y, &Col.white, menu::box_thick);

						DrawLine(vLeftHandOut.x, vLeftHandOut.y, vLeftHandOut1.x, vLeftHandOut1.y, &Col.white, menu::box_thick);
						DrawLine(vRightHandOut.x, vRightHandOut.y, vRightHandOut1.x, vRightHandOut1.y, &Col.white, menu::box_thick);

						DrawLine(vLeftThighOut.x, vLeftThighOut.y, vHipOut.x, vHipOut.y, &Col.white, menu::box_thick);
						DrawLine(vRightThighOut.x, vRightThighOut.y, vHipOut.x, vHipOut.y, &Col.white, menu::box_thick);

						DrawLine(vLeftCalfOut.x, vLeftCalfOut.y, vLeftThighOut.x, vLeftThighOut.y, &Col.white, menu::box_thick);
						DrawLine(vRightCalfOut.x, vRightCalfOut.y, vRightThighOut.x, vRightThighOut.y, &Col.white, menu::box_thick);
					}
					else
					{
						Vector3 vHeadBone = GetBoneWithRotation(mesh, 8);
						Vector3 vHeadBoneOut = WorldToScreen(vHeadBone, Location, Rotation, FOV);

						Vector3 vHip = GetBoneWithRotation(mesh, 3);
						Vector3 vHipOut = WorldToScreen(vHip, Location, Rotation, FOV);

						Vector3 vNeck = GetBoneWithRotation(mesh, 7);
						Vector3 vNeckOut = WorldToScreen(vNeck, Location, Rotation, FOV);

						Vector3 vUpperArmLeft = GetBoneWithRotation(mesh, 21);
						Vector3 vUpperArmLeftOut = WorldToScreen(vUpperArmLeft, Location, Rotation, FOV);

						Vector3 vUpperArmRight = GetBoneWithRotation(mesh, 47);
						Vector3 vUpperArmRightOut = WorldToScreen(vUpperArmRight, Location, Rotation, FOV);

						Vector3 vLeftHand = GetBoneWithRotation(mesh, 22);
						Vector3 vLeftHandOut = WorldToScreen(vLeftHand, Location, Rotation, FOV);

						Vector3 vRightHand = GetBoneWithRotation(mesh, 48);
						Vector3 vRightHandOut = WorldToScreen(vRightHand, Location, Rotation, FOV);

						Vector3 vLeftHand1 = GetBoneWithRotation(mesh, 23);
						Vector3 vLeftHandOut1 = WorldToScreen(vLeftHand1, Location, Rotation, FOV);

						Vector3 vRightHand1 = GetBoneWithRotation(mesh, 49);
						Vector3 vRightHandOut1 = WorldToScreen(vRightHand1, Location, Rotation, FOV);

						Vector3 vRightThigh = GetBoneWithRotation(mesh, 82);
						Vector3 vRightThighOut = WorldToScreen(vRightThigh, Location, Rotation, FOV);

						Vector3 vLeftThigh = GetBoneWithRotation(mesh, 75);
						Vector3 vLeftThighOut = WorldToScreen(vLeftThigh, Location, Rotation, FOV);

						Vector3 vRightCalf = GetBoneWithRotation(mesh, 83);
						Vector3 vRightCalfOut = WorldToScreen(vRightCalf, Location, Rotation, FOV);

						Vector3 vLeftCalf = GetBoneWithRotation(mesh, 76);
						Vector3 vLeftCalfOut = WorldToScreen(vLeftCalf, Location, Rotation, FOV);

						Vector3 vLeftFoot = GetBoneWithRotation(mesh, 78);
						Vector3 vLeftFootOut = WorldToScreen(vLeftFoot, Location, Rotation, FOV);

						Vector3 vRightFoot = GetBoneWithRotation(mesh, 85);
						Vector3 vRightFootOut = WorldToScreen(vRightFoot, Location, Rotation, FOV);

						Vector3 Pelvis = GetBoneWithRotation(mesh, 3);
						Vector3 PelvisOut = WorldToScreen(Pelvis, Location, Rotation, FOV);

						DrawLine(vNeckOut.x, vNeckOut.y, vHeadBoneOut.x, vHeadBoneOut.y, &Col.white, menu::box_thick);
						DrawLine(PelvisOut.x, PelvisOut.y, vNeckOut.x, vNeckOut.y, &Col.white, menu::box_thick);

						DrawLine(vUpperArmLeftOut.x, vUpperArmLeftOut.y, vNeckOut.x, vNeckOut.y, &Col.white, menu::box_thick);
						DrawLine(vUpperArmRightOut.x, vUpperArmRightOut.y, vNeckOut.x, vNeckOut.y, &Col.white, menu::box_thick);

						DrawLine(vLeftHandOut.x, vLeftHandOut.y, vUpperArmLeftOut.x, vUpperArmLeftOut.y, &Col.white, menu::box_thick);
						DrawLine(vRightHandOut.x, vRightHandOut.y, vUpperArmRightOut.x, vUpperArmRightOut.y, &Col.white, menu::box_thick);

						DrawLine(vLeftHandOut.x, vLeftHandOut.y, vLeftHandOut1.x, vLeftHandOut1.y, &Col.white, menu::box_thick);
						DrawLine(vRightHandOut.x, vRightHandOut.y, vRightHandOut1.x, vRightHandOut1.y, &Col.white, menu::box_thick);

						DrawLine(vLeftThighOut.x, vLeftThighOut.y, vHipOut.x, vHipOut.y, &Col.white, menu::box_thick);
						DrawLine(vRightThighOut.x, vRightThighOut.y, vHipOut.x, vHipOut.y, &Col.white, menu::box_thick);

						DrawLine(vLeftCalfOut.x, vLeftCalfOut.y, vLeftThighOut.x, vLeftThighOut.y, &Col.white, menu::box_thick);
						DrawLine(vRightCalfOut.x, vRightCalfOut.y, vRightThighOut.x, vRightThighOut.y, &Col.white, menu::box_thick);

						DrawLine(vLeftFootOut.x, vLeftFootOut.y, vLeftCalfOut.x, vLeftCalfOut.y, &Col.white, menu::box_thick);
						DrawLine(vRightFootOut.x, vRightFootOut.y, vRightCalfOut.x, vRightCalfOut.y, &Col.white, menu::box_thick);
					}

				}

				if (visuals::lines)
				{
					if (visuals::lineMode == 0)
						DrawLine(ScreenCenterX, 0, vHeadBoneOut.x, vHeadBoneOut.y, &ESPColor, 1);
					if (visuals::lineMode == 1)
						DrawLine(ScreenCenterX, ScreenCenterY, vHeadBoneOut.x, vHeadBoneOut.y, &ESPColor, 1);
					if (visuals::lineMode == 2)
						DrawLine(ScreenCenterX, ScreenCenterY + ScreenCenterY + 80, vHeadBoneOut.x, vHeadBoneOut.y, &ESPColor, 1);
				}

				if (visuals::show_head)
					DrawCircle(vHeadBoneOut.x, vHeadBoneOut.y, BoxHeight / 25, &Col.white, 30);

				auto dx = vHeadBoneOut.x - (Width / 2);
				auto dy = vHeadBoneOut.y - (Height / 2);
				auto dist = sqrtf(dx * dx + dy * dy);

				if (dist < aimbot::aimfov * 8 && distance < closestDistance) {

					closestDistance = distance;
					closeentity = entity.pObjPointer;
					iLockIndex = i;
				}
			}
		}
	}

	if (iLockIndex < 1)
		return;

	if (closeentity) {

		if (aimbot::aimbot && GetAsyncKeyState(hotkeys::aimkey) < 0) {

			PredictAim(closeentity, closestDistance, LocationX, RotationX);
		}
	}
}

HRESULT DirectXInit(HWND hWnd)
{
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_Object)))
		exit(3);

	ZeroMemory(&p_Params, sizeof(p_Params));
	p_Params.Windowed = TRUE;
	p_Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	p_Params.hDeviceWindow = hWnd;
	p_Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	p_Params.BackBufferFormat = D3DFMT_A8R8G8B8;
	p_Params.BackBufferWidth = Width;
	p_Params.BackBufferHeight = Height;
	p_Params.EnableAutoDepthStencil = TRUE;
	p_Params.AutoDepthStencilFormat = D3DFMT_D16;
	p_Params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(p_Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &p_Params, 0, &p_Device)))
	{
		p_Object->Release();
		exit(4);
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImFontConfig font_config;
	font_config.OversampleH = 1;
	font_config.OversampleV = 1;
	font_config.PixelSnapH = true;

	static const ImWchar ranges[] =
	{
		0x0020, 0x00FF,
		0x0400, 0x044F,
		0,
	};

	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 13.f);

	SkeetFont = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(icon_compressed_data, icon_compressed_size, 15.f, &font_config, ranges);

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(p_Device);

	ImGui::StyleColorsClassic();
	ImGuiStyle* style = &ImGui::GetStyle();
	style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 0.0f;
	style->FramePadding = ImVec2(2, 2);
	style->FrameRounding = 0.0f;
	style->ItemSpacing = ImVec2(8, 8);// ImVec2(12, 8);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 0.0f;
	style->GrabMinSize = 10.0f;
	style->GrabRounding = 0.0f;
	style->TabRounding = 0.f;
	style->ChildRounding = 0.f;

	style->Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
	style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_Border] = ImColor(40, 40, 40, 255);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.95f, 0.67f, 0.31f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.95f, 0.67f, 0.31f, 1.00f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImColor(245, 160, 15, 255);
	style->Colors[ImGuiCol_SliderGrab] = ImColor(40, 40, 40, 255);
	style->Colors[ImGuiCol_SliderGrabActive] = ImColor(60, 60, 60, 255);
	style->Colors[ImGuiCol_Button] = ImColor(40, 40, 40, 255);
	style->Colors[ImGuiCol_ButtonHovered] = ImColor(50, 50, 50, 255);
	style->Colors[ImGuiCol_ButtonActive] = ImColor(50, 50, 50, 255);
	style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);

	style->WindowTitleAlign.x = 0.50f;

	p_Object->Release();
	return S_OK;
}
void SetupWindow()
{
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)SetWindowToTarget, 0, 0, 0);

	char randomtitle[] = "RADEON";

	WNDCLASSEXA wcex = {
		sizeof(WNDCLASSEXA),
		0,
		DefWindowProcA,
		0,
		0,
		nullptr,
		LoadIcon(nullptr, IDI_APPLICATION),
		LoadCursor(nullptr, IDC_ARROW),
		nullptr,
		nullptr,
		randomtitle,
		LoadIcon(nullptr, IDI_APPLICATION)
	};

	RECT Rect;
	GetWindowRect(GetDesktopWindow(), &Rect);

	RegisterClassExA(&wcex);

	MyWnd = CreateWindowExA(NULL, randomtitle, randomtitle, WS_POPUP, Rect.left, Rect.top, Rect.right, Rect.bottom, NULL, NULL, wcex.hInstance, NULL);
	SetWindowLong(MyWnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
	SetLayeredWindowAttributes(MyWnd, RGB(0, 0, 0), 255, LWA_ALPHA);

	MARGINS margin = { -1 };
	DwmExtendFrameIntoClientArea(MyWnd, &margin);

	ShowWindow(MyWnd, SW_SHOW);
	UpdateWindow(MyWnd);
}
float colorNameBot[3] = { 1.00f, 1.00f, 1.00f };
void background()
{
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Once);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.09f, 0.09f, 0.09f, 0.40f / 1.f * 2.f));
	static const auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove;
	ImGui::Begin("##background", nullptr, flags);
	ImGui::End();
	ImGui::PopStyleColor();
}
bool draw_abigsquare()
{
	static int MenuTab = 0;
	static int VisualTab = 0;
	float
		TextSpaceLine = 90.f,
		SpaceLineOne = 120.f,
		SpaceLineTwo = 280.f,
		SpaceLineThr = 420.f;
	ImGuiStyle* style = &ImGui::GetStyle();

	static auto flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar;
	ImGui::Begin("AvalancheSoftware.net - Valorant External", NULL, flags);
	{
		ImGui::SetWindowSize(ImVec2(450, 215), ImGuiCond_Once);
		ImGui::SetNextWindowBgAlpha(0.0f);
		{
			style->ItemSpacing = ImVec2(1, 4);
			ImGui::PushFont(SkeetFont);
			{
				ImGui::BeginChild("##Features", ImVec2(115.f, 170.f), true);
				{
					ImGui::SetCursorPos(ImVec2(15, 39));
					if (ImGui::Button(("Aimbot"), ImVec2(85, 20)))
					{
						MenuTab = 0;
					}
					ImGui::SetCursorPos(ImVec2(15, 63));
					if (ImGui::Button(("Visuals"), ImVec2(85, 20)))
					{
						MenuTab = 1;
					}
					ImGui::SetCursorPos(ImVec2(15, 87));
					if (ImGui::Button(("Misc"), ImVec2(85, 20)))
					{
						MenuTab = 2;
					}
					//ImGui::SetCursorPos(ImVec2(15, 111));
					//if (ImGui::Button(("Build"), ImVec2(85, 20)))
					//{
					//	MenuTab = 3;
					//}
				}
				ImGui::EndChild();

			} ImGui::PopFont();


			if (MenuTab == 0)
			{
				ImGui::SetCursorPos(ImVec2(139, 32));
				ImGui::Checkbox("Aimbot", &aimbot::aimbot);
				ImGui::SetCursorPos(ImVec2(139, 55));
				ImGui::PushItemWidth(200.f);
				ImGui::Combo("##AimMode", &aimbot::aimmode, Aimmode, IM_ARRAYSIZE(Aimmode));
				ImGui::SetCursorPos(ImVec2(139, 76));
				ImGui::PushItemWidth(200.f);
				ImGui::Combo("##BONES", &aimbot::hitbox, Hitbox, IM_ARRAYSIZE(Hitbox));
				ImGui::SetCursorPos(ImVec2(139, 99));
				ImGui::Checkbox("Smooth", &aimbot::smooth);
				ImGui::SetCursorPos(ImVec2(139, 122));
				ImGui::PushItemWidth(200.f);
				ImGui::SliderFloat("", &aimbot::aimspeed, 1.f, 20.f, "%.1f");
				ImGui::SetCursorPos(ImVec2(139, 150));
				ImGui::Checkbox("Show FOV", &aimbot::fovcircle);
				ImGui::SetCursorPos(ImVec2(139, 174));
				ImGui::PushItemWidth(200.f);
				ImGui::SliderFloat("##fovkrai", &aimbot::aimfov, 1.f, 360.0f, "%.1f");
				ImGui::SetCursorPos(ImVec2(139, 188));
			}
			else if (MenuTab == 1)
			{
				if (VisualTab == 0)
				{
					ImGui::SetCursorPos(ImVec2(139, 32));
					ImGui::Checkbox("ESP", &visuals::box);
					ImGui::SetCursorPos(ImVec2(139, 55));
					ImGui::PushItemWidth(200.f);
					ImGui::Combo("##BOXSTYLES", &visuals::boxMode, boxStyle, IM_ARRAYSIZE(boxStyle));
					ImGui::SetCursorPos(ImVec2(139, 76));
					ImGui::Checkbox("Show Skeleton", &visuals::skel);
					ImGui::SetCursorPos(ImVec2(139, 99));
					ImGui::Checkbox("Show Distance", &visuals::name);
					ImGui::SetCursorPos(ImVec2(139, 122));
					ImGui::PushItemWidth(200.f);
					ImGui::SliderInt("", &visuals::MaxDistance, 0.f, 800.f);
					ImGui::SetCursorPos(ImVec2(139, 150));
					ImGui::Checkbox("Show Health", &visuals::health);
					ImGui::SetCursorPos(ImVec2(139, 174));
					ImGui::PushItemWidth(200.f);
					ImGui::Combo("##HEALTHSTYLE", &visuals::healthMode, healthMode, IM_ARRAYSIZE(healthMode));
				}
			}
			if (MenuTab == 2)
			{

				ImGui::SetCursorPos(ImVec2(139, 32));
				ImGui::Text("Aimbot Hotkey:");
				ImGui::SetCursorPos(ImVec2(139, 55));
				HotkeyButton(hotkeys::aimkey, ChangeKey, keystatus);

				float color_red = 1.;
				float color_green = 0;
				float color_blue = 0;
				float color_random = 0.0;
				float color_speed = -10.0;
			}
		}
		ImGui::End();
	}
	return true;
}

void shortcurts()
{
	if (GetAsyncKeyState(VK_INSERT))
	{
		if (menu_key == false)
		{
			menu_key = true;
		}
		else if (menu_key == true)
		{
			menu_key = false;
		}
		Sleep(200);
	}
}
void render() {

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	shortcurts();

	if (menu_key)
	{
		background();
		draw_abigsquare();
		ImGui::GetIO().MouseDrawCursor = 1;
	}
	else {
		ImGui::GetIO().MouseDrawCursor = 0;
	}

	if (aimbot::fovcircle)
	{
		float radiusx = aimbot::aimfov * (ScreenCenterX / 100);
		float radiusy = aimbot::aimfov * (ScreenCenterY / 100);

		float calcradius = (radiusx + radiusy) / 2;
		ImGui::GetOverlayDrawList()->AddCircle(ImVec2(ScreenCenterX, ScreenCenterY), calcradius, 0xFFFFFFFF, 25);
	}

	ESP();

	ImGui::EndFrame();
	p_Device->SetRenderState(D3DRS_ZENABLE, false);
	p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	p_Device->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
	p_Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	if (p_Device->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		p_Device->EndScene();
	}
	HRESULT result = p_Device->Present(NULL, NULL, NULL, NULL);

	if (result == D3DERR_DEVICELOST && p_Device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
		p_Device->Reset(&p_Params);
		ImGui_ImplDX9_CreateDeviceObjects();
	}
}

WPARAM MainLoop()
{
	static RECT old_rc;
	ZeroMemory(&Message, sizeof(MSG));

	while (Message.message != WM_QUIT)
	{
		if (PeekMessage(&Message, MyWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		HWND hwnd_active = GetForegroundWindow();
		if (GetAsyncKeyState(0x23) & 1)
			exit(8);

		if (hwnd_active == GameWnd) {
			HWND hwndtest = GetWindow(hwnd_active, GW_HWNDPREV);
			SetWindowPos(MyWnd, hwndtest, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}
		RECT rc;
		POINT xy;

		ZeroMemory(&rc, sizeof(RECT));
		ZeroMemory(&xy, sizeof(POINT));
		GetClientRect(GameWnd, &rc);
		ClientToScreen(GameWnd, &xy);
		rc.left = xy.x;
		rc.top = xy.y;

		ImGuiIO& io = ImGui::GetIO();
		io.ImeWindowHandle = GameWnd;
		io.DeltaTime = 1.0f / 60.0f;

		POINT p;
		GetCursorPos(&p);
		io.MousePos.x = p.x - xy.x;
		io.MousePos.y = p.y - xy.y;

		if (GetAsyncKeyState(VK_LBUTTON)) {
			io.MouseDown[0] = true;
			io.MouseClicked[0] = true;
			io.MouseClickedPos[0].x = io.MousePos.x;
			io.MouseClickedPos[0].x = io.MousePos.y;
		}
		else
			io.MouseDown[0] = false;

		if (rc.left != old_rc.left || rc.right != old_rc.right || rc.top != old_rc.top || rc.bottom != old_rc.bottom)
		{

			old_rc = rc;

			Width = rc.right;
			Height = rc.bottom;

			p_Params.BackBufferWidth = Width;
			p_Params.BackBufferHeight = Height;
			SetWindowPos(MyWnd, (HWND)0, xy.x, xy.y, Width, Height, SWP_NOREDRAW);
			p_Device->Reset(&p_Params);
		}
		render();

	}
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CleanuoD3D();
	DestroyWindow(MyWnd);

	return Message.wParam;
}
LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, Message, wParam, lParam))
		return true;

	switch (Message)
	{
	case WM_DESTROY:
		CleanuoD3D();
		PostQuitMessage(0);
		exit(4);
		break;
	case WM_SIZE:
		if (p_Device != NULL && wParam != SIZE_MINIMIZED)
		{
			ImGui_ImplDX9_InvalidateDeviceObjects();
			p_Params.BackBufferWidth = LOWORD(lParam);
			p_Params.BackBufferHeight = HIWORD(lParam);
			HRESULT hr = p_Device->Reset(&p_Params);
			if (hr == D3DERR_INVALIDCALL)
				IM_ASSERT(0);
			ImGui_ImplDX9_CreateDeviceObjects();
		}
		break;
	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
		break;
	}
	return 0;
}
void CleanuoD3D()
{
	if (p_Device != NULL)
	{
		p_Device->EndScene();
		p_Device->Release();
	}
	if (p_Object != NULL)
	{
		p_Object->Release();
	}
}
int isTopwin()
{
	HWND hWnd = GetForegroundWindow();
	if (hWnd == GameWnd)
		return TopWindowGame;
	if (hWnd == MyWnd)
		return TopWindowMvoe;

	return 0;
}
void SetWindowToTarget()
{
	while (true)
	{
		GameWnd = FindWindowW(TEXT("UnrealWindow"), NULL);
		if (GameWnd)
		{
			ZeroMemory(&GameRect, sizeof(GameRect));
			GetWindowRect(GameWnd, &GameRect);
			Width = GameRect.right - GameRect.left;
			Height = GameRect.bottom - GameRect.top;
			DWORD dwStyle = GetWindowLong(GameWnd, GWL_STYLE);
			if (dwStyle & WS_BORDER)
			{
				GameRect.top += 32;
				Height -= 39;
			}
			ScreenCenterX = Width / 2;
			ScreenCenterY = Height / 2;
			MoveWindow(MyWnd, GameRect.left, GameRect.top, Width, Height, true);
		}
	}
}

bool CheckDriverStatus() {
	int icheck = 29;
	NTSTATUS status = 0;
	SLog(L"getting base");
	Unprotect(Driver::GetBaseAddress);
	uintptr_t BaseAddr = Driver::GetBaseAddress(Driver::currentProcessId);
	SLog(std::wstring(L"base: " + std::to_wstring(BaseAddr)).c_str());
	if (BaseAddr == 0) {
		return false;
	}
	Protect(Driver::GetBaseAddress);

	SLog(L"reading var");
	int checked = Driver::read<int>(Driver::currentProcessId, (uintptr_t)&icheck, &status);
	SLog(std::wstring(L"readed: " + std::to_wstring(checked)).c_str());
	if (checked != icheck) {
		return false;
	}

	return true;
}
DWORD GetProcessIdByName(wchar_t* name) {
	Protect(_ReturnAddress());

	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry) == TRUE) {
		while (Process32Next(snapshot, &entry) == TRUE) {
			if (_wcsicmp(entry.szExeFile, name) == 0) {
				Unprotect(_ReturnAddress());
				return entry.th32ProcessID;
			}
		}
	}

	CloseHandle(snapshot);
	Unprotect(_ReturnAddress());
	return 0;
}

uintptr_t milliseconds_now() {
	static LARGE_INTEGER s_frequency;
	static BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);
	if (s_use_qpc) {
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return (1000LL * now.QuadPart) / s_frequency.QuadPart;
	}
	else {
		return GetTickCount();
	}
}
void LoadProtectedFunctions() {

	uintptr_t t = milliseconds_now();
	BYTE xorkey = 0x0;
	for (DWORD i = 0; i < 8; i++) {
		xorkey = ((BYTE*)&t)[i];
		if (xorkey > 0x3 && xorkey < 0xf0) {
			break;
		}
	}
	if (xorkey <= 0x3 || xorkey >= 0xf0) {
		xorkey = 0x56;
	}

	addFunc({ LoadProtectedFunctions, (uintptr_t)CheckDriverStatus - (uintptr_t)LoadProtectedFunctions - 0x3, xorkey, false });
	addFunc({ CheckDriverStatus, (uintptr_t)GetProcessIdByName - (uintptr_t)CheckDriverStatus - 0x3, xorkey, false });
	addFunc({ GetProcessIdByName, (uintptr_t)milliseconds_now - (uintptr_t)GetProcessIdByName - 0x3, xorkey, false });

	addFunc({ Driver::SendCommand, (uintptr_t)Driver::GetBaseAddress - (uintptr_t)Driver::SendCommand - 0x3, xorkey, false });
	addFunc({ Driver::GetBaseAddress, (uintptr_t)Driver::copy_memory - (uintptr_t)Driver::GetBaseAddress - 0x3, xorkey, false });
	addFunc({ Driver::copy_memory, (uintptr_t)GetKernelModuleExport - (uintptr_t)Driver::copy_memory - 0x3, xorkey, false });
	addFunc({ GetKernelModuleExport, (uintptr_t)GetKernelModuleAddress - (uintptr_t)GetKernelModuleExport - 0x3, xorkey, false });
	addFunc({ GetKernelModuleAddress, (uintptr_t)Driver::initialize - (uintptr_t)GetKernelModuleAddress - 0x3, xorkey, false });
	addFunc({ Driver::initialize, (uintptr_t)Driver::read_memory - (uintptr_t)Driver::initialize - 0x3, xorkey, false });
	//addFunc({ Driver::read_memory, (uintptr_t)Driver::write_memory - (uintptr_t)Driver::read_memory - 0x3, xorkey, false });
	//addFunc({ Driver::write_memory, (uintptr_t)getEntity - (uintptr_t)Driver::write_memory - 0x3, xorkey, false });

	for (size_t i = 0; i < funcCount; i++) {
		if (functions[i].address != LoadProtectedFunctions)
			Protect(functions[i].address);
	}
	Unprotect(_ReturnAddress());
}
bool verify_game() {

	wchar_t name[] = { 'V', 'A', 'L', 'O', 'R', 'A', 'N', 'T', '-', 'W', 'i' , 'n' , '6', '4', '-' , 'S' , 'h', 'i', 'p', 'p', 'i', 'n', 'g', '.','e', 'x', 'e' , 0 };
	//VALORANT-Win64-Shipping
	Unprotect(GetProcessIdByName);
	pid = GetProcessIdByName(name);
	Protect(GetProcessIdByName);
	memset(name, 0, sizeof(name));

	Unprotect(Driver::GetBaseAddress);
	BaseAddr = Driver::GetBaseAddress(pid);
	Protect(Driver::GetBaseAddress);

	//printf("-> m_pid: %d base: %llx\n", pid, BaseAddr);

	if (BaseAddr != 0) {

		GamePid = pid;
		GameBaseAddress = BaseAddr;


		GamePid = 0;
		GameBaseAddress = 0;

	}
	//ProtectedSleep(2000);

	return true;
}