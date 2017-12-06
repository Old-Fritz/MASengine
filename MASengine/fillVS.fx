////////////////////////////////////////////////////////////////////////////////
// Filename: fillVS.fx
////////////////////////////////////////////////////////////////////////////////



/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	float4 clipPlane;
};



//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
	float4 position : POSITION;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float clip : SV_ClipDistance0;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType vertexShader(VertexInputType input)
{
	PixelInputType output;
	float4 worldPosition;

	// Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Set the clipping plane.
	output.clip = dot(mul(input.position, worldMatrix), clipPlane);

	return output;
}