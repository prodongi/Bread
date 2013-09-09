
static const int MAX_MAT_SKIN = 30;
float4x4 matSkin[MAX_MAT_SKIN];
int skinLinkNum;

void skinAni(inout float4 _skinPos,
			 inout float3 _skinNormal,
			 float4 pos,
			 float3 normal,
			 float4 blendWeights,
			 float4 blendIndices)
{
	float3 skinPos, skinNormal;
	int4 indexVector = D3DCOLORtoUBYTE4(blendIndices);
	
	float blendWeightsArray[4] = (float[4])blendWeights;
	int indexArray[4] = (int[4])indexVector;
	
	skinPos = 0.0f;
	skinNormal = 0.0f;
	float lastWeight = 0.0f;
	for (int i = 0; i < skinLinkNum; ++i)
	{
		lastWeight = lastWeight + blendWeightsArray[i];
		
		skinPos += mul(pos, matSkin[indexArray[i]]) * blendWeightsArray[i];
		skinNormal += mul(normal, matSkin[indexArray[i]]) * blendWeightsArray[i];
	}
	
	lastWeight = 1.0f - lastWeight;
	skinPos += mul(pos, matSkin[indexArray[skinLinkNum-1]]) * lastWeight;
	skinNormal += mul(normal, matSkin[indexArray[skinLinkNum-1]]) * lastWeight;
	
	_skinPos = float4(skinPos, 1.0f);	
	_skinNormal = skinNormal;
}			 











