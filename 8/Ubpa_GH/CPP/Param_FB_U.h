#pragma once
#include "param_fb.h"
class Param_FB_U :
	public Param_FB
{
private:
	std::vector<float> getWeights(HE_vert * vert, Mesh3D * ptr_mesh);
};

