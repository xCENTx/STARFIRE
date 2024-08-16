#include "LID_Functions.h"
#include "Memory.h"

namespace LID
{

	bool FVector2D::IsValid() { return x == 0.f && y == 0.f; }
	float FVector2D::DotProduct(const FVector2D& other) { return (x * other.x) + (y * other.y); }
	float FVector2D::MagnitudeSqr() { return DotProduct(*this); }
	float FVector2D::Magnitude() { return sqrt(DotProduct(*this)); }
	float FVector2D::Distance(const FVector2D& other) { return powf(other.x - x, 2.f) + powf(other.y - y, 2.f); }
	float FVector2D::DistanceM(const FVector2D& other) { return Distance(other) * .01f; }

	bool FVector::IsValid() { return x == 0.f && y == 0.f; z == 0.f; }
	float FVector::DotProduct(const FVector& other) { return (x * other.x) + (y * other.y) + (z * other.z); }
	float FVector::MagnitudeSqr() { return DotProduct(*this); }
	float FVector::Magnitude() { return sqrt(DotProduct(*this)); }
	float FVector::Distance(const FVector& other) { return sqrt(powf(other.x - x, 2.f) + powf(other.y - y, 2.f) + powf(z - other.z, 2.f)); }
	float FVector::DistanceM(const FVector& other) { return Distance(other) * .01f; }


	FVector FRotator::ToVector() 
	{
		static float UROTTORAD = M_PI / 32768;

		float pitch = this->pitch * UROTTORAD;
		float yaw = this->yaw * UROTTORAD;
		float roll = this->roll * UROTTORAD;

		float cosPitch = cos(pitch);
		float sinPitch = sin(pitch);
		float cosYaw = cos(yaw);
		float sinYaw = sin(yaw);

		return FVector{ cosPitch * cosYaw, cosPitch * sinYaw, sinPitch };
	}

	FVector FRotator::ToDegrees()
	{
		static float half = 180.f;
		FVector res;

		res.x = ((float)this->roll / 32768) * half;
		res.y = ((float)this->pitch / 32768) * half;
		res.z = ((float)this->yaw / 32768) * half;

		return res;
	}

	FVector FRotator::ToRadians()
	{
		static float conv = M_PI / 180.f;
		FVector degress = this->ToDegrees();

		FVector res;
		res.x = degress.x * conv;
		res.y = degress.y * conv;
		res.z = degress.z * conv;

		return res;
	}


	FVector ToVector(FRotator rotator)
	{
		static float UROTTORAD = M_PI / 32768;

		float pitch = rotator.pitch * UROTTORAD;
		float yaw = rotator.yaw * UROTTORAD;
		float roll = rotator.roll * UROTTORAD;

		float cosPitch = cos(pitch);
		float sinPitch = sin(pitch);
		float cosYaw = cos(yaw);
		float sinYaw = sin(yaw);

		return FVector{ cosPitch * cosYaw, cosPitch * sinYaw, sinPitch };
	}

	void GetAxes(FRotator r, FVector& x, FVector& y, FVector& z)
	{
		x = ToVector(r);
		r.yaw += 16384;
		FRotator r2 = r;
		r2.pitch = 0;
		y = ToVector(r2);
		y.z = 0.0f;
		r.yaw -= 16384;
		r.pitch += 16384;
		z = ToVector(r);
	}

	bool WorldToScreen(FVector origin, FVector2D* screen)
	{
		// Get Player Controller
		auto pPlayer = GetLocalPC();
		if (!pPlayer)
			return false;

		// Get LocalPlayerCamera
		auto pCamera = GetLocalCamera();
		if (!pCamera)
			return false;

		//	Get Camera Information
		POV mCameraView;
		FRotator camRotation{};
		FVector vAxisX, vAxisY, vAxisZ, vDelta, vTransformed, camLocation;
		if (!GetCameraView(&mCameraView))
			return false;

		camLocation = mCameraView.Location;
		camRotation = mCameraView.Rotation;

		//	pCamera->GetCameraViewPoint(&camLocation, &camRotation);
		GetAxes(camRotation, vAxisX, vAxisY, vAxisZ);

		//	Calculate Transformed Position
		vDelta = origin - camLocation;
		vTransformed.x = vDelta.DotProduct(vAxisY);
		vTransformed.y = vDelta.DotProduct(vAxisZ);
		vTransformed.z = vDelta.DotProduct(vAxisX);
		if (vTransformed.z < 1.00f)
			return false;

		//	Get Screen & FOV
		float mFOV = mCameraView.FOV;
		FVector2D center = FVector2D(szScreen.x * .5f, szScreen.y * .5f);

		//	Transform to Screen Space
		FVector2D Screen2D;
		Screen2D.x = center.x + vTransformed.x * (center.x / tanf(mFOV * M_PI / 360.f)) / vTransformed.z;
		Screen2D.y = center.y - vTransformed.y * (center.x / tanf(mFOV * M_PI / 360.f)) / vTransformed.z;

		*screen = Screen2D;

		return true;
	}

