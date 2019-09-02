////////////////////////////////////////////////////////////////////////////////
// Filename: light.ps
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
Texture2D textures[13];
SamplerState SampleType[3];

//float g_sizeOfShaderTexture = 128;

cbuffer ParamsBuffer
{
    float4 ambientColor1;
    float4 diffuseColor1;
    float4 specularColor1;
	float4 ambientColor2;
	float4 provsColor[256];
	float4 diffuseColor2;
	float4 specularColor2;
	float4 cameraPosition;
	float specularPower2;
	float specularPower1;
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
	float clip : SV_ClipDistance0;
	float3 lightPos1 : TEXCOORD2;
	float3 lightPos2 : TEXCOORD3;
	float3 worldPos : TEXCOORD4;
};

float4 CalculateLight(float3 normal, float3 viewDirection, float3 lightPos1, float3 lightPos2);
float4 CalculatePhysicalMap(PixelInputType input);
float4 CalculateCurrentPhysical(float4 color, float2 mapCoord);
bool NearColor(float4 color1, float4 color2);
float4 ConvertRGB256(float r, float g, float b);
float4 CalculateSkyColor(float3 pos);

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 pixelShader(PixelInputType input) : SV_TARGET
{
	float4 terrainColor;
	float4 lightColor;
    float4 color;
    float4 provColor;
    float4 provColor2;
    float4 tempColor;
    int provnum;
    int provnum2;
    float2 mapCoord;
	float4 skyColor;

	///CALCULATE BORDERS///
    provColor = textures[0].Sample(SampleType[0], input.tex);
	provColor2 = textures[0].Sample(SampleType[1], input.tex);

    provnum2 = provColor2.z * 255.01f;
    provnum = provColor.z * 255.01f;

    if (provColor.x > 0 || provColor.y > 0)
    {
        if (provColor.x >= 0.9f || provColor.y >= 0.9f)
        {
            color = float4(0.3f, 0.3f, 0.7f, 1.0f);
            provnum = 400;
        }
        else if (provColor.x > 0.25f || provColor.y > 0.25f)
        {
            provColor = float4(0.4f, 0.4f, 0.4f, 1);
            provnum = 300;
        }
        else
        {
            provColor = provColor2;
            provnum = provnum2;
            if (provColor.x > 0 || provColor.y > 0)
            {
                provColor = float4(0.4f, 0.4f, 0.4f, 1);
                provnum = 300;
            }
        }
    }
    else
    {
        provColor = provColor2;
        provnum = provnum2;
    }
	

	///CALCULATE LIGHT///
	lightColor = CalculateLight(input.normal, input.viewDirection, input.lightPos1, input.lightPos2);
	skyColor = CalculateSkyColor(input.worldPos);
	lightColor *= skyColor;
	//return color;


	terrainColor = CalculatePhysicalMap(input);

	///CALCULATE FINAL///

    if (provnum < 255)
    {
        provColor = provsColor[provnum];
        //provColor = lerp(provColor, yellow, provColor.w / 1.6f);
    }
	color = lerp(terrainColor, provColor, 0.4f);
    //color = lerp(lightColor, color, 0.35f);
	color = color * lightColor;
    color.w = 1.0f;
	
   if (provnum == 400)
        color = terrainColor * lightColor;

    //return textures[1].Sample(SampleType[1], input.tex);
	return  color;
}


float4 CalculateLight(float3 normal,float3 viewDirection, float3 lightPos1, float3 lightPos2)
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
			specular2 = pow(specular2, specularPower2);

			// Add the specular to the final color.
			color2 = saturate(color2 + specular2);
		}
	}

	// Multiply the texture pixel by the combination of all two light colors to get the final result.
	color = saturate(color1 + color2);

	return color;
}

