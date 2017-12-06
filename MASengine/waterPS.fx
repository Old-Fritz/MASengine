////////////////////////////////////////////////////////////////////////////////
// Filename: light.ps
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture[3];
SamplerState SampleType[2];

//float g_sizeOfShaderTexture = 128;

cbuffer ParamsBuffer
{
	float4 ambientColor;
	float4 diffuseColor;
	float4 specularColor;
	float3 lightDirection;
	float specularPower;
	float3 cameraPosition;
	float waterTranslation;
};


//////////////
// TYPEDEFS //
//////////////

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
};

float4 CalculateLight(float3 normal);
////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 pixelShader(PixelInputType input) : SV_TARGET
{
	float4 color;
	float4 lightColor;
	float4 textureColor, provColor, normalColor;
	float2 newTex;


	//return shaderTexture[2].Sample(SampleType[1], input.tex);

	provColor = shaderTexture[2].Sample(SampleType[1], input.tex);
	if (provColor.z == 1)
		return float4(0,0,0,0);

	input.tex *= 2.0f;
	input.tex.y += waterTranslation;

	normalColor = shaderTexture[0].Sample(SampleType[0], input.tex);
	lightColor = CalculateLight(normalColor.xyz);
	textureColor = shaderTexture[1].Sample(SampleType[0], input.tex);

	color = lightColor*textureColor;

	

	return color;
}


float4 CalculateLight(float3 normal)
{
	float4 color;
	float4 specular;
	float3 lightDir;
	float lightIntensity;

	// Set the default output color to the ambient light value for all pixels.
	color = ambientColor;

	// Initialize the specular color.
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Invert the light direction for calculations.
	lightDir = -lightDirection;

	// Calculate the amount of light on this pixel.
	lightIntensity = saturate(dot(normal, lightDir));

	if (lightIntensity > 0.0f)
	{
		// Determine the final diffuse color based on the diffuse color and the amount of light intensity.
		color += (diffuseColor * lightIntensity);
	}

	// Saturate the ambient and diffuse color.
	color = saturate(color);

	return color;
}