	__int64 GetLocalPawn()
	{
		//	UBrgUiManager -> mPawnPlayerBase
		auto pUIMan = g_Memory->Read<long long>(g_Memory->GetModuleBase() + Offsets::GBrgUiMan);
		if (!pUIMan)
			return 0;

		return g_Memory->Read<long long>(pUIMan + 0x2D1C);
	}

	__int64 GetDroppedItem()
	{
		//	UBrgUiManager -> mDropItemManager
		auto pUIMan = g_Memory->Read<long long>(g_Memory->GetModuleBase() + Offsets::GBrgUiMan);
		if (!pUIMan)
			return 0;

		return g_Memory->Read<long long>(pUIMan + 0x2850);
	}

	FVector GetDroppedItemLocation(__int64 pItem)
	{
		return g_Memory->Read<FVector>((pItem + 0x74) + 0x1C);
	}

	__int64 GetDroppedItemActor(__int64 pItem)
	{
		return g_Memory->Read<long long>((pItem + 0x74) + 0x14);
	}

	__int64 GetGameInfo()
	{
		//	UBrgUiManager -> mGameInfoNative

		auto pUIMan = g_Memory->Read<long long>(g_Memory->GetModuleBase() + Offsets::GBrgUiMan);
		if (!pUIMan)
			return 0;
	
		return g_Memory->Read<long long>(pUIMan + 0x27EC);
	}

	__int64 GetLocalPC()
	{
		// localPawn -> Controller

		long long pPawn = GetLocalPawn();
		if (!pPawn)
			return 0;

		return g_Memory->Read<long long>(pPawn + 0x2A0);
	}

	__int64 GetLocalCamera()
	{

		// localPawn -> Controller -> Camera

		long long pPC = GetLocalPC();
		if (!pPC)
			return 0;

		return g_Memory->Read<long long>(pPC + 0x47C);
	}

	bool GetCameraView(POV* pView)
	{
		// localPawn -> Controller -> Camera -> CameraCache -> FTPOV

		long long pCamera = GetLocalCamera();
		if (!pCamera)
			return false;

		*pView = g_Memory->Read<POV>(pCamera + 0x500);
		
		return true;
	}

	FVector GetPawnLocation(__int64 pPawn)
	{
		// localPawn -> location (AActor)

		return g_Memory->Read<FVector>(pPawn + 0x80);
	}

	FRotator GetPawnRotation(__int64 pPawn)
	{
		// localPawn -> location (AActor)

		return g_Memory->Read<FRotator>(pPawn + 0x8C);
	}

	bool GetPawnMatrix(__int64 pPawn, FMatrix* out)
	{
		//	APawn -> CollisionComponent -> LocalToWorld

		auto pCollision = g_Memory->Read<long long>(pPawn + 0x01F8);//	PrimitiveComponent*
		if (!Memory::IsValidPtr(pCollision))
			return false;

		*out = g_Memory->Read<FMatrix>(pCollision + 0xC0);

		return true;
	}

	bool GetPawnFwdVector(__int64 pPawn, FVector* fwd)
	{
		LID::FMatrix mat;
		if (!LID::GetPawnMatrix(pPawn, &mat))
			return false;
		
		*fwd = mat.xPlane;

		return true;
	}

	bool GetPawnRightVector(__int64 pPawn, FVector* right)
	{
		LID::FMatrix mat;
		if (!LID::GetPawnMatrix(pPawn, &mat))
			return false;

		*right = mat.yPlane;

		return true;
	}

	bool GetPawnUpVector(__int64 pPawn, FVector* up)
	{
		LID::FMatrix mat;
		if (!LID::GetPawnMatrix(pPawn, &mat))
			return false;

		*up = mat.zPlane;

		return true;
	}

