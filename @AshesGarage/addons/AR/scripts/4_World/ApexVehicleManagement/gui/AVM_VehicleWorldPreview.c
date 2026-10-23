#ifndef SERVER
// Left in the PBO so old references compile. Do not spawn a world camera from the garage.
class AVM_VehicleWorldPreview
{
	void AVM_VehicleWorldPreview(Widget viewport)
	{
	}

	void Build(string vehicleClass, array<string> storedParts)
	{
	}

	void Rotate(float degrees)
	{
	}

	void Zoom(int wheel)
	{
	}

	bool OwnsWidget(Widget widget)
	{
		return false;
	}

	void RestorePlayerCamera()
	{
	}

	void Clear()
	{
	}
};
#endif
