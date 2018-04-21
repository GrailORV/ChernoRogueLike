//=============================================================================
//
// �C���X�^���V���O�p�V�F�[�_�[ [Instancing.hlsl]
// Author : SORA ENOMOTO
//
//=============================================================================

//*********************************************************
// �萔�o�b�t�@
//*********************************************************
float4x4 View : VIEW;
float4x4 Proj : PROJECTION;

//*********************************************************
// ���͗p�\����
//*********************************************************
struct VS_INPUT
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 TexUV : TEXCOORD0;
	float4 Diffuse : COLOR0;
	float4 World0 : TEXCOORD1;
	float4 World1 : TEXCOORD2;
	float4 World2 : TEXCOORD3;
	float4 World3 : TEXCOORD4;
};

//*********************************************************
// �o�͗p�\����
//*********************************************************
struct VS_OUTPUT
{
	float4 Pos : POSITION;
	float4 Diffuse : COLOR0;
	float2 TexUV : TEXCOORD0;
};

//*********************************************************
// ���_�V�F�[�_�[
//*********************************************************
VS_OUTPUT VSmain(VS_INPUT vIn)
{
	VS_OUTPUT vOut;

	float4x4 World = {
		vIn.World0.x,vIn.World0.y,vIn.World0.z,vIn.World0.w,
		vIn.World1.x,vIn.World1.y,vIn.World1.z,vIn.World1.w,
		vIn.World2.x,vIn.World2.y,vIn.World2.z,vIn.World2.w,
		vIn.World3.x,vIn.World3.y,vIn.World3.z,vIn.World3.w,
	};

	vOut.Pos = mul(float4(vIn.Pos, 1), World);
	vOut.Pos = mul(vOut.Pos, View);
	vOut.Pos = mul(vOut.Pos, Proj);
	vOut.Diffuse = vIn.Diffuse;
	vOut.TexUV = vIn.TexUV;

	return vOut;
}

//*********************************************************
// �e�N�j�b�N
//*********************************************************
technique Effect
{
	pass P0
	{
		VertexShader = compile vs_3_0 VSmain();
	}
}