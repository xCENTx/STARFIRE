#pragma once

#define _USE_MATH_DEFINES
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
#include <cmath>
#include <string>

namespace LID
{
	__int64 GetLocalPawn();
	__int64 GetLocalPC();
	__int64 GetLocalCamera(); 
	__int64 GetDroppedItem();
	__int64 GetDroppedItemActor(__int64 pItem);
	__int64 GetGameInfo();
	void ReturnToSafeZone();
	void TeleportPawn(__int64 pPawn, const struct FVector& location);
	void TeleportLocalPlayer(const struct FVector& location);
	bool GetNearestEscalator(struct FVector* out);
	bool GetNearestElevator(struct FVector* out);
	bool GetCameraView(struct POV* pView);
	bool GetPawnBounds(__int64 pPawn, struct AABB* box); 
	bool GetPawnBones(__int64 pPawn, std::vector<FVector>* bones);
	bool GetPawnMatrix(__int64 pPawn, struct FMatrix* out);
	bool GetPawnFwdVector(__int64 pPawn, FVector* fwd);
	bool GetPawnRightVector(__int64 pPawn, FVector* right);
	bool GetPawnUpVector(__int64 pPawn, FVector* up);
	bool IsPawnVisible(__int64 pPawn);
	struct FVector GetPawnLocation(__int64 pPawn);
	struct FRotator GetPawnRotation(__int64 pPawn);
	FVector GetDroppedItemLocation(__int64 pItem);
	bool WorldToScreen(struct FVector WorldLocation, struct FVector2D* Screen2D);
}

#include "LID_Classes.h"
