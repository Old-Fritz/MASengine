////////////////////////////////////////////////////////////////////////////////
// Filename: light.ps
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture;
SamplerState SampleType;

//float g_sizeOfShaderTexture = 128;

cbuffer ParamsBuffer
{
	float4 ambientColor1;
	float4 diffuseColor1;
	float4 specularColor1;
	float specularPower1;
	float4 ambientColor2;
	float4 diffuseColor2;
	float4 specularColor2;
	float specularPower2;
	float3 cameraPosition;
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
	float3 lightPos1 : TEXCOORD2;
	float3 lightPos2 : TEXCOORD3;
};

float4 CalculateLight(float3 normal, float3 viewDirection, float3 lightPos1, float3 lightPos2);
////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 pixelShader(PixelInputType input) : SV_TARGET
{
	float4 color;
	float4 lightColor;
	float4 textureColor;

	///CALCULATE LIGHT///
	lightColor = CalculateLight(input.normal, input.viewDirection, input.lightPos1, input.lightPos2);
	textureColor = shaderTexture.Sample(SampleType, input.tex);

	color = lightColor * textureColor;
	return color;
}


float4 CalculateLight(float3 normal, float3 viewDirection, float3 lightPos1, float3 lightPos2)
{
	float4 color, color1, color2;
	float4 specular1, specular2;
	float lightIntensity1, lightIntensity2;
	float3 reflection1, reflection2;

	// Set the default output color to the ambient light value for all pixels.
	color1 = ambientColor1;
	color2 = ambientColor2;

	// Initialize the specular color.
	specular1 = float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular2 = float4(0.0f, 0.0f, 0.0f, 0.0f);


	// Calculate the amount of light on this pixel.
	lightIntensity1 = saturate(dot(normal, lightPos1));
	lightIntensity2 = saturate(dot(normal, lightPos2));


	if (lightIntensity1 > 0.0f)
	{
		// Determine the final diffuse color based on the diffuse color and the amount of light intensity.
		color1 += (diffuseColor1 * lightIntensity1);

		// Saturate the ambient and diffuse color.
		color1 = saturate(color1);

		// Calculate the reflection vector based on the light intensity, normal vector, and light direction.
		reflection1 = normalize(2 * lightIntensity1 * normal - lightPos1);

		// Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
		specular1 = pow(saturate(dot(reflection1, viewDirection)), specularPower1);
	}

	// Add the specular component last to the output color.
	color1 = saturate(color1 + specular1);

	if (lightIntensity2 > 0.0f)
	{
		// Determine the final diffuse color based on the diffuse color and the amount of light intensity.
		color2 += (diffuseColor2 * lightIntensity2);

		// Saturate the ambient and diffuse color.
		color2 = saturate(color2);

		// Calculate the reflection vector based on the light intensity, normal vector, and light direction.
		reflection2 = normalize(2 * lightIntensity2 * normal - lightPos2);

		// Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
		specular2 = pow(saturate(dot(reflection1, viewDirection)), specularPower2);
	}

	// Add the specular component last to the output color.
	color2 = saturate(color2 + specular2);

	// Multiply the texture pixel by the combination of all two light colors to get the final result.
	color = saturate(color1 + color2);

	return color;
}


