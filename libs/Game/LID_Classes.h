#pragma once
namespace LID
{

	namespace Offsets
	{
		static constexpr auto GObjects = 0x022A4128;
		static constexpr auto GNames = 0x022A40E0;
		static constexpr auto GWorld = 0x022B9D40;
		static constexpr auto GBrgUiMan = 0x0F242EE0;
	};

	struct FDouble
	{
		int A;	//0x0000
		int B;	//0x0004
	};	//Size: 0x0008

	struct FVector2D
	{
		float x, y;
		FVector2D() : x(0), y(0) {}
		FVector2D(float x, float y) : x(x), y(y) {}

		FVector2D operator+(const FVector2D& other) const { return FVector2D(x + other.x, y + other.y); }
		FVector2D operator-(const FVector2D& other) const { return FVector2D(x - other.x, y - other.y); }
		FVector2D operator*(const FVector2D& other) const { return FVector2D(x * other.x, y * other.y); }
		FVector2D operator/(const FVector2D& other) const { return FVector2D(x / other.x, y / other.y); }
		FVector2D& operator=(const FVector2D& other) { x = other.x; y = other.y; return *this; }

		bool IsValid();
		float DotProduct(const FVector2D& other);
		float MagnitudeSqr();
		float Magnitude();
		float Distance(const FVector2D& other);
		float DistanceM(const FVector2D& other);
	};

	struct FVector
	{
		float x, y, z;
		FVector() : x(0), y(0), z(0) {}
		FVector(float x, float y, float z) : x(x), y(y), z(z) {}

		FVector operator+(const float scalar) const { return FVector(x + scalar, y + scalar, z + scalar); }
		FVector operator+(const FVector& other) const { return FVector(x + other.x, y + other.y, z + other.z); }
		FVector operator-(const float scalar) const { return FVector(x - scalar, y - scalar, z - scalar); }
		FVector operator-(const FVector& other) const { return FVector(x - other.x, y - other.y, z - other.z); }
		FVector operator*(const float scalar) const { return FVector(x * scalar, y * scalar, z * scalar); }
		FVector operator*(const FVector& other) const { return FVector(x * other.x, y * other.y, z * other.z); }
		FVector operator/(const float scalar) const { return FVector(x / scalar, y / scalar, z / scalar); }
		FVector operator/(const FVector& other) const { return FVector(x / other.x, y / other.y, z / other.z); }
		FVector& operator=(const FVector& other) { x = other.x; y = other.y; z = other.z; return *this; }