float4 CalculatePhysicalMap(PixelInputType input)
{
	float4 textureColor, textureColorLeft, textureColorRight, textureColorUp, textureColorDown;
	float2 texCoordLeft, texCoordRight, texCoordUp, texCoordDown;
	float2 squereCoord, mapCoord;
	float sizeOfShaderTexture = 128;

	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	//textureColor = textures[1].Sample(SampleType[1], input.tex*(sizeOfShaderTexture / 129.0f));
	textureColor = textures[1].Sample(SampleType[1], (input.tex*sizeOfShaderTexture + 1.0f) / (sizeOfShaderTexture + 2.0f) - 0.00003f);
	squereCoord.x = (input.tex.x*sizeOfShaderTexture) - (int)(input.tex.x*sizeOfShaderTexture);
	squereCoord.y = (input.tex.y*sizeOfShaderTexture) - (int)(input.tex.y*sizeOfShaderTexture);
	//squereCoord = (input.tex*sizeOfShaderTexture + 1.0f);
	texCoordLeft = texCoordRight = texCoordUp = texCoordDown = (input.tex*sizeOfShaderTexture + 1.0f) / (sizeOfShaderTexture + 2.0f) - 0.00003f;
	texCoordLeft.x -= 1.0f / (sizeOfShaderTexture + 2.0f);
	texCoordRight.x += 1.0f / (sizeOfShaderTexture + 2.0f);
	texCoordUp.y -= 1.0f / (sizeOfShaderTexture + 2.0f);
	texCoordDown.y += 1.0f / (sizeOfShaderTexture + 2.0f);
	textureColorLeft = textures[1].Sample(SampleType[1], texCoordLeft);
	textureColorRight = textures[1].Sample(SampleType[1], texCoordRight);
	textureColorUp = textures[1].Sample(SampleType[1], texCoordUp);
	textureColorDown = textures[1].Sample(SampleType[1], texCoordDown);
	//return textureColorLeft;

	mapCoord = input.tex * 16;

	textureColor = CalculateCurrentPhysical(textureColor, mapCoord);
	//return textureColor;
	textureColorLeft = CalculateCurrentPhysical(textureColorLeft, mapCoord);
	textureColorRight = CalculateCurrentPhysical(textureColorRight, mapCoord);
	textureColorUp = CalculateCurrentPhysical(textureColorUp, mapCoord);
	textureColorDown = CalculateCurrentPhysical(textureColorDown, mapCoord);

	float4 horColor;
	if (squereCoord.x > 0.5f)
		horColor = textureColor*(1.5f - squereCoord.x) + textureColorRight*((squereCoord.x - 0.5f));
	else
		//horColor = textureColor;
		horColor = textureColor*((squereCoord.x + 0.5f)) + textureColorLeft*(0.5f - squereCoord.x);

	float4 verColor;
	if (squereCoord.y > 0.5f)
		horColor = horColor*(1.5f - squereCoord.y) + textureColorDown*((squereCoord.y - 0.5f));
	else
		//horColor = textureColor;
		horColor = horColor*((squereCoord.y + 0.5f)) + textureColorUp*(0.5f - squereCoord.y);

	textureColor = horColor;

	return textureColor;


}

float4 CalculateCurrentPhysical(float4 color, float2 mapCoord)
{
	float4 textureColor;

		
	if (NearColor(color, ConvertRGB256(0.0f, 165.0f, 232.0f))) //sea
		textureColor = textures[3].Sample(SampleType[2], mapCoord);
	else if (NearColor(color, ConvertRGB256(243.0f, 237.0f, 99.0f))) //beach
		textureColor = textures[4].Sample(SampleType[2], mapCoord);
	else if (NearColor(color, ConvertRGB256(0, 135.0f, 71.0f))) //forest
		textureColor = textures[5].Sample(SampleType[2], mapCoord);
	else if (NearColor(color, ConvertRGB256(161.0f, 194.0f, 49.0f))) //hills
		textureColor = textures[6].Sample(SampleType[2], mapCoord);
	else if (NearColor(color, ConvertRGB256(128.0f, 104.0f, 24.0f))) //middle mountains
		textureColor = textures[7].Sample(SampleType[2], mapCoord);
	else if (NearColor(color, ConvertRGB256(128.0f, 104.0f, 103.0f))) //big mountains
		textureColor = textures[8].Sample(SampleType[2], mapCoord);
	else if (NearColor(color, ConvertRGB256(30, 30, 30))) //cities
		textureColor = textures[9].Sample(SampleType[2], mapCoord);
	else
		textureColor = textures[3].Sample(SampleType[2], mapCoord);

	return textureColor;
}

bool NearColor(float4 color1, float4 color2)
{
	return (abs(color1.x - color2.x) < 0.08f && abs(color1.y - color2.y) < 0.08f && abs(color1.z - color2.z) < 0.08f);
}

float4 ConvertRGB256(float r, float g, float b)
{
	return float4(r / 256.0f, g / 256.0f, b / 256.0f, 1.0f);
}

float4 CalculateSkyColor(float3 pos)
{
	pos.x = pos.x / 768;
	pos.y = (768 - pos.z) / 768;

	float4 skyColor = textures[2].Sample(SampleType[0], pos.xy);
	skyColor = float4(1 - skyColor.x, 1 - skyColor.y, 1 - skyColor.z, skyColor.w);
	if (skyColor.x > 1)
		skyColor.x = 1;
	if (skyColor.y > 1)
		skyColor.y = 1;
	if (skyColor.z > 1)
		skyColor.z = 1;
	return skyColor;
}