	bool GetPawnBounds(__int64 pPawn, AABB* box)
	{
		//	APawn -> CollisionComponent -> Bounds
		
		auto pCollision = g_Memory->Read<long long>(pPawn + 0x01F8);//	PrimitiveComponent*
		if (!Memory::IsValidPtr(pCollision))
			return false;

		auto boxExtents = g_Memory->Read<FBoxSphereBounds>(pCollision + 0x90);//	FBoxSphereBounds
		FVector mmin = boxExtents.Origin - boxExtents.BoxExtent;
		FVector mmax = boxExtents.Origin + boxExtents.BoxExtent;

		*box = AABB(mmin, mmax);

		return true;
	}

	bool IsPawnVisible(__int64 pPawn)
	{
		//	APawn -> CollisionComponent -> LastSubmitTime & LastRenderTime
		auto pCollision = g_Memory->Read<long long>(pPawn + 0x01F8);//	PrimitiveComponent*
		if (!Memory::IsValidPtr(pCollision))
			return false;
		
		auto lastSubmit = g_Memory->Read<float>(pCollision + 0x022C);	//	LastSubmitTime
		auto lastRender = g_Memory->Read<float>(pCollision + 0x0234);	//	LastRenderTime

		//	@TODO: get visible time 

		return false;
	}

	void TeleportLocalPlayer(const FVector& location)
	{
		//	UBrgUiManager -> mPawnPlayerBase -> location = location

		auto pPawn = GetLocalPawn();//	UBrgUiManager -> mPawnPlayerBase
		if (!Memory::IsValidPtr(pPawn))
			return;
		
		TeleportPawn(pPawn, location);
	}

	void TeleportPawn(__int64 pPawn, const FVector& location)
	{
		//	UBrgUiManager -> mPawnPlayerBase -> location = location
		if (!Memory::IsValidPtr(pPawn))
			return;

		g_Memory->Write<FVector>(pPawn + 0x80, location);	//	APawn -> Location
	}

	void ReturnToSafeZone()
	{
		//	UBrgUiManager -> mGameInfoNative -> mbPlayerEmergency = 1 [ bitfield , setting 0x1514 to value of 30 - 50 will trigger a emergency exit ]

		auto pPawn = GetLocalPawn();	//	UBrgUiManager -> mPawnPlayerBase
		if (!Memory::IsValidPtr(pPawn))
			return;
		
		auto pGame = GetGameInfo();	//	UBrgUiManager -> mGameInfoNative
		if (!Memory::IsValidPtr(pGame))
			return;

		g_Memory->Write<__int32>(pGame + 0x1514, 32);	//	ABrgGameInfo -> mbPlayerEmergency
	}

	bool GetNearestEscalator(FVector* out)
	{
		//	UBrgUiManager -> mGameInfoNative -> mElevator2Locations 

		auto pGame = GetGameInfo();	//	UBrgUiManager -> mGameInfoNative
		if (!Memory::IsValidPtr(pGame))
			return false;

		auto elevators = g_Memory->Read<TArray<FVector>>(pGame + 0x0634);//	ABrgGameInfoNativeBase -> mEscalatorLocations // TArray<FVector>
		auto pData = elevators.Data();
		if (elevators.Count() <= 0)
			return false;

		*out = g_Memory->Read<FVector>(__int64(pData));

		return !out->IsValid();
	}

	bool GetNearestElevator(FVector* out)
	{
		//	UBrgUiManager -> mGameInfoNative -> mElevator2Locations 

		auto pGame = GetGameInfo();	//	UBrgUiManager -> mGameInfoNative
		if (!Memory::IsValidPtr(pGame))
			return false;

		auto elevators = g_Memory->Read<TArray<FVector>>(pGame + 0x0794);//	ABrgGameInfoNativeBase -> mElevator2Locations // TArray<FVector>
		auto pData = elevators.Data();
		if (elevators.Count() <= 0)
			return false;

		*out = g_Memory->Read<FVector>(__int64(pData));
		
		return !out->IsValid();
	}

	bool GetPawnBones(__int64 pPawn, std::vector<FVector>* bones)
	{
		auto pMesh = g_Memory->Read<long long>(pPawn + 0x04A8);
		if (!Memory::IsValidPtr(pMesh))
			return false;
		
		auto atoms = g_Memory->Read<TArray<FBoneAtom>>(pMesh + 0x328);
		if (atoms.Count() <= 0)
			return false;

		std::vector<FVector> res;
		auto pData = __int64(atoms.Data());
		for (int i = 0; i < atoms.Count(); i++)
		{
			auto bone = g_Memory->Read<FBoneAtom>(pData + (i * sizeof(FBoneAtom)));

			res.push_back(bone.Translation);
		}

		*bones = res;

		return res.size() > 0;
	}
}