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
	float3 instrot : ROTATION1;

	// Vertex Buffer
    float3 position : POSITION0;
    float3 color: COLOR;
};

struct VertexShaderOutput {
    float4 color : COLOR;
    float4 position : SV_POSITION;
};

matrix AngleAxis4x4(float angle, float3 axis) {
	float c, s;
	sincos(angle, s, c);

	float t = 1 - c;
	float x = axis.x;
	float y = axis.y;
	float z = axis.z;

	return matrix(
		t * x * x + c, t * x * y - s * z, t * x * z + s * y, 0,
		t * x * y + s * z, t * y * y + c, t * y * z - s * x, 0,
		t * x * z - s * y, t * y * z + s * x, t * z * z + c, 0,
		0, 0, 0, 1
		);
}

VertexShaderOutput SimpleVertexShader( AppData IN ) {
    VertexShaderOutput OUT;

    matrix mvp = mul( projectionMatrix, mul( viewMatrix, worldMatrix ) );
	float3 offset = float3(IN.position.x + IN.instpos.x, IN.position.y + IN.instpos.y, IN.position.z + IN.instpos.z);

	matrix rotation = mul(AngleAxis4x4(IN.instrot.z, float3(0, 0, 1)), mul(AngleAxis4x4(IN.instrot.y, float3(0, 1, 0)), AngleAxis4x4(IN.instrot.x, float3(1, 0, 0))));
	matrix translation = matrix(/*1*/ 1, 0, 0, 0, /*2*/ 0, 1, 0, 0, /*3*/ 0, 0, 1, 0, /*4*/ 0, 0, 0, 1);
	matrix scale = matrix(/*1*/ 1, 0, 0, 0, /*2*/ 0, 1, 0, 0, /*3*/ 0, 0, 1, 0, /*4*/ 0, 0, 0, 1);

	OUT.position = mul(mul(mul(translation, rotation), scale), float4(offset, 1.0f)); // TRS
	OUT.position = mul(mvp, OUT.position);

	OUT.color = float4(IN.color, 1.0f);

    return OUT;
}