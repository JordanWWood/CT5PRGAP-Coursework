cbuffer PerApplication : register( b0 ) {
    matrix projectionMatrix;
}

cbuffer PerFrame : register( b1 ) {
    matrix viewMatrix;
}

cbuffer PerObject : register( b2 )  {
    matrix worldMatrix;
}

struct AppData {
	// Instance Buffer
	float3 instpos : POSITION1;

	// Vertex Buffer
    float3 position : POSITION0;
    float3 color: COLOR;
};

struct VertexShaderOutput {
	// TODO exploded texcoord
    float4 color : COLOR;
    float4 position : SV_POSITION;
};

float nrand(float2 uv) {
	return frac(sin(dot(uv, float2(12.9898, 78.233))) * 43758.5453);
}

VertexShaderOutput SimpleVertexShader( AppData IN ) {
    VertexShaderOutput OUT;
    matrix mvp = mul( projectionMatrix, mul( viewMatrix, worldMatrix ) );

	IN.position.x += IN.instpos.x;
	IN.position.y += IN.instpos.y;
	IN.position.z += IN.instpos.z;

    OUT.position = mul( mvp, float4( IN.position, 1.0f ) );
	OUT.color = float4(IN.color, 1.0f);

    return OUT;
}