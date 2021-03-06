////////////////////////////////////////////////////////////////////////////////
// Filename: light.ps
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
Texture2D normalTexture : register(t0);
Texture2D depthTexture : register(t1);
Texture2D provTexture : register(t2);
Texture2D skyTexture : register(t3);
Texture2D waterTexture : register(t4);

SamplerState SampleType[2];

//float g_sizeOfShaderTexture = 128;

cbuffer ParamsBuffer
{
	float4 ambientColor1;
	float4 diffuseColor1;
	float4 specularColor1;
	float4 ambientColor2;
	float4 diffuseColor2;
	float4 specularColor2;
	float4 cameraPosition;
	float specularPower2;
	float specularPower1;
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
	float3 lightPos1 : TEXCOORD2;
	float3 lightPos2 : TEXCOORD3;
	float3 worldPos : TEXCOORD4;
};

float4 CalculateLight(float3 normal, float3 viewDirection, float3 lightPos1, float3 lightPos2);
float4 CalculateSkyColor(float3 pos);
////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 pixelShader(PixelInputType input) : SV_TARGET
{
	float2 reflectTexCoord, refractTexCoord;
	float3 normal;
	float4 color;
	float4 lightColor;
	float4 skyColor, depthColor, provColor, normalColor,waterColor, skyShadowColor;
	float reflectRefractScale = 1.6f;

	
	
	provColor = provTexture.Sample(SampleType[1], input.tex);
	if (provColor.z == 1)
		return float4(0,0,0,0);

	// Calculate the projected reflection texture coordinates.
	reflectTexCoord.x = input.worldPos.x/768;
	reflectTexCoord.y = (768-input.worldPos.z) / 768;
	refractTexCoord = input.tex;
	
	input.tex*=4;
	input.tex.y += waterTranslation;

	normalColor = normalTexture.Sample(SampleType[0], input.tex);

	// Expand the range of the normal from (0,1) to (-1,+1).
	normal = (normalColor.xyz * 2.0f) - 1.0f;

	// Re-position the texture coordinate sampling position by the normal map value to simulate the rippling wave effect
	reflectTexCoord += normal.xy*reflectRefractScale;
	refractTexCoord += normal.xy*reflectRefractScale;

	depthColor = depthTexture.Sample(SampleType[1], refractTexCoord) + float4(0.05f, 0.3f, 0, 0);
	waterColor = waterTexture.Sample(SampleType[0], reflectTexCoord);
	skyColor = skyTexture.Sample(SampleType[0], reflectTexCoord);
	
	waterColor = lerp(waterColor, depthColor, 0.6f);
	color = lerp(skyColor, waterColor, 0.75f);

	skyShadowColor = CalculateSkyColor(input.worldPos);

	normalColor = normalTexture.Sample(SampleType[0], input.tex *= 4+ normal.xy*reflectRefractScale- waterTranslation*2);

	// Expand the range of the normal from (0,1) to (-1,+1).
	normal = (normalColor.xyz * 2.0f) - 1.0f;

	lightColor = CalculateLight(float3(normal.x, normal.z, normal.y),input.viewDirection, input.lightPos1, input.lightPos2);
	//lightColor *= skyShadowColor;
	lightColor = lerp(lightColor, skyShadowColor, 0.5f);
	//textureColor = shaderTexture[1].Sample(SampleType[0], input.tex)

	color = lightColor*color;

	return color;
}


float4 CalculateLight(float3 normal, float3 viewDirection, float3 lightPos1, float3 lightPos2)
{
	float4 color, color1, color2;
	float4 specular1;
	float specular2;
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

	if (lightIntensity2 < 0.2f)
		lightIntensity2 += 0.4f;

	if (lightIntensity2 > 0.0f)
	{
		// Determine the final diffuse color based on the diffuse color and the amount of light intensity.
		color2 += (diffuseColor2 * lightIntensity2);

		// Saturate the ambient and diffuse color.
		color2 = saturate(color2);


		// Calculate the reflection vector using the normal and the direction of the light.
		reflection2 = -reflect(normalize(lightPos2), normal);

		// Calculate the specular light based on the reflection and the camera position.
		specular2 = dot(normalize(reflection2), normalize(viewDirection));

		// Check to make sure the specular was positive so we aren't adding black spots to the water.
		if (specular2 > 0.0f)
		{
			// Increase the specular light by the shininess value.
			specular2 = pow(specular2, specularPower2/10.0f);

			// Add the specular to the final color.
			color2 = saturate(color2 + specular2);
		}
	}

	// Multiply the texture pixel by the combination of all two light colors to get the final result.
	color = saturate(color1 + color2);

	return color;
}

float4 CalculateSkyColor(float3 pos)
{
	pos.x = pos.x / 768;
	pos.y = (768 - pos.z) / 768;

	float4 skyColor = skyTexture.Sample(SampleType[0], pos.xy);
	skyColor = float4(1 - skyColor.x, 1 - skyColor.y, 1 - skyColor.z, skyColor.w);
	if (skyColor.x > 1)
		skyColor.x = 1;
	if (skyColor.y > 1)
		skyColor.y = 1;
	if (skyColor.z > 1)
		skyColor.z = 1;
	//skyColor.w = 0.5f;
	return skyColor;
}