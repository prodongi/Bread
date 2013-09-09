
#ifdef _VEGETATION_ANI_

uniform float vegetationTime;
uniform float vegetationY;
const float magnitude = 0.05f;

void vegetationAni(inout float4 pos)
{
	float amplitude = magnitude * (max(0, pos.y - vegetationY));
	float4 wave = amplitude * float4(sin(vegetationTime), 0, cos(vegetationTime), 0);
	float4 offset = pos + wave;
	pos = offset;
}

#endif