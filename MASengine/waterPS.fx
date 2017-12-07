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

float4 CalculateLight(float3 normal, float3 viewDirection);
////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 pixelShader(PixelInputType input) : SV_TARGET
{
	float2 reflectTexCoord;
	float3 normal;
	float4 color;
	float4 lightColor;
	float4 textureColor, provColor, normalColor;
	float2 newTex;


	//return shaderTexture[2].Sample(SampleType[1], input.tex);

	provColor = shaderTexture[2].Sample(SampleType[1], input.tex);
	if (provColor.z == 1)
		return float4(0,0,0,0);

	input.tex /= 4;
	input.tex.y += waterTranslation;

	normalColor = shaderTexture[0].Sample(SampleType[0], input.tex);
	lightColor = CalculateLight(input.normal,input.viewDirection);
	//textureColor = shaderTexture[1].Sample(SampleType[0], input.tex);

	// Calculate the projected reflection texture coordinates.
	//reflectTexCoord.x = input.position.x / input.position.w / 2.0f + 0.5f;
	//reflectTexCoord.y = -input.position.y / input.position.w / 2.0f + 0.5f;

	// Expand the range of the normal from (0,1) to (-1,+1).
	normal = (normalColor.xyz * 2.0f) - 1.0f;

	// Re-position the texture coordinate sampling position by the normal map value to simulate the rippling wave effect.
	reflectTexCoord = input.tex + (normal.xy);

	// Sample the texture pixels from the textures using the updated texture coordinates.
	textureColor = shaderTexture[1].Sample(SampleType[0], reflectTexCoord);

	color = lightColor*textureColor;

	

	return color;
}


float4 CalculateLight(float3 normal, float3 viewDirection)
{
	float4 color;
	float4 specular;
	float3 lightDir;
	float lightIntensity;
	float3 reflection;

	// Expand the range of the normal from (0,1) to (-1,+1).
	//normal = (normal * 2.0f) - 1.0f;

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

		// Saturate the ambient and diffuse color.
		color = saturate(color);

		// Calculate the reflection vector based on the light intensity, normal vector, and light direction.
		reflection = normalize(2 * lightIntensity * normal - lightDir);

		// Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
		specular = pow(saturate(dot(reflection, viewDirection)), specularPower);
	}

	// Add the specular component last to the output color.
	color = saturate(color + specular);

	return color;


}