		FVector& operator+=(const float scalar) { x += scalar; y += scalar; z += scalar; return *this; }
		FVector& operator+=(const FVector& other) { x += other.x; y += other.y; z += other.z; return *this; }
		FVector& operator-=(const float scalar) { x -= scalar; y -= scalar; z -= scalar; return *this; }
		FVector& operator-=(const FVector& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
		FVector& operator*=(const float scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
		FVector& operator*=(const FVector& other) { x *= other.x; y *= other.y; z *= other.z; return *this; }
		FVector& operator/=(const float scalar) { x /= scalar; y /= scalar; z /= scalar; return *this; }
		FVector& operator/=(const FVector& other) { x /= other.x; y /= other.y; z /= other.z; return *this; }


		bool IsValid();
		float DotProduct(const FVector& other);
		float MagnitudeSqr();
		float Magnitude();
		float Distance(const FVector& other);
		float DistanceM(const FVector& other);
	};

	struct FRotator
	{
		__int32 pitch, yaw, roll;
		FRotator() : pitch(0), yaw(0), roll(0) {}
		FRotator(float x, float y, float z) : pitch(x), yaw(y), roll(z) {}

		FRotator operator+(const float scalar) const { return FRotator(pitch + scalar, yaw + scalar, roll + scalar); }
		FRotator operator+(const FRotator& other) const { return FRotator(pitch + other.pitch, yaw + other.yaw, roll + other.roll); }
		FRotator operator-(const float scalar) const { return FRotator(pitch - scalar, yaw - scalar, roll - scalar); }
		FRotator operator-(const FRotator& other) const { return FRotator(pitch - other.pitch, yaw - other.yaw, roll - other.roll); }
		FRotator operator*(const float scalar) const { return FRotator(pitch * scalar, yaw * scalar, roll * scalar); }
		FRotator operator*(const FRotator& other) const { return FRotator(pitch * other.pitch, yaw * other.yaw, roll * other.roll); }
		FRotator operator/(const float scalar) const { return FRotator(pitch / scalar, yaw / scalar, roll / scalar); }
		FRotator operator/(const FRotator& other) const { return FRotator(pitch / other.pitch, yaw / other.yaw, roll / other.roll); }
		FRotator& operator=(const FRotator& other) { pitch = other.pitch; yaw = other.yaw; roll = other.roll; return *this; }

		FRotator& operator+=(const float scalar) { pitch += scalar; yaw += scalar; roll += scalar; return *this; }
		FRotator& operator+=(const FRotator& other) { pitch += other.pitch; yaw += other.yaw; roll += other.roll; return *this; }
		FRotator& operator-=(const float scalar) { pitch -= scalar; yaw -= scalar; roll -= scalar; return *this; }
		FRotator& operator-=(const FRotator& other) { pitch -= other.pitch; yaw -= other.yaw; roll -= other.roll; return *this; }
		FRotator& operator*=(const float scalar) { pitch *= scalar; yaw *= scalar; roll *= scalar; return *this; }
		FRotator& operator*=(const FRotator& other) { pitch *= other.pitch; yaw *= other.yaw; roll *= other.roll; return *this; }
		FRotator& operator/=(const float scalar) { pitch /= scalar; yaw /= scalar; roll /= scalar; return *this; }
		FRotator& operator/=(const FRotator& other) { pitch /= other.pitch; yaw /= other.yaw; roll /= other.roll; return *this; }

		FVector ToVector();
		FVector ToDegrees();
		FVector ToRadians();
	}; 

	struct FPlane : FVector
	{
		float w;
	};

	struct FQuat
	{
		float x;	//0x0000
		float y;	//0x0004
		float z;	//0x0008
		float w;	//0x000C
	};	//Size: 0x0010

	struct FMatrix
	{
		FPlane xPlane, yPlane, zPlane, wPlane;
		FMatrix() : xPlane(FPlane()), yPlane(FPlane()), zPlane(FPlane()), wPlane(FPlane()) {}
		FMatrix(FPlane x, FPlane y, FPlane z, FPlane w) : xPlane(x), yPlane(y), zPlane(z), wPlane(w) {}
	};

	struct FBoxSphereBounds
	{
		FVector Origin;	//0x0000
		FVector BoxExtent;	//0x000C
		float SphereRadius;	//0x0018
	};	//Size: 0x001C

	struct FBoneAtom
	{
		FQuat Rotation;	//0x0000
		FVector Translation;	//0x0010
		float N00001160;	//0x001C
	};	//Size: 0x0020


	struct AABB
	{
		FVector mmin;
		FVector mmax;

		AABB() : mmin(FVector()), mmax(FVector()) {}
		AABB(FVector min, FVector max) : mmin(min), mmax(max) {}


		FVector Center() const { return (mmin + mmax) / 2; }
		FVector Size() const { mmax - mmin; }
		FVector Extents() const { return Size() * .5f; }
	};

	template<typename T>
	class TArray
	{
	private:
		T* _data;        
		__int32 _count;  
		__int32 _max;    

	public:
		TArray()
		{
			_data = nullptr;
			_count = 0;
			_max = 0;
		}

		T* Data() const
		{
			return _data;
		}

		__int32 Count() const
		{
			return _count;
		}

		__int32 Max() const
		{
			return _max;
		}

		bool IsValidIndex(__int32 i) const
		{
			return i < _count;
		}

		T& operator[](__int32 i)
		{
			return _data[i];
		}

		const T& operator[](__int32 i) const
		{
			return _data[i];
		}
	};

	struct POV
	{
	public:
		struct FVector Location;  
		struct FRotator Rotation; 
		float FOV;                
	};

	inline FVector2D szScreen;
}