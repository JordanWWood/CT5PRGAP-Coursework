struct PixelShaderInput
{
	float4 PositionWS   : TEXCOORD1;
	float3 NormalWS     : TEXCOORD2;
	float2 TexCoord     : TEXCOORD0;
};

float4 SimplePixelShader( PixelShaderInput IN ) : SV_TARGET
{
	float4 texColor = { 1, 1, 1, 1 };
	if (Material.UseTexture)
		texColor = Texture.Sample(Sampler, IN.TexCoord);

    return texColor;
}